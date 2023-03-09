#include "STDInclude.hpp"

#define IW4X_IMG_VERSION "0"

static_assert(sizeof Game::IW4::GfxImageFileHeader::tag == sizeof Game::IW3::GfxImageFileHeader::tag, "tag size mismatch!");
static_assert(sizeof Game::IW4::GfxImageFileHeader::dimensions == sizeof Game::IW3::GfxImageFileHeader::dimensions, "dimensions size mismatch!");
static_assert(sizeof Game::IW4::GfxImageFileHeader::fileSizeForPicmip == sizeof Game::IW3::GfxImageFileHeader::fileSizeForPicmip, "fileSizeForPicmip size mismatch!");

namespace Components
{
	std::string IGfxImage::ConvertIWIOnTheFly(const std::string& filename)
	{
		FileSystem::File img(filename);

		if (!img.Exists())
		{
			Logger::Print("Image %s not found, mapping to normalmap!\n", filename.data());
			img = FileSystem::File("images/$identitynormalmap.iwi");
		}

		if (img.Exists())
		{
			Game::IW4::GfxImageFileHeader header_iw4{};
			const Game::IW3::GfxImageFileHeader header_iw3 = *reinterpret_cast<const Game::IW3::GfxImageFileHeader*>(img.GetBuffer().data());

			header_iw4.version = 8;
			header_iw4.flags = 0;
			header_iw4.format = header_iw3.format;

			header_iw4.flags = TranslateFlags(header_iw3.flags);

			std::memcpy(header_iw4.tag, header_iw3.tag, sizeof header_iw4.tag);
			std::memcpy(header_iw4.dimensions, header_iw3.dimensions, sizeof header_iw4.dimensions);
			std::memcpy(header_iw4.fileSizeForPicmip, header_iw3.fileSizeForPicmip, sizeof header_iw4.fileSizeForPicmip);

			// Fix sizes
			for (int i = 0; i < 4; ++i)
			{
				int* size = &header_iw4.fileSizeForPicmip[i];

				if (*size)
				{
					*size -= sizeof(Game::IW3::GfxImageFileHeader);
					*size += sizeof(Game::IW4::GfxImageFileHeader);
				}
			}

			std::string buffer(reinterpret_cast<char*>(&header_iw4), sizeof Game::IW4::GfxImageFileHeader);
			buffer.append(img.GetBuffer().data() + (sizeof Game::IW3::GfxImageFileHeader), img.GetBuffer().size() - (sizeof Game::IW3::GfxImageFileHeader));

			return buffer;
		}

		Logger::Print("Unable to map to normalmap, this should not happen!\n");
		return "";
	}

	Game::IW4::GfxImageLoadDef* IGfxImage::ConvertLoadDef(Game::IW3::GfxImageLoadDef* iw3LoadDef)
	{
		auto iw4Def = reinterpret_cast<Game::IW4::GfxImageLoadDef*>(LocalAllocator.Allocate(sizeof(Game::IW4::GfxImageLoadDef) + iw3LoadDef->resourceSize - 4));

		std::memcpy(iw4Def->data, iw3LoadDef->data, iw3LoadDef->resourceSize);
		iw4Def->flags = TranslateFlags(iw3LoadDef->flags);
		iw4Def->format = iw3LoadDef->format;
		iw4Def->levelCount = iw3LoadDef->levelCount;
		iw4Def->resourceSize = iw3LoadDef->resourceSize;

		return iw4Def;
	}

	Game::IW4::GfxTexture* IGfxImage::ConvertTexture(Game::IW3::GfxTexture* iw3Texture)
	{
		auto iw4Tex = reinterpret_cast<Game::IW4::GfxTexture*>(LocalAllocator.Allocate<Game::IW4::GfxTexture>());

		iw4Tex->loadDef = ConvertLoadDef(iw3Texture->loadDef);

		return iw4Tex;
	}

	Game::IW4::GfxImage* IGfxImage::Convert(Game::IW3::GfxImage* image)
	{
		if (!image) return nullptr;
		std::string name = image->name;
		if (name[0] == '*') name.erase(name.begin());

		static auto correctSpeculars = Game::Dvar_FindVar("iw3x_correct_speculars");
		if (correctSpeculars && correctSpeculars->current.enabled) {
			if (Utils::StartsWith(name, "reflection_probe")) {
				CorrectSpecularImage(image);
			}
		}


		Game::IW4::GfxImage* iw4Image = LocalAllocator.Allocate<Game::IW4::GfxImage>();

		iw4Image->texture.loadDef = ConvertLoadDef(image->texture.loadDef);

		iw4Image->mapType = static_cast<unsigned char>(image->mapType);
		iw4Image->semantic = static_cast<unsigned char>(image->semantic);
		iw4Image->category = image->category;
		iw4Image->picmip = image->picmip;
		iw4Image->noPicmip = image->noPicmip;
		iw4Image->track = image->track;
		iw4Image->cardMemory = image->cardMemory;
		iw4Image->width = image->width;
		iw4Image->height = image->height;
		iw4Image->depth = image->depth;
		iw4Image->delayLoadPixels = image->delayLoadPixels;
		iw4Image->name = image->name;

		return iw4Image;
	}

