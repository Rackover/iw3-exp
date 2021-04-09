#include "STDInclude.hpp"

#define IW4X_FX_VERSION 1

namespace Components
{
	void IFxEffectDef::SaveFxElemVisuals(Game::IW3::FxElemVisuals* visuals, char elemType, Utils::Stream* buffer)
	{
		switch (elemType)
		{
		case Game::IW4::FX_ELEM_TYPE_MODEL:
		{
			if (visuals->model)
			{
				buffer->saveString(visuals->model->name);
				AssetHandler::Dump(Game::XAssetType::ASSET_TYPE_XMODEL, { visuals->model });
			}

			break;
		}

		case Game::IW4::FX_ELEM_TYPE_OMNI_LIGHT:
		case Game::IW4::FX_ELEM_TYPE_SPOT_LIGHT:
			break;

		case Game::IW4::FX_ELEM_TYPE_SOUND:
		{
			if (visuals->soundName)
			{
				buffer->saveString(visuals->soundName);
				//Logger::Print("Unable to dump sounds yet!\n");
				AssetHandler::Dump(Game::XAssetType::ASSET_TYPE_SOUND, { Game::DB_FindXAssetHeader(Game::XAssetType::ASSET_TYPE_SOUND, visuals->soundName).sound });
			}

			break;
		}

		case Game::IW4::FX_ELEM_TYPE_RUNNER:
		{
			if (visuals->effectDef.handle)
			{
				buffer->saveString(visuals->effectDef.handle->name);
				AssetHandler::Dump(Game::XAssetType::ASSET_TYPE_FX, { visuals->effectDef.handle });
			}

			break;
		}

		default:
		{
			if (visuals->material)
			{
				buffer->saveString(visuals->material->info.name);
				AssetHandler::Dump(Game::XAssetType::ASSET_TYPE_MATERIAL, { visuals->material });
			}

			break;
		}
		}
	}

	void IFxEffectDef::SaveConvertedFx(Game::IW4::FxEffectDef* asset)
	{
		AssertSize(Game::IW3::FxEffectDef, 32);

		Utils::Stream buffer;
		buffer.saveArray("IW4xFx  ", 8); // No idea what to fill in
		buffer.saveObject(IW4X_FX_VERSION);

		buffer.saveObject(*asset);

		if (asset->name)
		{
			buffer.saveString(asset->name);
		}

		if (asset->elemDefs)
		{
			AssertSize(Game::IW3::FxElemDef, 252);

			buffer.saveArray(asset->elemDefs, asset->elemDefCountEmission + asset->elemDefCountLooping + asset->elemDefCountOneShot);

			for (int i = 0; i < (asset->elemDefCountEmission + asset->elemDefCountLooping + asset->elemDefCountOneShot); ++i)
			{
				Game::IW4::FxElemDef* elemDef = &asset->elemDefs[i];

				if (elemDef->velSamples)
				{
					AssertSize(Game::IW3::FxElemVelStateSample, 96);
					buffer.saveArray(elemDef->velSamples, elemDef->velIntervalCount + 1);
				}

				if (elemDef->visSamples)
				{
					AssertSize(Game::IW3::FxElemVisStateSample, 48);
					buffer.saveArray(elemDef->visSamples, elemDef->visStateIntervalCount + 1);
				}

				// Save_FxElemDefVisuals
				{
					if (elemDef->elemType == Game::IW4::FX_ELEM_TYPE_DECAL)
					{
						if (elemDef->visuals.markArray)
						{
							AssertSize(Game::IW3::FxElemMarkVisuals, 8);
							buffer.saveArray(elemDef->visuals.markArray, elemDef->visualCount);

							for (char j = 0; j < elemDef->visualCount; ++j)
							{
								if (elemDef->visuals.markArray[j].materials[0])
								{
									buffer.saveString(elemDef->visuals.markArray[j].materials[0]->info.name);
									AssetHandler::Dump(Game::XAssetType::ASSET_TYPE_MATERIAL, { elemDef->visuals.markArray[j].materials[0] });
								}

								if (elemDef->visuals.markArray[j].materials[1])
								{
									buffer.saveString(elemDef->visuals.markArray[j].materials[1]->info.name);
									AssetHandler::Dump(Game::XAssetType::ASSET_TYPE_MATERIAL, { elemDef->visuals.markArray[j].materials[1] });
								}
							}
						}
					}
					else if (elemDef->visualCount > 1)
					{
						if (elemDef->visuals.array)
						{
							AssertSize(Game::IW3::FxElemVisuals, 4);
							buffer.saveArray(elemDef->visuals.array, elemDef->visualCount);

							for (char j = 0; j < elemDef->visualCount; ++j)
							{
								IFxEffectDef::SaveFxElemVisuals(&elemDef->visuals.array[j], elemDef->elemType, &buffer);
							}
						}
					}
					else
					{
						IFxEffectDef::SaveFxElemVisuals(&elemDef->visuals.instance, elemDef->elemType, &buffer);
					}
				}

				if (elemDef->effectOnImpact)
				{
					buffer.saveString(elemDef->effectOnImpact->name);
					AssetHandler::Dump(Game::XAssetType::ASSET_TYPE_FX, { Game::DB_FindXAssetHeader(Game::XAssetType::ASSET_TYPE_FX, elemDef->effectOnImpact->name).fx });
				}

				if (elemDef->effectOnDeath)
				{
					buffer.saveString(elemDef->effectOnDeath->name);
					AssetHandler::Dump(Game::XAssetType::ASSET_TYPE_FX, { Game::DB_FindXAssetHeader(Game::XAssetType::ASSET_TYPE_FX, elemDef->effectOnDeath->name).fx });
				}

				if (elemDef->effectEmitted)
				{
					buffer.saveString(elemDef->effectEmitted->name);
					AssetHandler::Dump(Game::XAssetType::ASSET_TYPE_FX, { Game::DB_FindXAssetHeader(Game::XAssetType::ASSET_TYPE_FX, elemDef->effectEmitted->name).fx });
				}

				// Save_FxElemExtendedDefPtr
				{

					if (elemDef->elemType == Game::IW4::FX_ELEM_TYPE_TRAIL)
					{
						// Save_FxTrailDef
						{
							if (elemDef->trailDef)
							{
								AssertSize(Game::IW4::FxTrailDef, 36);

								Game::IW4::FxTrailDef* trailDef = elemDef->trailDef;
								buffer.saveObject(*trailDef);

								if (trailDef->verts)
								{
									AssertSize(Game::IW3::FxTrailVertex, 20);

									buffer.saveArray(trailDef->verts, trailDef->vertCount);
								}

								if (trailDef->inds)
								{
									buffer.saveArray(trailDef->inds, trailDef->indCount);
								}
							}
						}
					}
					else if(elemDef->trailDef)
					{
						Logger::Error("Fx element of type %d has traildef, that's impossible?\n", elemDef->elemType);
					}
				}
			}
		}

		Utils::WriteFile(Utils::VA("%s/fx/%s.iw4xFx", AssetHandler::GetExportPath().data(), asset->name), buffer.toBuffer());
	}

