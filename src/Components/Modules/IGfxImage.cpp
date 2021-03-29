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

				translateFlags(Game::IW3::IMG_FLAG_NOPICMIP,       Game::IW4::IMG_FLAG_NOPICMIP);
				translateFlags(Game::IW3::IMG_FLAG_NOMIPMAPS,      Game::IW4::IMG_FLAG_NOMIPMAPS);
				translateFlags(Game::IW3::IMG_FLAG_CUBEMAP,        Game::IW4::IMG_FLAG_MAPTYPE_CUBE);
				translateFlags(Game::IW3::IMG_FLAG_VOLMAP,         Game::IW4::IMG_FLAG_MAPTYPE_3D);
				translateFlags(Game::IW3::IMG_FLAG_STREAMING,      Game::IW4::IMG_FLAG_STREAMING);
				translateFlags(Game::IW3::IMG_FLAG_LEGACY_NORMALS, Game::IW4::IMG_FLAG_LEGACY_NORMALS);
				translateFlags(Game::IW3::IMG_FLAG_CLAMP_U,        Game::IW4::IMG_FLAG_CLAMP_U);
				translateFlags(Game::IW3::IMG_FLAG_CLAMP_V,        Game::IW4::IMG_FLAG_CLAMP_V);
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

	int IGfxImage::StoreTexture()
	{
		Game::IW3::GfxImageLoadDef ** loadDef = *reinterpret_cast<Game::IW3::GfxImageLoadDef***>(0xE34814);
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