	void IGfxImage::CorrectSpecularImage(Game::IW3::GfxImage* image) {

		Components::Logger::Print("Regenerating specular image %s...\n", image->name);

		assert(image->mapType == Game::IW3::MAPTYPE_CUBE);

#define SIDES 6 // Cube has 6 SIDES!
#define CHANNELS 4 // R G B and A
#define IWI_HEADER_SIZE 28 // This would be 32 for IW4, and it's 28 for IW3. This is the size of the header on .IWI file before the actual data

		std::string mapName = MapDumper::GetMapName();

		// Here's the plan :
		// 
		// iw3 has baked images for specular reflection (metallic reflection, sniper scope reflection)
		//	 but they're too contrasted and burnt for proper reflections in iw4
		// They're usually damaged so badly that we can't use them at all.
		// 
		// As a workaround, we can use the loadscreen image of the map (as it usually has the same colors as the map or almost)
		//	as a baked reflection probe. We still keep the alpha channel from the original probe so we have a bit of
		//	difference between each probe on the map, but we take the RGB from the loadscreen image.
		// That image has to be decoded manually because the game doesn't know how to decompress it. For iw3, its
		//	format will always be DXT1.
		// Then we also need to change the pixel data to make it a bit less contrasted and bright, otherwise
		//	the specular reflections are very aggressive. That's about it! :)

		std::vector<std::tuple<unsigned int, unsigned int>> mips = std::vector<std::tuple<unsigned int, unsigned int>>();
		[[maybe_unused]] unsigned int totalSize = 0;
		if (image->noPicmip == false) // => Has mipmaps
		{
			unsigned short maxDimension = std::max(image->height, image->width);
			int mipmapFactor = 1;
			int minBlockSize = 1;

			while (maxDimension != 0)
			{
				maxDimension >>= 1;
				auto x = std::max(image->width / mipmapFactor, minBlockSize);
				auto y = std::max(image->height / mipmapFactor, minBlockSize);
				totalSize += (x) * (y)*CHANNELS;
				mips.emplace_back(std::tuple<int, int>(x, y));
				mipmapFactor *= 2;
			}

		}
		else {
			totalSize = image->width * image->height * CHANNELS;
			mips.emplace_back(std::tuple<int, int>(image->width, image->height));
		}

		size_t dataIndex = 0;
		for (size_t i = 0; i < mips.size(); i++)
		{
			unsigned int thisWidth = std::get<0>(mips[i]);
			unsigned int thisHeight = std::get<1>(mips[i]);

			size_t faceSize = thisWidth * thisHeight * CHANNELS;

			for (size_t side = 0; side < SIDES; side++)
			{
				Utils::Memory::Allocator allocator;

				// Blur it !
				unsigned char* buff1 = allocator.AllocateArray<unsigned char>(16384); // 64 * 64 * 4, should be enough?
				unsigned char* buff2 = allocator.AllocateArray<unsigned char>(16384); // 64 * 64 * 4, should be enough?

				memcpy_s(buff1, faceSize, &image->texture.loadDef->data[dataIndex], faceSize);

				for (size_t passes = 0; passes < 1; passes++)
				{
					Utils::Image::GaussianBlur4(buff1, buff2, thisWidth, thisHeight, CHANNELS, 2);
				}

				auto dataStart = dataIndex;
				for (size_t x = 0; x < thisWidth; x++)
				{
					for (size_t y = 0; y < thisHeight; y++)
					{
						for (size_t channel = 0; channel < CHANNELS; channel++)
						{
							auto localIndex = dataIndex - dataStart;

							// Tone it down
							unsigned char newByte = buff2[localIndex];

							if (channel < CHANNELS - 1) {

								auto otherChannel = (channel + 1) % (CHANNELS - 1);
								auto otherOtherChannel = (channel + 2) % (CHANNELS - 1);

								auto otherColor = buff2[localIndex - channel + otherChannel];
								auto otherOtherColor = buff2[localIndex - channel + otherOtherChannel];

								unsigned char average = (newByte + otherColor + otherOtherColor) / 3;

								newByte = static_cast<unsigned char>(std::lerp(newByte, average, 0.15f));
								newByte = std::clamp(newByte, static_cast<unsigned char>(25), static_cast<unsigned char>(170));
							}
							else {
								// Alpha channel - we lower it a little bit
								newByte = static_cast<unsigned char>(image->texture.loadDef->data[dataIndex] * 0.75); // pretty sure this does nothing! Alpha channel seems unused once ingame
							}

							image->texture.loadDef->data[dataIndex] = newByte;
							dataIndex++;
						}
					}
				}
			}
		}
	}

