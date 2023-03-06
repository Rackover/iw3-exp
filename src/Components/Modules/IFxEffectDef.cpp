#include "STDInclude.hpp"

#define IW4X_FX_VERSION 1

namespace Components
{
	void IFxEffectDef::ConvertFxElemVisuals(Game::IW3::FxElemVisuals* visuals, char iw4Type, Game::IW4::FxElemVisuals* target)
	{
		switch (iw4Type)
		{
		case Game::IW4::FX_ELEM_TYPE_MODEL:
		{
			if (visuals->model)
			{
				target->model = AssetHandler::Convert(Game::IW3::XAssetType::ASSET_TYPE_XMODEL, { visuals->model }).model;
			}

			break;
		}

		case Game::IW4::FX_ELEM_TYPE_RUNNER:
		{
			if (visuals->effectDef.handle)
			{
				target->effectDef.handle = AssetHandler::Convert(Game::IW3::XAssetType::ASSET_TYPE_FX, { visuals->effectDef.handle }).fx;
			}

			break;
		}

		case Game::IW4::FX_ELEM_TYPE_SOUND:
			target->soundName = visuals->soundName;
			break;

		case Game::IW4::FX_ELEM_TYPE_OMNI_LIGHT:
		case Game::IW4::FX_ELEM_TYPE_SPOT_LIGHT:
			break;

		default:
		{
			if (visuals->material)
			{
				// We reallocate it because we're copying it over to the new one and modifying it soo...
				auto backup = visuals->material->info.name;

				auto suffixed_name = std::string(backup) + IMaterialTechniqueSet::techsetSuffix;

				// We have to rename it because it gets otherwise shadowed by iw4!
				visuals->material->info.name = LocalAllocator.DuplicateString(suffixed_name);
				target->material = AssetHandler::Convert(Game::IW3::XAssetType::ASSET_TYPE_MATERIAL, { visuals->material }).material;

				assert(target->material);

				//// Restore original name
				visuals->material->info.name = backup;
			}

			break;
		}
		}
	}