	void IFxEffectDef::Dump(Game::IW3::FxEffectDef* fx)
	{
		if (!fx) return;
		Utils::Memory::Allocator allocator;

		Game::IW4::FxEffectDef newFx;
		std::memcpy(&newFx, fx, sizeof(newFx));

		int count = newFx.elemDefCountEmission + newFx.elemDefCountLooping + newFx.elemDefCountOneShot;
		newFx.elemDefs = allocator.allocateArray<Game::IW4::FxElemDef>(count);

		for (int i = 0; i < count; ++i)
		{
			Game::IW4::FxElemDef* elemDef = &newFx.elemDefs[i];
			Game::IW3::FxElemDef* srcElemDef = &fx->elemDefs[i];

			std::memcpy(elemDef, srcElemDef, sizeof(Game::IW4::FxElemDef));
			elemDef->bounds.compute(srcElemDef->collMins, srcElemDef->collMaxs);
			
			if(elemDef->elemType >= Game::IW3::FX_ELEM_TYPE_MODEL)
			{
				elemDef->elemType += 2;
			}

			if (elemDef->trailDef)
			{
				if (elemDef->elemType == Game::IW4::FX_ELEM_TYPE_TRAIL)
				{
					elemDef->trailDef = allocator.allocate<Game::IW4::FxTrailDef>();

					elemDef->trailDef->scrollTimeMsec = srcElemDef->trailDef->scrollTimeMsec;
					elemDef->trailDef->repeatDist = srcElemDef->trailDef->repeatDist;

					// Not sure about those
					// TODO: Talk to aerosoul94
					elemDef->trailDef->invSplitDist = static_cast<float>(srcElemDef->trailDef->splitDist);
					elemDef->trailDef->invSplitArcDist = static_cast<float>(elemDef->trailDef->invSplitDist);
					elemDef->trailDef->invSplitTime = static_cast<float>(elemDef->trailDef->scrollTimeMsec);

					elemDef->trailDef->vertCount = srcElemDef->trailDef->vertCount;
					elemDef->trailDef->verts = srcElemDef->trailDef->verts;
					elemDef->trailDef->indCount = srcElemDef->trailDef->indCount;
					elemDef->trailDef->inds = srcElemDef->trailDef->inds;
				}
				else
				{
					Logger::Print("Fx element of type %d has a trailDef, that shouldn't happen!\n", elemDef->elemType & 0xF);
					elemDef->trailDef = nullptr;
				}
			}
		}

		IFxEffectDef::SaveConvertedFx(&newFx);
	}

	IFxEffectDef::IFxEffectDef()
	{
		Command::Add("dumpFxEffectDef", [] (Command::Params params)
		{
			if (params.Length() < 2) return;
			IFxEffectDef::Dump(Game::DB_FindXAssetHeader(Game::XAssetType::ASSET_TYPE_FX, params[1]).fx);
		});
	}

	IFxEffectDef::~IFxEffectDef()
	{

	}
}