	int IGfxImage::StoreTexture()
	{
		Game::IW3::GfxImageLoadDef** loadDef = *reinterpret_cast<Game::IW3::GfxImageLoadDef***>(0xE34814);
		Game::IW3::GfxImage* image = *reinterpret_cast<Game::IW3::GfxImage**>(0xE346C4);

		size_t size = 16 + (*loadDef)->resourceSize;
		void* data = Loader::GetAlloctor()->Allocate(size);
		std::memcpy(data, *loadDef, size);

		image->texture.loadDef = reinterpret_cast<Game::IW3::GfxImageLoadDef*>(data);

		return 0;
	}

	void IGfxImage::ReleaseTexture(Game::IW3::XAssetHeader header)
	{
		if (header.image && header.image->texture.loadDef)
		{
			Loader::GetAlloctor()->free(header.image->texture.loadDef);
		}
	}

	int Components::IGfxImage::TranslateFlags(int iw3Flags)
	{
		int iw4Flags = 0;

		const auto translateFlags = [&](Game::IW3::file_image_flags_t sourceFlag, Game::IW4::file_image_flags_t targetFlag)
		{
			if (iw3Flags & sourceFlag)
			{
				iw4Flags |= targetFlag;
			}
		};

		translateFlags(Game::IW3::IMG_FLAG_NOPICMIP, Game::IW4::IMG_FLAG_NOPICMIP);
		translateFlags(Game::IW3::IMG_FLAG_NOMIPMAPS, Game::IW4::IMG_FLAG_NOMIPMAPS);
		translateFlags(Game::IW3::IMG_FLAG_CUBEMAP, Game::IW4::IMG_FLAG_MAPTYPE_CUBE);
		translateFlags(Game::IW3::IMG_FLAG_VOLMAP, Game::IW4::IMG_FLAG_MAPTYPE_3D);
		translateFlags(Game::IW3::IMG_FLAG_STREAMING, Game::IW4::IMG_FLAG_STREAMING);
		translateFlags(Game::IW3::IMG_FLAG_LEGACY_NORMALS, Game::IW4::IMG_FLAG_LEGACY_NORMALS);
		translateFlags(Game::IW3::IMG_FLAG_CLAMP_U, Game::IW4::IMG_FLAG_CLAMP_U);
		translateFlags(Game::IW3::IMG_FLAG_CLAMP_V, Game::IW4::IMG_FLAG_CLAMP_V);

		// Not sure about these
		//translateFlags(Game::IW3::IMG_FLAG_DYNAMIC,        Game::IW4::IMG_FLAG_DYNAMIC);
		//translateFlags(Game::IW3::IMG_FLAG_RENDER_TARGET,  Game::IW4::IMG_FLAG_RENDER_TARGET);
		//translateFlags(Game::IW3::IMG_FLAG_SYSTEMMEM,      Game::IW4::IMG_FLAG_SYSTEMMEM);

		//iw4Flags |= Game::IW4::IMG_FLAG_ALPHA_WEIGHTED_COLORS;
		//iw4Flags |= Game::IW4::IMG_FLAG_GAMMA_SRGB;

		return iw4Flags;
	}

	IGfxImage::IGfxImage()
	{
		Command::Add("dumpGfxImage", [](const Command::Params& params)
			{
				if (params.Length() < 2) return;

				Game::IW3::GfxImage* image = Game::DB_FindXAssetHeader(Game::IW3::ASSET_TYPE_IMAGE, params[1]).image;

				auto converted = IGfxImage::Convert(image);
				MapDumper::GetApi()->write(Game::IW4::XAssetType::ASSET_TYPE_IMAGE, converted);
			});

		Utils::Hook(0x616E80, IGfxImage::StoreTexture, HOOK_JUMP).install()->quick();
		Utils::Hook(0x488C00, IGfxImage::ReleaseTexture, HOOK_JUMP).install()->quick();
	}

	IGfxImage::~IGfxImage()
	{

	}
}