	Game::IW4::FxEffectDef* IFxEffectDef::Convert(Game::IW3::FxEffectDef* fx)
	{
		if (!fx) return nullptr;
		Game::IW4::FxEffectDef newFx;
		std::memcpy(&newFx, fx, sizeof(newFx));

		int count = newFx.elemDefCountEmission + newFx.elemDefCountLooping + newFx.elemDefCountOneShot;
		newFx.elemDefs = LocalAllocator.AllocateArray<Game::IW4::FxElemDef>(count);

		for (int i = 0; i < count; ++i)
		{
			Game::IW4::FxElemDef* iw4ElemDef = &newFx.elemDefs[i];
			Game::IW3::FxElemDef* srcElemDef = &fx->elemDefs[i];

			static_assert(sizeof Game::IW3::FxElemDef == sizeof Game::IW4::FxElemDef);
			std::memcpy(iw4ElemDef, srcElemDef, sizeof(Game::IW4::FxElemDef));
			iw4ElemDef->bounds.compute(srcElemDef->collMins, srcElemDef->collMaxs);

			if (srcElemDef->elemType >= Game::IW3::FX_ELEM_TYPE_MODEL)
			{
				iw4ElemDef->elemType = static_cast<Game::IW4::FxElemType>(static_cast<int>(iw4ElemDef->elemType) + 2);
			}

			if (iw4ElemDef->elemType == Game::IW4::FX_ELEM_TYPE_DECAL)
			{
				if (iw4ElemDef->visuals.markArray)
				{
					for (char j = 0; j < iw4ElemDef->visualCount; ++j)
					{
						if (iw4ElemDef->visuals.markArray[j].materials[0])
						{
							iw4ElemDef->visuals.markArray[j].materials[0] =
								AssetHandler::Convert(Game::IW3::XAssetType::ASSET_TYPE_MATERIAL, {
									srcElemDef->visuals.markArray[j].materials[0]
									}).material;
						}

						if (iw4ElemDef->visuals.markArray[j].materials[1])
						{
							iw4ElemDef->visuals.markArray[j].materials[1] =
								AssetHandler::Convert(Game::IW3::XAssetType::ASSET_TYPE_MATERIAL, {
									srcElemDef->visuals.markArray[j].materials[1]
									}).material;
						}
					}
				}
			}
			else if (iw4ElemDef->visualCount > 1)
			{
				if (iw4ElemDef->visuals.array)
				{
					iw4ElemDef->visuals.array = LocalAllocator.AllocateArray<Game::IW4::FxElemVisuals>(srcElemDef->visualCount);

					for (char j = 0; j < srcElemDef->visualCount; ++j)
					{
						iw4ElemDef->visuals.array[j].anonymous = nullptr;
						ConvertFxElemVisuals(&srcElemDef->visuals.array[j],  iw4ElemDef->elemType, &iw4ElemDef->visuals.array[j]);

						if (srcElemDef->visuals.array[j].anonymous)
						{
							assert(iw4ElemDef->visuals.array[j].anonymous);
						}
					}
				}
			}
			else if (iw4ElemDef->visualCount == 1)
			{
				iw4ElemDef->visuals.instance.anonymous = nullptr; // Making sure it's dead
				ConvertFxElemVisuals(&srcElemDef->visuals.instance, iw4ElemDef->elemType, &iw4ElemDef->visuals.instance);

				if (srcElemDef->visuals.instance.anonymous)
				{
					assert(iw4ElemDef->visuals.instance.anonymous);
				}
			}

			if (iw4ElemDef->trailDef)
			{
				if (iw4ElemDef->elemType == Game::IW4::FX_ELEM_TYPE_TRAIL)
				{
					iw4ElemDef->trailDef = LocalAllocator.Allocate<Game::IW4::FxTrailDef>();

					iw4ElemDef->trailDef->scrollTimeMsec = srcElemDef->trailDef->scrollTimeMsec;
					iw4ElemDef->trailDef->repeatDist = srcElemDef->trailDef->repeatDist;

					// Not sure about those
					// TODO: Talk to aerosoul94
					iw4ElemDef->trailDef->invSplitDist = static_cast<float>(srcElemDef->trailDef->splitDist);
					iw4ElemDef->trailDef->invSplitArcDist = static_cast<float>(iw4ElemDef->trailDef->invSplitDist);
					iw4ElemDef->trailDef->invSplitTime = static_cast<float>(iw4ElemDef->trailDef->scrollTimeMsec);

					iw4ElemDef->trailDef->vertCount = srcElemDef->trailDef->vertCount;
					iw4ElemDef->trailDef->verts = srcElemDef->trailDef->verts;
					iw4ElemDef->trailDef->indCount = srcElemDef->trailDef->indCount;
					iw4ElemDef->trailDef->inds = srcElemDef->trailDef->inds;
				}
				else
				{
					Logger::Print("Fx element of type %d has a trailDef, that shouldn't happen!\n", iw4ElemDef->elemType & 0xF);
					assert(false);
					iw4ElemDef->trailDef = nullptr;
				}
			}

			if (srcElemDef->effectOnImpact.handle)
			{
				iw4ElemDef->effectOnImpact.handle = AssetHandler::Convert(Game::IW3::XAssetType::ASSET_TYPE_FX, { srcElemDef->effectOnImpact.handle }).fx;
			}

			if (srcElemDef->effectOnDeath.handle)
			{
				iw4ElemDef->effectOnDeath.handle = AssetHandler::Convert(Game::IW3::XAssetType::ASSET_TYPE_FX, { srcElemDef->effectOnDeath.handle }).fx;
			}

			if (srcElemDef->effectEmitted.handle)
			{
				iw4ElemDef->effectEmitted.handle = AssetHandler::Convert(Game::IW3::XAssetType::ASSET_TYPE_FX, { srcElemDef->effectEmitted.handle }).fx;
			}
		}

		auto allocated = LocalAllocator.Allocate<Game::IW4::FxEffectDef>();
		*allocated = newFx;

		return allocated;
	}

	IFxEffectDef::IFxEffectDef()
	{
		Command::Add("dumpFxEffectDef", [](Command::Params params)
			{
				if (params.Length() < 2) return;
				auto converted = IFxEffectDef::Convert(Game::DB_FindXAssetHeader(Game::IW3::XAssetType::ASSET_TYPE_FX, params[1]).fx);
				MapDumper::GetApi()->write(Game::IW4::XAssetType::ASSET_TYPE_FX, converted);
			});
	}

	IFxEffectDef::~IFxEffectDef()
	{

	}
}
