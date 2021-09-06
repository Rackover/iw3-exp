#include "STDInclude.hpp"

#define IW4X_IMG_VERSION "0"

static_assert(sizeof Game::IW4::GfxImageFileHeader::tag == sizeof Game::IW3::GfxImageFileHeader::tag, "tag size mismatch!");
static_assert(sizeof Game::IW4::GfxImageFileHeader::dimensions == sizeof Game::IW3::GfxImageFileHeader::dimensions, "dimensions size mismatch!");
static_assert(sizeof Game::IW4::GfxImageFileHeader::fileSizeForPicmip == sizeof Game::IW3::GfxImageFileHeader::fileSizeForPicmip, "fileSizeForPicmip size mismatch!");

namespace Components
{
	void IGfxImage::Dump(Game::IW3::GfxImage* image)
	{
		if (!image) return;
		std::string name = image->name;

		if (image->texture.loadDef && image->texture.loadDef->resourceSize > 0)
		{
			if (name[0] == '*') name.erase(name.begin());

			if (Utils::StartsWith(name, "reflection_probe")) {
				CorrectSpecularImage(image);
			}

			Utils::Stream buffer;
			buffer.saveArray("IW4xImg" IW4X_IMG_VERSION, 8); // just stick version in the magic since we have an extra char

			buffer.saveObject(static_cast<unsigned char>(image->mapType));
			buffer.saveObject(image->semantic);
			buffer.saveObject(image->category);

			buffer.saveObject(image->texture.loadDef->resourceSize);
			buffer.save(image->texture.loadDef, 16 + image->texture.loadDef->resourceSize);

			Utils::WriteFile(Utils::VA("%s/images/%s.iw4xImage", AssetHandler::GetExportPath().data(), name.data()), buffer.toBuffer());
		}
		else
		{
			FileSystem::File img(Utils::VA("images/%s.iwi", image->name));

			if (!img.Exists())
			{
				// Ignore that
				if (Utils::StartsWith(image->name, "watersetup")) return;

				Logger::Print("Image %s not found, mapping to normalmap!\n", name.data());
				img = FileSystem::File("images/$identitynormalmap.iwi");
			}

			if (img.Exists())
			{
				Game::IW4::GfxImageFileHeader header_iw4;
				const Game::IW3::GfxImageFileHeader header_iw3 = *reinterpret_cast<const Game::IW3::GfxImageFileHeader*>(img.GetBuffer().data());

				header_iw4.version = 8;
				header_iw4.flags = 0;
				header_iw4.format = header_iw3.format;

				const auto translateFlags = [&](Game::IW3::file_image_flags_t sourceFlag, Game::IW4::file_image_flags_t targetFlag)
				{
					if (header_iw3.flags & sourceFlag)
					{
						header_iw4.flags |= targetFlag;
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
				// 				translateFlags(Game::IW3::IMG_FLAG_DYNAMIC,        Game::IW4::IMG_FLAG_DYNAMIC);
				// 				translateFlags(Game::IW3::IMG_FLAG_RENDER_TARGET,  Game::IW4::IMG_FLAG_RENDER_TARGET);
				// 				translateFlags(Game::IW3::IMG_FLAG_SYSTEMMEM,      Game::IW4::IMG_FLAG_SYSTEMMEM);

				header_iw4.flags |= Game::IW4::IMG_FLAG_ALPHA_WEIGHTED_COLORS;
				header_iw4.flags |= Game::IW4::IMG_FLAG_GAMMA_SRGB;

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

				Utils::WriteFile(Utils::VA("%s/images/%s.iwi", AssetHandler::GetExportPath().data(), image->name), buffer);
			}
			else
			{
				Logger::Print("Unable to map to normalmap, this should not happen!\n");
			}
		}
	}

	void IGfxImage::CorrectSpecularImage(Game::IW3::GfxImage* image) {
		assert(image->mapType == Game::IW3::MAPTYPE_CUBE);

		#define SIDES 6 // Cube has 6 SIDES!
		#define CHANNELS 4 // R G B and A
		#define IWI_HEADER_SIZE 28 // This would be 32 for IW4, and it's 28 for IW3. This is the size of the header on .IWI file before the actual data

#ifdef DEBUG
		const unsigned int sizeOfASide = image->texture.loadDef->resourceSize / SIDES;
#endif
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

		Game::IW3::XAssetHeader baseMapHeader = Game::DB_FindXAssetHeader(Game::XAssetType::ASSET_TYPE_IMAGE, Utils::VA("loadscreen_%s", mapName.c_str()));
		auto baseMapImg = baseMapHeader.image;

		const int pixels = CHANNELS * baseMapImg->width * baseMapImg->height;

		// Fetch & decompress loadscreen image
		// Note: We don't consider mipmaps here. Loadscreens shouldn't have mips, but
		//	actually on some custom maps, they do. 
		// I think it's okay. It will be technically incorrect but the colors will be right.

		FileSystem::File baseImg(Utils::VA("images/%s.iwi", Utils::VA("loadscreen_%s", mapName.c_str())));
		std::vector<uint32_t> replacementImageBuffer = std::vector<uint32_t>(pixels);
		unsigned char* iwiData = reinterpret_cast<unsigned char*>(baseImg.GetBuffer().data());
		char iwiFormat = iwiData[4]; // I + W + i + 6  and the next char is => format.
		unsigned char* dxtRawDataStart = &iwiData[IWI_HEADER_SIZE];

		if (iwiFormat == Game::GfxImageFileFormat::IMG_FORMAT_DXT1) {
			BlockDecompressImageDXT1(baseMapImg->width, baseMapImg->height, dxtRawDataStart, reinterpret_cast<unsigned long*>(&replacementImageBuffer[0]));
		}
		else if (iwiFormat == Game::GfxImageFileFormat::IMG_FORMAT_DXT5) {
			BlockDecompressImageDXT5(baseMapImg->width, baseMapImg->height, dxtRawDataStart, reinterpret_cast<unsigned long*>(&replacementImageBuffer[0]));
		}
		else {
			// No can do! I don't know how to decompress this image.
			// No correction will be applied
			return;
		}

		int dataIndex = 0;

		std::vector<std::tuple<unsigned int, unsigned int>> mips = std::vector<std::tuple<unsigned int, unsigned int>>();
		if (image->noPicmip == false) // => Has mipmaps
		{
			unsigned short maxDimension = max(image->height, image->width);
			int mipmapFactor = 1;
			int minBlockSize = 1;
			unsigned int totalSize = 0;

			while (maxDimension != 0)
			{
				maxDimension >>= 1;
				auto x = max(image->width / mipmapFactor, minBlockSize);
				auto y = max(image->height / mipmapFactor, minBlockSize);
				totalSize += (x) * (y)*CHANNELS;
				mips.emplace_back(std::tuple<int, int>(x, y));
				mipmapFactor *= 2;
			}
#if DEBUG
			assert(totalSize == sizeOfASide);
#endif
		}
		else {
			mips.emplace_back(std::tuple<int, int>(image->width, image->height));
		}

		float xStep = (float)baseMapImg->width / (float)image->width;
		float yStep = (float)baseMapImg->height / (float)image->height;

		for (size_t i = 0; i < mips.size(); i++)
		{
			unsigned int thisWidth = std::get<0>(mips[i]);
			unsigned int thisHeight = std::get<1>(mips[i]);

			for (size_t side = 0; side < SIDES; side++)
			{
				for (size_t x = 0; x < thisWidth; x++)
				{
					for (size_t y = 0; y < thisHeight; y++)
					{
						union {
							char byteValue[4];
							long longValue;
						} baseMapPixels;

						// Note: Rotation here is incorrect. It should be the following:
						// 
						//  side 0 is rotated ccw 90°
						//  side 1 is rotated cw 90°
						//	side 2 is rotated 180°
						//	side 3 is not rotated
						//	side 4 is not rotated
						//	side 5 is not rotated
						// 
						// I just don't have the time to do that at the moment, but feel
						//	free to implement it in the future! switch(side){...}

						size_t newPixelIndex = static_cast<size_t>(std::floor(xStep * x) * baseMapImg->width + std::floor(yStep * y));
						baseMapPixels.longValue = replacementImageBuffer[newPixelIndex];

						for (size_t channel = 0; channel < CHANNELS; channel++)
						{
							unsigned char newByte;

							if (channel < CHANNELS - 1) {
								newByte = baseMapPixels.byteValue[channel + 1];
								newByte = std::clamp(newByte, static_cast<unsigned char>(60), static_cast<unsigned char>(170));
								newByte = static_cast<unsigned char>(std::lerp(newByte, 127, 0.3f));
							}
							else {
								// Alpha channel - we lower it a little bit
								newByte = baseMapPixels.byteValue[channel - CHANNELS + 1];
								newByte = static_cast<unsigned char>(newByte * 0.75);
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
		void* data = Loader::GetAlloctor()->allocate(size);
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

	IGfxImage::IGfxImage()
	{
		Command::Add("dumpGfxImage", [](Command::Params params)
			{
				if (params.Length() < 2) return;

				Game::IW3::GfxImage image;
				image.name = params[1];
				image.texture.loadDef = nullptr;

				IGfxImage::Dump(&image);
			});

		Utils::Hook(0x616E80, IGfxImage::StoreTexture, HOOK_JUMP).install()->quick();
		Utils::Hook(0x488C00, IGfxImage::ReleaseTexture, HOOK_JUMP).install()->quick();
	}

	IGfxImage::~IGfxImage()
	{

	}
}
