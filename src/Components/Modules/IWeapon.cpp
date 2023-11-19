#include "STDInclude.hpp"

#define IW4X_GAMEWORLD_VERSION 1

namespace Components
{
	std::unordered_map<Game::IW4::weapAnimFiles_t, Game::IW3::weapAnimFiles_t> weapAnimIW4ToIW3 = {
		  { Game::IW4::WEAP_ANIM_ROOT, Game::IW3::WEAP_ANIM_ROOT },
		  { Game::IW4::WEAP_ANIM_IDLE, Game::IW3::WEAP_ANIM_IDLE },
		  { Game::IW4::WEAP_ANIM_EMPTY_IDLE, Game::IW3::WEAP_ANIM_EMPTY_IDLE },
		  { Game::IW4::WEAP_ANIM_FIRE, Game::IW3::WEAP_ANIM_FIRE },
		  { Game::IW4::WEAP_ANIM_HOLD_FIRE, Game::IW3::WEAP_ANIM_HOLD_FIRE },
		  { Game::IW4::WEAP_ANIM_LASTSHOT, Game::IW3::WEAP_ANIM_LASTSHOT },
		  { Game::IW4::WEAP_ANIM_RECHAMBER, Game::IW3::WEAP_ANIM_RECHAMBER },
		  { Game::IW4::WEAP_ANIM_MELEE, Game::IW3::WEAP_ANIM_MELEE },
		  { Game::IW4::WEAP_ANIM_MELEE_CHARGE, Game::IW3::WEAP_ANIM_MELEE_CHARGE },
		  { Game::IW4::WEAP_ANIM_RELOAD, Game::IW3::WEAP_ANIM_RELOAD },
		  { Game::IW4::WEAP_ANIM_RELOAD_EMPTY, Game::IW3::WEAP_ANIM_RELOAD_EMPTY },
		  { Game::IW4::WEAP_ANIM_RELOAD_START, Game::IW3::WEAP_ANIM_RELOAD_START },
		  { Game::IW4::WEAP_ANIM_RELOAD_END, Game::IW3::WEAP_ANIM_RELOAD_END },
		  { Game::IW4::WEAP_ANIM_RAISE, Game::IW3::WEAP_ANIM_RAISE },
		  { Game::IW4::WEAP_ANIM_FIRST_RAISE, Game::IW3::WEAP_ANIM_FIRST_RAISE },
		  { Game::IW4::WEAP_ANIM_DROP, Game::IW3::WEAP_ANIM_DROP },
		  { Game::IW4::WEAP_ANIM_ALT_RAISE, Game::IW3::WEAP_ANIM_ALT_RAISE },
		  { Game::IW4::WEAP_ANIM_ALT_DROP, Game::IW3::WEAP_ANIM_ALT_DROP },
		  { Game::IW4::WEAP_ANIM_QUICK_RAISE, Game::IW3::WEAP_ANIM_QUICK_RAISE },
		  { Game::IW4::WEAP_ANIM_QUICK_DROP, Game::IW3::WEAP_ANIM_QUICK_DROP },
		  { Game::IW4::WEAP_ANIM_EMPTY_RAISE, Game::IW3::WEAP_ANIM_EMPTY_RAISE },
		  { Game::IW4::WEAP_ANIM_EMPTY_DROP, Game::IW3::WEAP_ANIM_EMPTY_DROP },
		  { Game::IW4::WEAP_ANIM_SPRINT_IN, Game::IW3::WEAP_ANIM_SPRINT_IN },
		  { Game::IW4::WEAP_ANIM_SPRINT_LOOP, Game::IW3::WEAP_ANIM_SPRINT_LOOP },
		  { Game::IW4::WEAP_ANIM_SPRINT_OUT, Game::IW3::WEAP_ANIM_SPRINT_OUT },
		  { Game::IW4::WEAP_ANIM_DETONATE, Game::IW3::WEAP_ANIM_DETONATE },
		  { Game::IW4::WEAP_ANIM_NIGHTVISION_WEAR, Game::IW3::WEAP_ANIM_NIGHTVISION_WEAR },
		  { Game::IW4::WEAP_ANIM_NIGHTVISION_REMOVE, Game::IW3::WEAP_ANIM_NIGHTVISION_REMOVE },
		  { Game::IW4::WEAP_ANIM_ADS_FIRE, Game::IW3::WEAP_ANIM_ADS_FIRE },
		  { Game::IW4::WEAP_ANIM_ADS_LASTSHOT, Game::IW3::WEAP_ANIM_ADS_LASTSHOT },
		  { Game::IW4::WEAP_ANIM_ADS_RECHAMBER, Game::IW3::WEAP_ANIM_ADS_RECHAMBER },
		  { Game::IW4::WEAP_ANIM_ADS_UP, Game::IW3::WEAP_ANIM_ADS_UP },
		  { Game::IW4::WEAP_ANIM_ADS_DOWN, Game::IW3::WEAP_ANIM_ADS_DOWN }
	};

	std::unordered_map<Game::IW3::weapClass_t, Game::IW4::weapClass_t> weapClassIW3ToIW4 =
	{
	  { Game::IW3::WEAPCLASS_RIFLE, Game::IW4::WEAPCLASS_RIFLE },
	  { Game::IW3::WEAPCLASS_MG, Game::IW4::WEAPCLASS_MG },
	  { Game::IW3::WEAPCLASS_SMG, Game::IW4::WEAPCLASS_SMG },
	  { Game::IW3::WEAPCLASS_SPREAD, Game::IW4::WEAPCLASS_SPREAD },
	  { Game::IW3::WEAPCLASS_PISTOL, Game::IW4::WEAPCLASS_PISTOL },
	  { Game::IW3::WEAPCLASS_GRENADE, Game::IW4::WEAPCLASS_GRENADE },
	  { Game::IW3::WEAPCLASS_ROCKETLAUNCHER, Game::IW4::WEAPCLASS_ROCKETLAUNCHER },
	  { Game::IW3::WEAPCLASS_TURRET, Game::IW4::WEAPCLASS_TURRET },
	  { Game::IW3::WEAPCLASS_NON_PLAYER, Game::IW4::WEAPCLASS_NON_PLAYER },
	  { Game::IW3::WEAPCLASS_ITEM, Game::IW4::WEAPCLASS_ITEM }
	};

	std::unordered_map<Game::IW3::ImpactType, Game::IW4::ImpactType> impactTypeIW3ToIW4 =
	{
  { Game::IW3::IMPACT_TYPE_NONE, Game::IW4::IMPACT_TYPE_NONE },
  { Game::IW3::IMPACT_TYPE_BULLET_SMALL, Game::IW4::IMPACT_TYPE_BULLET_SMALL },
  { Game::IW3::IMPACT_TYPE_BULLET_LARGE, Game::IW4::IMPACT_TYPE_BULLET_LARGE },
  { Game::IW3::IMPACT_TYPE_BULLET_AP, Game::IW4::IMPACT_TYPE_BULLET_AP },
  { Game::IW3::IMPACT_TYPE_SHOTGUN, Game::IW4::IMPACT_TYPE_SHOTGUN },
  { Game::IW3::IMPACT_TYPE_GRENADE_BOUNCE, Game::IW4::IMPACT_TYPE_GRENADE_BOUNCE },
  { Game::IW3::IMPACT_TYPE_GRENADE_EXPLODE, Game::IW4::IMPACT_TYPE_GRENADE_EXPLODE },
  { Game::IW3::IMPACT_TYPE_ROCKET_EXPLODE, Game::IW4::IMPACT_TYPE_ROCKET_EXPLODE },
  { Game::IW3::IMPACT_TYPE_PROJECTILE_DUD, Game::IW4::IMPACT_TYPE_PROJECTILE_DUD }
	};

	IWeapon::IWeapon()
	{
		Command::Add("dumpWeapon", [](Command::Params params)
			{
				if (params.Length() < 2) return;

				Utils::Hook::Call<void()>(0x416430)(); // BG_ClearWeaponDef needs to be called to init playeranimtype

				const auto n = params[1];

				if ("*"s == n)
				{
					std::vector<std::string> names{};

					Game::DB_EnumXAssetEntries(Game::IW3::XAssetType::ASSET_TYPE_WEAPON, [&](Game::IW3::XAssetEntryPoolEntry* poolEntry) {
						if (poolEntry)
						{
							auto entry = &poolEntry->entry;
							names.emplace_back(entry->asset.header.weapon->szInternalName);
						}
						}, false);

					for (const auto& name : names)
					{
						Game::IW3::XAssetHeader entry{};

						const auto weaponDef = Utils::Hook::Call<Game::IW3::WeaponDef * (const char*)>(0x41D270)(name.c_str());

						if (weaponDef)
						{
							entry.weapon = weaponDef;
						}
						else
						{
							entry = Game::DB_FindXAssetEntry(Game::IW3::XAssetType::ASSET_TYPE_WEAPON, name.data())->entry.asset.header;
						}

						auto converted = IWeapon::Convert(entry.weapon);
						MapDumper::GetApi()->write(Game::IW4::ASSET_TYPE_WEAPON, converted);

					}

					return;
				}


				auto converted = IWeapon::Convert(Game::DB_FindXAssetHeader(Game::IW3::XAssetType::ASSET_TYPE_WEAPON, params[1]).weapon);
				MapDumper::GetApi()->write(Game::IW4::XAssetType::ASSET_TYPE_WEAPON, converted);
			});
	}

	IWeapon::~IWeapon()
	{

	}

	Game::IW4::WeaponCompleteDef* IWeapon::Convert(Game::IW3::WeaponDef* asset)
	{
		if (!asset) return nullptr;

#define CONVERT_SOUND(target, name)\
if (asset->##name)\
target->##name = AssetHandler::Convert(Game::IW3::ASSET_TYPE_SOUND, { asset->##name }).sound

#define CONVERT_FX(target, name)\
if (asset->##name)\
target->##name = AssetHandler::Convert(Game::IW3::ASSET_TYPE_FX, { asset->##name }).fx

#define CONVERT_MATERIAL(target, name)\
if (asset->##name)\
target->##name = AssetHandler::Convert(Game::IW3::ASSET_TYPE_MATERIAL, { asset->##name }).material

#define CONVERT_XMODEL(target, name)\
if (asset->##name)\
target->##name = AssetHandler::Convert(Game::IW3::ASSET_TYPE_XMODEL, { asset->##name }).model

		Game::IW4::WeaponCompleteDef* iw4WeaponCompleteDef = LocalAllocator.Allocate<Game::IW4::WeaponCompleteDef>();
		iw4WeaponCompleteDef->weapDef = LocalAllocator.Allocate<Game::IW4::WeaponDef>();

		iw4WeaponCompleteDef->szInternalName = LocalAllocator.DuplicateString("iw3_"s + asset->szInternalName);
		iw4WeaponCompleteDef->szDisplayName = asset->szDisplayName;
		iw4WeaponCompleteDef->weapDef->szOverlayName = asset->szOverlayName;

		iw4WeaponCompleteDef->weapDef->gunXModel = LocalAllocator.AllocateArray<Game::IW4::XModel*>(16);
		iw4WeaponCompleteDef->weapDef->worldModel = LocalAllocator.AllocateArray<Game::IW4::XModel*>(16);
		for (size_t i = 0; i < 16; i++)
		{
			iw4WeaponCompleteDef->weapDef->gunXModel[i] = AssetHandler::Convert(Game::IW3::ASSET_TYPE_XMODEL, { asset->gunXModel[i] }).model;
			iw4WeaponCompleteDef->weapDef->worldModel[i] = AssetHandler::Convert(Game::IW3::ASSET_TYPE_XMODEL, { asset->worldModel[i] }).model;
		}

		// hitLocation_t translates very well into iw4
		iw4WeaponCompleteDef->weapDef->locationDamageMultipliers = LocalAllocator.AllocateArray<float>(20);
		std::memcpy(iw4WeaponCompleteDef->weapDef->locationDamageMultipliers, asset->locationDamageMultipliers, 19 * sizeof(float));

		iw4WeaponCompleteDef->weapDef->handXModel = AssetHandler::Convert(Game::IW3::ASSET_TYPE_XMODEL, { asset->handXModel }).model;

		iw4WeaponCompleteDef->szXAnims = LocalAllocator.AllocateArray<const char*>(Game::IW4::weapAnimFiles_t::NUM_WEAP_ANIMS);
		iw4WeaponCompleteDef->weapDef->szXAnimsLeftHanded = LocalAllocator.AllocateArray<const char*>(Game::IW4::weapAnimFiles_t::NUM_WEAP_ANIMS);
		iw4WeaponCompleteDef->weapDef->szXAnimsRightHanded = LocalAllocator.AllocateArray<const char*>(Game::IW4::weapAnimFiles_t::NUM_WEAP_ANIMS);

		const auto empty = LocalAllocator.DuplicateString("");

		for (size_t i = 0; i < Game::IW4::weapAnimFiles_t::NUM_WEAP_ANIMS; i++)
		{
			iw4WeaponCompleteDef->szXAnims[i] = asset->szXAnims[weapAnimIW4ToIW3[static_cast<Game::IW4::weapAnimFiles_t>(i)]];

			if (i > 0)
			{
				if (iw4WeaponCompleteDef->szXAnims[i] == nullptr)
				{
					iw4WeaponCompleteDef->szXAnims[i] = empty;
				}

				iw4WeaponCompleteDef->weapDef->szXAnimsLeftHanded[i] = empty;
				iw4WeaponCompleteDef->weapDef->szXAnimsRightHanded[i] = empty;
			}
		}

		iw4WeaponCompleteDef->weapDef->szModeName = asset->szModeName;

		iw4WeaponCompleteDef->hideTags = LocalAllocator.AllocateArray<unsigned short>(32);
		std::memcpy(iw4WeaponCompleteDef->hideTags, asset->hideTags, 8 * sizeof(unsigned short));

		iw4WeaponCompleteDef->weapDef->notetrackSoundMapKeys = LocalAllocator.AllocateArray<unsigned short>(16);
		iw4WeaponCompleteDef->weapDef->notetrackSoundMapValues = LocalAllocator.AllocateArray<unsigned short>(16);
		iw4WeaponCompleteDef->weapDef->notetrackRumbleMapKeys = LocalAllocator.AllocateArray<unsigned short>(16);
		iw4WeaponCompleteDef->weapDef->notetrackRumbleMapValues = LocalAllocator.AllocateArray<unsigned short>(16);
		std::memcpy(iw4WeaponCompleteDef->weapDef->notetrackSoundMapKeys, asset->notetrackSoundMapKeys, 16 * sizeof(unsigned short));
		std::memcpy(iw4WeaponCompleteDef->weapDef->notetrackSoundMapValues, asset->notetrackSoundMapValues, 16 * sizeof(unsigned short));

		iw4WeaponCompleteDef->weapDef->weapType = static_cast<Game::IW4::weapType_t>(asset->weapType); // Bite me
		iw4WeaponCompleteDef->weapDef->weapClass = weapClassIW3ToIW4[asset->weapClass];
		iw4WeaponCompleteDef->weapDef->penetrateType = asset->penetrateType;
		iw4WeaponCompleteDef->impactType = impactTypeIW3ToIW4[asset->impactType];
		iw4WeaponCompleteDef->weapDef->fireType = static_cast<Game::IW4::weapFireType_t>(asset->fireType); // aligned
		iw4WeaponCompleteDef->weapDef->offhandClass = static_cast<Game::IW4::OffhandClass>(asset->offhandClass); // aligned
		iw4WeaponCompleteDef->weapDef->stance = asset->stance;

		CONVERT_FX(iw4WeaponCompleteDef->weapDef, viewFlashEffect);
		CONVERT_FX(iw4WeaponCompleteDef->weapDef, worldFlashEffect);

		CONVERT_SOUND(iw4WeaponCompleteDef->weapDef, pickupSound);
		CONVERT_SOUND(iw4WeaponCompleteDef->weapDef, pickupSoundPlayer);
		CONVERT_SOUND(iw4WeaponCompleteDef->weapDef, ammoPickupSound);
		CONVERT_SOUND(iw4WeaponCompleteDef->weapDef, ammoPickupSoundPlayer);
		CONVERT_SOUND(iw4WeaponCompleteDef->weapDef, projectileSound);
		CONVERT_SOUND(iw4WeaponCompleteDef->weapDef, pullbackSound);
		CONVERT_SOUND(iw4WeaponCompleteDef->weapDef, pullbackSoundPlayer);
		CONVERT_SOUND(iw4WeaponCompleteDef->weapDef, fireSound);
		CONVERT_SOUND(iw4WeaponCompleteDef->weapDef, fireSoundPlayer);
		CONVERT_SOUND(iw4WeaponCompleteDef->weapDef, fireLoopSound);
		CONVERT_SOUND(iw4WeaponCompleteDef->weapDef, fireLoopSoundPlayer);
		CONVERT_SOUND(iw4WeaponCompleteDef->weapDef, fireStopSound);
		CONVERT_SOUND(iw4WeaponCompleteDef->weapDef, fireStopSoundPlayer);
		CONVERT_SOUND(iw4WeaponCompleteDef->weapDef, fireLastSound);
		CONVERT_SOUND(iw4WeaponCompleteDef->weapDef, fireLastSoundPlayer);
		CONVERT_SOUND(iw4WeaponCompleteDef->weapDef, emptyFireSound);
		CONVERT_SOUND(iw4WeaponCompleteDef->weapDef, emptyFireSoundPlayer);
		CONVERT_SOUND(iw4WeaponCompleteDef->weapDef, meleeSwipeSound);
		CONVERT_SOUND(iw4WeaponCompleteDef->weapDef, meleeSwipeSoundPlayer);
		CONVERT_SOUND(iw4WeaponCompleteDef->weapDef, meleeHitSound);
		CONVERT_SOUND(iw4WeaponCompleteDef->weapDef, meleeMissSound);
		CONVERT_SOUND(iw4WeaponCompleteDef->weapDef, rechamberSound);
		CONVERT_SOUND(iw4WeaponCompleteDef->weapDef, rechamberSoundPlayer);
		CONVERT_SOUND(iw4WeaponCompleteDef->weapDef, reloadSound);
		CONVERT_SOUND(iw4WeaponCompleteDef->weapDef, reloadSoundPlayer);
		CONVERT_SOUND(iw4WeaponCompleteDef->weapDef, reloadEmptySound);
		CONVERT_SOUND(iw4WeaponCompleteDef->weapDef, reloadEmptySoundPlayer);
		CONVERT_SOUND(iw4WeaponCompleteDef->weapDef, reloadStartSound);
		CONVERT_SOUND(iw4WeaponCompleteDef->weapDef, reloadStartSoundPlayer);
		CONVERT_SOUND(iw4WeaponCompleteDef->weapDef, reloadEndSound);
		CONVERT_SOUND(iw4WeaponCompleteDef->weapDef, reloadEndSoundPlayer);
		CONVERT_SOUND(iw4WeaponCompleteDef->weapDef, detonateSound);
		CONVERT_SOUND(iw4WeaponCompleteDef->weapDef, detonateSoundPlayer);
		CONVERT_SOUND(iw4WeaponCompleteDef->weapDef, nightVisionWearSound);
		CONVERT_SOUND(iw4WeaponCompleteDef->weapDef, nightVisionWearSoundPlayer);
		CONVERT_SOUND(iw4WeaponCompleteDef->weapDef, nightVisionRemoveSound);
		CONVERT_SOUND(iw4WeaponCompleteDef->weapDef, nightVisionRemoveSoundPlayer);
		CONVERT_SOUND(iw4WeaponCompleteDef->weapDef, altSwitchSound);
		CONVERT_SOUND(iw4WeaponCompleteDef->weapDef, altSwitchSoundPlayer);
		CONVERT_SOUND(iw4WeaponCompleteDef->weapDef, raiseSound);
		CONVERT_SOUND(iw4WeaponCompleteDef->weapDef, raiseSoundPlayer);
		CONVERT_SOUND(iw4WeaponCompleteDef->weapDef, firstRaiseSound);
		CONVERT_SOUND(iw4WeaponCompleteDef->weapDef, firstRaiseSoundPlayer);
		CONVERT_SOUND(iw4WeaponCompleteDef->weapDef, putawaySound);
		CONVERT_SOUND(iw4WeaponCompleteDef->weapDef, putawaySoundPlayer);

		// TODO Bounce sounds

		CONVERT_FX(iw4WeaponCompleteDef->weapDef, viewShellEjectEffect);
		CONVERT_FX(iw4WeaponCompleteDef->weapDef, worldShellEjectEffect);
		CONVERT_FX(iw4WeaponCompleteDef->weapDef, viewLastShotEjectEffect);
		CONVERT_FX(iw4WeaponCompleteDef->weapDef, worldLastShotEjectEffect);

		CONVERT_MATERIAL(iw4WeaponCompleteDef->weapDef, reticleCenter);
		CONVERT_MATERIAL(iw4WeaponCompleteDef->weapDef, reticleSide);

#define COPY_RANGE(targetMember, originEnd)\
		std::memcpy(&iw4WeaponCompleteDef->weapDef->##targetMember, &asset->##targetMember, reinterpret_cast<int>(&asset->##originEnd) - reinterpret_cast<int>(&asset->##targetMember))

		COPY_RANGE(iReticleCenterSize, vDuckedOfs);
		COPY_RANGE(vDuckedOfs, worldModel);

		// World model has been taken care of already

		CONVERT_XMODEL(iw4WeaponCompleteDef->weapDef, worldClipModel);
		CONVERT_XMODEL(iw4WeaponCompleteDef->weapDef, rocketModel);
		CONVERT_XMODEL(iw4WeaponCompleteDef->weapDef, knifeModel);
		CONVERT_XMODEL(iw4WeaponCompleteDef->weapDef, worldKnifeModel);
		CONVERT_MATERIAL(iw4WeaponCompleteDef->weapDef, hudIcon);

		iw4WeaponCompleteDef->weapDef->hudIconRatio = asset->hudIconRatio;

		CONVERT_MATERIAL(iw4WeaponCompleteDef->weapDef, ammoCounterIcon);

		iw4WeaponCompleteDef->weapDef->ammoCounterIconRatio = asset->ammoCounterIconRatio;
		iw4WeaponCompleteDef->weapDef->ammoCounterClip = asset->ammoCounterClip;

		COPY_RANGE(iStartAmmo, iClipSize);

		iw4WeaponCompleteDef->iClipSize = asset->iClipSize;

		COPY_RANGE(shotCount, iFireTime);
		iw4WeaponCompleteDef->iFireTime = asset->iFireTime;
		iw4WeaponCompleteDef->weapDef->iRechamberTime = asset->iRechamberTime;

		COPY_RANGE(iRechamberBoltTime, iAltRaiseTime);
		COPY_RANGE(quickDropTime, nightVisionWearTime);

		iw4WeaponCompleteDef->weapDef->nightVisionWearTime = asset->nightVisionWearTime;
		iw4WeaponCompleteDef->weapDef->nightVisionWearTimeFadeOutEnd = asset->nightVisionWearTimeFadeOutEnd;
		iw4WeaponCompleteDef->weapDef->nightVisionWearTimePowerUp = asset->nightVisionWearTimePowerUp;
		iw4WeaponCompleteDef->weapDef->nightVisionRemoveTime = asset->nightVisionRemoveTime;
		iw4WeaponCompleteDef->weapDef->nightVisionRemoveTimePowerDown = asset->nightVisionRemoveTimePowerDown;
		iw4WeaponCompleteDef->weapDef->nightVisionRemoveTimeFadeInStart = asset->nightVisionRemoveTimeFadeInStart;
		iw4WeaponCompleteDef->weapDef->fuseTime = asset->fuseTime;
		iw4WeaponCompleteDef->weapDef->aiFuseTime = asset->aiFuseTime;
		iw4WeaponCompleteDef->weapDef->requireLockonToFire = asset->requireLockonToFire;
		iw4WeaponCompleteDef->weapDef->noAdsWhenMagEmpty = asset->noAdsWhenMagEmpty;
		iw4WeaponCompleteDef->weapDef->avoidDropCleanup = asset->avoidDropCleanup;
		iw4WeaponCompleteDef->weapDef->autoAimRange = asset->autoAimRange;
		iw4WeaponCompleteDef->weapDef->aimAssistRange = asset->aimAssistRange;
		iw4WeaponCompleteDef->weapDef->aimAssistRangeAds = asset->aimAssistRangeAds;
		iw4WeaponCompleteDef->weapDef->aimPadding = asset->aimPadding;
		iw4WeaponCompleteDef->weapDef->enemyCrosshairRange = asset->enemyCrosshairRange;
		iw4WeaponCompleteDef->weapDef->crosshairColorChange = asset->crosshairColorChange;
		iw4WeaponCompleteDef->weapDef->moveSpeedScale = asset->moveSpeedScale;
		iw4WeaponCompleteDef->weapDef->adsMoveSpeedScale = asset->adsMoveSpeedScale;
		iw4WeaponCompleteDef->weapDef->sprintDurationScale = asset->sprintDurationScale;
		iw4WeaponCompleteDef->fAdsZoomFov = asset->fAdsZoomFov;
		iw4WeaponCompleteDef->weapDef->fAdsZoomInFrac = asset->fAdsZoomInFrac;
		iw4WeaponCompleteDef->weapDef->fAdsZoomOutFrac = asset->fAdsZoomOutFrac;

		CONVERT_MATERIAL(iw4WeaponCompleteDef->weapDef, overlayMaterial);
		CONVERT_MATERIAL(iw4WeaponCompleteDef->weapDef, overlayMaterialLowRes);

		iw4WeaponCompleteDef->weapDef->overlayReticle = asset->overlayReticle;
		iw4WeaponCompleteDef->weapDef->overlayInterface = asset->overlayInterface;
		iw4WeaponCompleteDef->weapDef->overlayWidth = asset->overlayWidth;
		iw4WeaponCompleteDef->weapDef->overlayHeight = asset->overlayHeight;
		iw4WeaponCompleteDef->weapDef->fAdsBobFactor = asset->fAdsBobFactor;
		iw4WeaponCompleteDef->weapDef->fAdsViewBobMult = asset->fAdsViewBobMult;
		iw4WeaponCompleteDef->weapDef->fHipSpreadStandMin = asset->fHipSpreadStandMin;
		iw4WeaponCompleteDef->weapDef->fHipSpreadDuckedMin = asset->fHipSpreadDuckedMin;
		iw4WeaponCompleteDef->weapDef->fHipSpreadProneMin = asset->fHipSpreadProneMin;
		iw4WeaponCompleteDef->weapDef->hipSpreadStandMax = asset->hipSpreadStandMax;
		iw4WeaponCompleteDef->weapDef->hipSpreadDuckedMax = asset->hipSpreadDuckedMax;
		iw4WeaponCompleteDef->weapDef->hipSpreadProneMax = asset->hipSpreadProneMax;
		iw4WeaponCompleteDef->weapDef->fHipSpreadDecayRate = asset->fHipSpreadDecayRate;
		iw4WeaponCompleteDef->weapDef->fHipSpreadFireAdd = asset->fHipSpreadFireAdd;
		iw4WeaponCompleteDef->weapDef->fHipSpreadTurnAdd = asset->fHipSpreadTurnAdd;
		iw4WeaponCompleteDef->weapDef->fHipSpreadMoveAdd = asset->fHipSpreadMoveAdd;
		iw4WeaponCompleteDef->weapDef->fHipSpreadDuckedDecay = asset->fHipSpreadDuckedDecay;
		iw4WeaponCompleteDef->weapDef->fHipSpreadProneDecay = asset->fHipSpreadProneDecay;
		iw4WeaponCompleteDef->weapDef->fHipReticleSidePos = asset->fHipReticleSidePos;
		iw4WeaponCompleteDef->iAdsTransInTime = asset->iAdsTransInTime;
		iw4WeaponCompleteDef->iAdsTransOutTime = asset->iAdsTransOutTime;
		iw4WeaponCompleteDef->weapDef->fAdsIdleAmount = asset->fAdsIdleAmount;
		iw4WeaponCompleteDef->weapDef->fHipIdleAmount = asset->fHipIdleAmount;
		iw4WeaponCompleteDef->weapDef->adsIdleSpeed = asset->adsIdleSpeed;
		iw4WeaponCompleteDef->weapDef->hipIdleSpeed = asset->hipIdleSpeed;
		iw4WeaponCompleteDef->weapDef->fIdleCrouchFactor = asset->fIdleCrouchFactor;
		iw4WeaponCompleteDef->weapDef->fIdleProneFactor = asset->fIdleProneFactor;
		iw4WeaponCompleteDef->weapDef->fGunMaxPitch = asset->fGunMaxPitch;
		iw4WeaponCompleteDef->weapDef->fGunMaxYaw = asset->fGunMaxYaw;
		iw4WeaponCompleteDef->weapDef->swayMaxAngle = asset->swayMaxAngle;
		iw4WeaponCompleteDef->weapDef->swayLerpSpeed = asset->swayLerpSpeed;
		iw4WeaponCompleteDef->weapDef->swayPitchScale = asset->swayPitchScale;
		iw4WeaponCompleteDef->weapDef->swayYawScale = asset->swayYawScale;
		iw4WeaponCompleteDef->weapDef->swayHorizScale = asset->swayHorizScale;
		iw4WeaponCompleteDef->weapDef->swayVertScale = asset->swayVertScale;
		iw4WeaponCompleteDef->weapDef->swayShellShockScale = asset->swayShellShockScale;
		iw4WeaponCompleteDef->weapDef->adsSwayMaxAngle = asset->adsSwayMaxAngle;
		iw4WeaponCompleteDef->weapDef->adsSwayLerpSpeed = asset->adsSwayLerpSpeed;
		iw4WeaponCompleteDef->weapDef->adsSwayPitchScale = asset->adsSwayPitchScale;
		iw4WeaponCompleteDef->weapDef->adsSwayYawScale = asset->adsSwayYawScale;
		iw4WeaponCompleteDef->weapDef->adsSwayHorizScale = asset->adsSwayHorizScale;
		iw4WeaponCompleteDef->weapDef->adsSwayVertScale = asset->adsSwayVertScale;
		iw4WeaponCompleteDef->weapDef->bRifleBullet = asset->bRifleBullet;
		iw4WeaponCompleteDef->weapDef->armorPiercing = asset->armorPiercing;
		iw4WeaponCompleteDef->weapDef->bBoltAction = asset->bBoltAction;
		iw4WeaponCompleteDef->weapDef->aimDownSight = asset->aimDownSight;
		iw4WeaponCompleteDef->weapDef->bRechamberWhileAds = asset->bRechamberWhileAds;
		iw4WeaponCompleteDef->weapDef->adsViewErrorMin = asset->adsViewErrorMin;
		iw4WeaponCompleteDef->weapDef->adsViewErrorMax = asset->adsViewErrorMax;
		iw4WeaponCompleteDef->weapDef->bCookOffHold = asset->bCookOffHold;
		iw4WeaponCompleteDef->weapDef->bClipOnly = asset->bClipOnly;
		iw4WeaponCompleteDef->weapDef->adsFireOnly = asset->adsFireOnly;
		iw4WeaponCompleteDef->weapDef->cancelAutoHolsterWhenEmpty = asset->cancelAutoHolsterWhenEmpty;
		iw4WeaponCompleteDef->weapDef->suppressAmmoReserveDisplay = asset->suppressAmmoReserveDisplay;
		iw4WeaponCompleteDef->enhanced = asset->enhanced;
		iw4WeaponCompleteDef->weapDef->laserSightDuringNightvision = asset->laserSightDuringNightvision;

		iw4WeaponCompleteDef->killIcon = AssetHandler::Convert(Game::IW3::ASSET_TYPE_MATERIAL, { asset->killIcon }).material;
		iw4WeaponCompleteDef->weapDef->killIconRatio = asset->killIconRatio;
		iw4WeaponCompleteDef->weapDef->flipKillIcon = asset->flipKillIcon;
		iw4WeaponCompleteDef->dpadIcon = AssetHandler::Convert(Game::IW3::ASSET_TYPE_MATERIAL, { asset->dpadIcon }).material;

		iw4WeaponCompleteDef->dpadIconRatio = asset->dpadIconRatio;
		iw4WeaponCompleteDef->weapDef->bNoPartialReload = asset->bNoPartialReload;
		iw4WeaponCompleteDef->weapDef->bSegmentedReload = asset->bSegmentedReload;
		iw4WeaponCompleteDef->weapDef->iReloadAmmoAdd = asset->iReloadAmmoAdd;
		iw4WeaponCompleteDef->weapDef->iReloadStartAdd = asset->iReloadStartAdd;

		if (asset->szAltWeaponName && strnlen(asset->szAltWeaponName, 1) > 0)
		{
			iw4WeaponCompleteDef->szAltWeaponName = LocalAllocator.DuplicateString("iw3_"s + asset->szAltWeaponName);
		}

		iw4WeaponCompleteDef->altWeaponIndex = asset->altWeaponIndex;
		iw4WeaponCompleteDef->weapDef->blocksProne = asset->blocksProne;
		iw4WeaponCompleteDef->weapDef->silenced = asset->silenced;
		iw4WeaponCompleteDef->weapDef->iExplosionRadius = asset->iExplosionRadius;
		iw4WeaponCompleteDef->weapDef->iExplosionRadiusMin = asset->iExplosionRadiusMin;
		iw4WeaponCompleteDef->weapDef->iExplosionInnerDamage = asset->iExplosionInnerDamage;
		iw4WeaponCompleteDef->weapDef->iExplosionOuterDamage = asset->iExplosionOuterDamage;
		iw4WeaponCompleteDef->weapDef->damageConeAngle = asset->damageConeAngle;
		iw4WeaponCompleteDef->weapDef->iProjectileSpeed = asset->iProjectileSpeed;
		iw4WeaponCompleteDef->weapDef->iProjectileSpeedUp = asset->iProjectileSpeedUp;
		iw4WeaponCompleteDef->weapDef->iProjectileSpeedForward = asset->iProjectileSpeedForward;
		iw4WeaponCompleteDef->weapDef->iProjectileActivateDist = asset->iProjectileActivateDist;
		iw4WeaponCompleteDef->weapDef->projLifetime = asset->projLifetime;
		iw4WeaponCompleteDef->weapDef->timeToAccelerate = asset->timeToAccelerate;
		iw4WeaponCompleteDef->weapDef->projectileCurvature = asset->projectileCurvature;

		CONVERT_XMODEL(iw4WeaponCompleteDef->weapDef, projectileModel);
		iw4WeaponCompleteDef->weapDef->projExplosion = asset->projExplosion;
		iw4WeaponCompleteDef->weapDef->projExplosionEffectForceNormalUp = asset->projExplosionEffectForceNormalUp;
		CONVERT_FX(iw4WeaponCompleteDef->weapDef, projExplosionEffect);
		CONVERT_FX(iw4WeaponCompleteDef->weapDef, projDudEffect);
		CONVERT_SOUND(iw4WeaponCompleteDef->weapDef, projDudSound);
		CONVERT_SOUND(iw4WeaponCompleteDef->weapDef, projExplosionSound);

		iw4WeaponCompleteDef->weapDef->bProjImpactExplode = asset->bProjImpactExplode;
		//iw4WeaponCompleteDef->weapDef->stickiness = asset->stickiness; // TODO
		iw4WeaponCompleteDef->weapDef->hasDetonator = asset->hasDetonator;
		iw4WeaponCompleteDef->weapDef->timedDetonation = asset->timedDetonation;
		iw4WeaponCompleteDef->weapDef->rotate = asset->rotate;
		iw4WeaponCompleteDef->weapDef->holdButtonToThrow = asset->holdButtonToThrow;
		iw4WeaponCompleteDef->weapDef->freezeMovementWhenFiring = asset->freezeMovementWhenFiring;
		iw4WeaponCompleteDef->weapDef->lowAmmoWarningThreshold = asset->lowAmmoWarningThreshold;

		iw4WeaponCompleteDef->weapDef->parallelBounce = LocalAllocator.AllocateArray<float>(31);
		std::memcpy(iw4WeaponCompleteDef->weapDef->parallelBounce, asset->parallelBounce, 29 * sizeof(float));

		iw4WeaponCompleteDef->weapDef->perpendicularBounce = LocalAllocator.AllocateArray<float>(31);
		std::memcpy(iw4WeaponCompleteDef->weapDef->perpendicularBounce, asset->perpendicularBounce, 29 * sizeof(float));
		CONVERT_FX(iw4WeaponCompleteDef->weapDef, projTrailEffect);

		COPY_RANGE(vProjectileColor, projIgnitionEffect);

		CONVERT_FX(iw4WeaponCompleteDef->weapDef, projIgnitionEffect);
		CONVERT_SOUND(iw4WeaponCompleteDef->weapDef, projIgnitionSound);

		iw4WeaponCompleteDef->weapDef->fAdsAimPitch = asset->fAdsAimPitch;
		iw4WeaponCompleteDef->weapDef->fAdsCrosshairInFrac = asset->fAdsCrosshairInFrac;
		iw4WeaponCompleteDef->weapDef->fAdsCrosshairOutFrac = asset->fAdsCrosshairOutFrac;
		iw4WeaponCompleteDef->weapDef->adsGunKickReducedKickBullets = asset->adsGunKickReducedKickBullets;
		iw4WeaponCompleteDef->weapDef->adsGunKickReducedKickPercent = asset->adsGunKickReducedKickPercent;
		iw4WeaponCompleteDef->weapDef->fAdsGunKickPitchMin = asset->fAdsGunKickPitchMin;
		iw4WeaponCompleteDef->weapDef->fAdsGunKickPitchMax = asset->fAdsGunKickPitchMax;
		iw4WeaponCompleteDef->weapDef->fAdsGunKickYawMin = asset->fAdsGunKickYawMin;
		iw4WeaponCompleteDef->weapDef->fAdsGunKickYawMax = asset->fAdsGunKickYawMax;
		iw4WeaponCompleteDef->weapDef->fAdsGunKickAccel = asset->fAdsGunKickAccel;
		iw4WeaponCompleteDef->weapDef->fAdsGunKickSpeedMax = asset->fAdsGunKickSpeedMax;
		iw4WeaponCompleteDef->weapDef->fAdsGunKickSpeedDecay = asset->fAdsGunKickSpeedDecay;
		iw4WeaponCompleteDef->weapDef->fAdsGunKickStaticDecay = asset->fAdsGunKickStaticDecay;
		iw4WeaponCompleteDef->weapDef->fAdsViewKickPitchMin = asset->fAdsViewKickPitchMin;
		iw4WeaponCompleteDef->weapDef->fAdsViewKickPitchMax = asset->fAdsViewKickPitchMax;
		iw4WeaponCompleteDef->weapDef->fAdsViewKickYawMin = asset->fAdsViewKickYawMin;
		iw4WeaponCompleteDef->weapDef->fAdsViewKickYawMax = asset->fAdsViewKickYawMax;
		iw4WeaponCompleteDef->fAdsViewKickCenterSpeed = asset->fAdsViewKickCenterSpeed;
		iw4WeaponCompleteDef->weapDef->fAdsViewScatterMin = asset->fAdsViewScatterMin;
		iw4WeaponCompleteDef->weapDef->fAdsViewScatterMax = asset->fAdsViewScatterMax;
		iw4WeaponCompleteDef->weapDef->fAdsSpread = asset->fAdsSpread;
		iw4WeaponCompleteDef->weapDef->hipGunKickReducedKickBullets = asset->hipGunKickReducedKickBullets;
		iw4WeaponCompleteDef->weapDef->hipGunKickReducedKickPercent = asset->hipGunKickReducedKickPercent;
		iw4WeaponCompleteDef->weapDef->fHipGunKickPitchMin = asset->fHipGunKickPitchMin;
		iw4WeaponCompleteDef->weapDef->fHipGunKickPitchMax = asset->fHipGunKickPitchMax;
		iw4WeaponCompleteDef->weapDef->fHipGunKickYawMin = asset->fHipGunKickYawMin;
		iw4WeaponCompleteDef->weapDef->fHipGunKickYawMax = asset->fHipGunKickYawMax;
		iw4WeaponCompleteDef->weapDef->fHipGunKickAccel = asset->fHipGunKickAccel;
		iw4WeaponCompleteDef->weapDef->fHipGunKickSpeedMax = asset->fHipGunKickSpeedMax;
		iw4WeaponCompleteDef->weapDef->fHipGunKickSpeedDecay = asset->fHipGunKickSpeedDecay;
		iw4WeaponCompleteDef->weapDef->fHipGunKickStaticDecay = asset->fHipGunKickStaticDecay;
		iw4WeaponCompleteDef->weapDef->fHipViewKickPitchMin = asset->fHipViewKickPitchMin;
		iw4WeaponCompleteDef->weapDef->fHipViewKickPitchMax = asset->fHipViewKickPitchMax;
		iw4WeaponCompleteDef->weapDef->fHipViewKickYawMin = asset->fHipViewKickYawMin;
		iw4WeaponCompleteDef->weapDef->fHipViewKickYawMax = asset->fHipViewKickYawMax;
		iw4WeaponCompleteDef->fHipViewKickCenterSpeed = asset->fHipViewKickCenterSpeed;
		iw4WeaponCompleteDef->weapDef->fHipViewScatterMin = asset->fHipViewScatterMin;
		iw4WeaponCompleteDef->weapDef->fHipViewScatterMax = asset->fHipViewScatterMax;
		iw4WeaponCompleteDef->weapDef->fightDist = asset->fightDist;
		iw4WeaponCompleteDef->weapDef->maxDist = asset->maxDist;

		// Accuracy graphs? No !
		// TODO, if ever

		iw4WeaponCompleteDef->weapDef->iPositionReloadTransTime = asset->iPositionReloadTransTime;
		iw4WeaponCompleteDef->weapDef->leftArc = asset->leftArc;
		iw4WeaponCompleteDef->weapDef->rightArc = asset->rightArc;
		iw4WeaponCompleteDef->weapDef->topArc = asset->topArc;
		iw4WeaponCompleteDef->weapDef->bottomArc = asset->bottomArc;
		iw4WeaponCompleteDef->weapDef->accuracy = asset->accuracy;
		iw4WeaponCompleteDef->weapDef->aiSpread = asset->aiSpread;
		iw4WeaponCompleteDef->weapDef->playerSpread = asset->playerSpread;

		std::memcpy(iw4WeaponCompleteDef->weapDef->minTurnSpeed, asset->minTurnSpeed, 2 * sizeof(float));
		std::memcpy(iw4WeaponCompleteDef->weapDef->maxTurnSpeed, asset->maxTurnSpeed, 2 * sizeof(float));

		iw4WeaponCompleteDef->weapDef->pitchConvergenceTime = asset->pitchConvergenceTime;
		iw4WeaponCompleteDef->weapDef->yawConvergenceTime = asset->yawConvergenceTime;
		iw4WeaponCompleteDef->weapDef->suppressTime = asset->suppressTime;
		iw4WeaponCompleteDef->weapDef->maxRange = asset->maxRange;
		iw4WeaponCompleteDef->weapDef->fAnimHorRotateInc = asset->fAnimHorRotateInc;
		iw4WeaponCompleteDef->weapDef->fPlayerPositionDist = asset->fPlayerPositionDist;
		iw4WeaponCompleteDef->weapDef->szUseHintString = asset->szUseHintString;
		iw4WeaponCompleteDef->weapDef->dropHintString = asset->dropHintString;
		iw4WeaponCompleteDef->weapDef->iUseHintStringIndex = asset->iUseHintStringIndex;
		iw4WeaponCompleteDef->weapDef->dropHintStringIndex = asset->dropHintStringIndex;
		iw4WeaponCompleteDef->weapDef->horizViewJitter = asset->horizViewJitter;
		iw4WeaponCompleteDef->weapDef->vertViewJitter = asset->vertViewJitter;
		iw4WeaponCompleteDef->weapDef->szScript = asset->szScript;
		std::memcpy(iw4WeaponCompleteDef->weapDef->fOOPosAnimLength, asset->fOOPosAnimLength, 2 * sizeof(float));
		iw4WeaponCompleteDef->weapDef->minDamage = asset->minDamage;
		iw4WeaponCompleteDef->weapDef->minPlayerDamage = asset->minPlayerDamage;
		iw4WeaponCompleteDef->weapDef->fMaxDamageRange = asset->fMaxDamageRange;
		iw4WeaponCompleteDef->weapDef->fMinDamageRange = asset->fMinDamageRange;
		iw4WeaponCompleteDef->weapDef->destabilizationRateTime = asset->destabilizationRateTime;
		iw4WeaponCompleteDef->weapDef->destabilizationCurvatureMax = asset->destabilizationCurvatureMax;
		iw4WeaponCompleteDef->weapDef->destabilizeDistance = asset->destabilizeDistance;

		// locationDamageMultipliers taken care of above

		iw4WeaponCompleteDef->weapDef->fireRumble = asset->fireRumble;
		iw4WeaponCompleteDef->weapDef->meleeImpactRumble = asset->meleeImpactRumble;
		iw4WeaponCompleteDef->adsDofStart = asset->adsDofStart;
		iw4WeaponCompleteDef->adsDofEnd = asset->adsDofEnd;

		iw4WeaponCompleteDef->penetrateMultiplier = 1.0f;
		iw4WeaponCompleteDef->iAltRaiseTime = asset->iAltRaiseTime;

		iw4WeaponCompleteDef->fireAnimLength = 170; // ?? Probably wrong, taken from M4_MP from IW4
		iw4WeaponCompleteDef->iFirstRaiseTime = asset->iFirstRaiseTime;
		iw4WeaponCompleteDef->ammoDropStockMax = asset->iDropAmmoMax;

		iw4WeaponCompleteDef->weapDef->playerAnimType = asset->playerAnimType;

		iw4WeaponCompleteDef->weapDef->stunnedTimeBegin = 500;
		iw4WeaponCompleteDef->weapDef->stunnedTimeLoop = 500;
		iw4WeaponCompleteDef->weapDef->stunnedTimeEnd = 500;
		iw4WeaponCompleteDef->weapDef->autoAimRange = 1000;

		iw4WeaponCompleteDef->weapDef->szSharedAmmoCapName = asset->szSharedAmmoCapName;
		
		if (iw4WeaponCompleteDef->weapDef->szSharedAmmoCapName && *iw4WeaponCompleteDef->weapDef->szSharedAmmoCapName)
		{
			iw4WeaponCompleteDef->weapDef->szSharedAmmoCapName = LocalAllocator.DuplicateString("IW3_"s + asset->szSharedAmmoCapName);
		}

		// Generate tracer
		iw4WeaponCompleteDef->weapDef->tracerType = GenerateTracerDef(asset);

		// Rename materials to prevent IW4 overshadowing
		DifferentiateWeapon(iw4WeaponCompleteDef);

		return iw4WeaponCompleteDef;
	}
	Game::IW4::TracerDef* IWeapon::GenerateTracerDef(Game::IW3::WeaponDef* weapon)
	{
		Game::IW3::Material* tracerMaterial = *reinterpret_cast<Game::IW3::Material**>(0x84CB44);

		if (!tracerMaterial)
		{
			// CGMedia not initialized
			tracerMaterial = Game::DB_FindXAssetEntry(Game::IW3::ASSET_TYPE_MATERIAL, "gfx_tracer")->entry.asset.header.material;
		}

		if (tracerMaterial)
		{
			Game::IW4::TracerDef* tracer = LocalAllocator.Allocate<Game::IW4::TracerDef>();
			tracer->material = AssetHandler::Convert(Game::IW3::ASSET_TYPE_MATERIAL, { tracerMaterial }).material;

			switch (weapon->weapClass)
			{
			default:
			{
				tracer->name = LocalAllocator.DuplicateString("iw3x_tracer_default");
				tracer->drawInterval = 4;
				tracer->speed = 7500.f;
				tracer->beamLength = 160.0f;
				tracer->beamWidth = 4.0f;
				tracer->screwRadius = 0.5f;
				tracer->screwDist = 100.0f;

				const float colors[5][4] = {
					{ 1.f, 1.f, 1.f, 1.f },
					{ 1.f, 1.f, 1.f, 1.f },
					{ 1.f, 1.f, 1.f, 1.f },
					{ 1.f, 1.f, 1.f, 1.f },
					{ 1.f, 1.f, 1.f, 1.f }
				};

				std::memcpy(tracer->colors, colors, 5 * 4 * sizeof(float));
			}
			break;


			case Game::IW3::WEAPCLASS_SMG:
			{
				tracer->name = LocalAllocator.DuplicateString("iw3x_tracer_smg");
				tracer->drawInterval = 2;
				tracer->speed = 4000.f;
				tracer->beamLength = 60.0f;
				tracer->beamWidth = 7.0f;
				tracer->screwRadius = 0.2f;
				tracer->screwDist = 100.0f;

				const float colors[5][4] = {
					{ 1.f, 0.9f, 0.9f, 0.f },
					{ 1.f, 0.9f, 0.9f, 1.f },
					{ 1.f, 0.9f, 0.9f, 1.f },
					{ 1.f, 1.f, 1.f, 0.75f },
					{ 1.f, 1.f, 1.f, 1.f }
				};

				std::memcpy(tracer->colors, colors, 5 * 4 * sizeof(float));
			}
			break;

			case Game::IW3::WEAPCLASS_PISTOL:
			{
				tracer->name = LocalAllocator.DuplicateString("iw3x_tracer_pistol");
				tracer->drawInterval = 1;
				tracer->speed = 4000.f;
				tracer->beamLength = 60.0f;
				tracer->beamWidth = 6.0f;
				tracer->screwRadius = 0.2f;
				tracer->screwDist = 100.0f;

				const float colors[5][4] = {
					{ 1.f, 0.9f, 0.9f, 0.f },
					{ 1.f, 0.9f, 0.9f, 1.f },
					{ 1.f, 0.9f, 0.9f, 1.f },
					{ 1.f, 1.f, 1.f, 0.75f },
					{ 1.f, 1.f, 1.f, 1.f }
				};

				std::memcpy(tracer->colors, colors, 5 * 4 * sizeof(float));
			}
			break;


			case Game::IW3::WEAPCLASS_MG:
			{
				tracer->name = LocalAllocator.DuplicateString("iw3x_tracer_machinegun");
				tracer->drawInterval = 2;
				tracer->speed = 5000.f;
				tracer->beamLength = 200.0f;
				tracer->beamWidth = 10.0f;
				tracer->screwRadius = 0.5f;
				tracer->screwDist = 100.0f;

				const float colors[5][4] = {
					{ 1.f, 0.75f, 0.75f, 0.f },
					{ 1.f, 0.75f, 0.75f, 1.f },
					{ 1.f, 1.f, 1.f, 1.f },
					{ 1.f, 0.75f, 0.75f, 1.f },
					{ 1.f, 0.75f, 0.75f, 0.f }
				};

				std::memcpy(tracer->colors, colors, 5 * 4 * sizeof(float));
			}
			break;

			case Game::IW3::WEAPCLASS_RIFLE:
			{
				if (weapon->penetrateType == Game::IW3::PENETRATE_TYPE_LARGE)
				{
					// sniper
					tracer->name = LocalAllocator.DuplicateString("iw3x_tracer_sniper");
					tracer->drawInterval = 3;
					tracer->speed = 7500.f;
					tracer->beamLength = 800.0f;
					tracer->beamWidth = 3.0f;
					tracer->screwRadius = 0.0f;
					tracer->screwDist = 100.0f;

					const float colors[5][4] = {
						{ 1.f, 1.f, 1.f, 0.f },
						{ 1.f, 1.f, 1.f, 0.5f },
						{ 1.f, 1.f, 1.f, 0.75f },
						{ 1.f, 1.f, 1.f, 0.5f },
						{ 1.f, 1.f, 1.f, 0.f }
					};

					std::memcpy(tracer->colors, colors, 5 * 4 * sizeof(float));
				}
				else
				{
					// assault rifle
					tracer->name = LocalAllocator.DuplicateString("iw3x_tracer_rifle");
					tracer->drawInterval = 3;
					tracer->speed = 4500.f;
					tracer->beamLength = 100.0f;
					tracer->beamWidth = 8.0f;
					tracer->screwRadius = 0.5f;
					tracer->screwDist = 50.0f;

					const float colors[5][4] = {
						{ 1.f, 0.75f, 0.75f, 0.f },
						{ 1.f, 0.75f, 0.75f, 1.f },
						{ 1.f, 0.75f, 0.75f, 1.f },
						{ 1.f, 1.f, 1.f, 0.5f },
						{ 1.f, 1.f, 1.f, 1.f }
					};

					std::memcpy(tracer->colors, colors, 5 * 4 * sizeof(float));
				}
			}
			break;
			}

			return tracer;
		}

		return nullptr;
	}

	void IWeapon::DifferentiateWeapon(Game::IW4::WeaponCompleteDef* weapon)
	{
		for (size_t i = 0; i < 16; i++)
		{
			if (weapon->weapDef->gunXModel[i])
			{
				DifferentiateModel(weapon->weapDef->gunXModel[i]);
			}

			if (weapon->weapDef->worldModel[i])
			{
				DifferentiateModel(weapon->weapDef->worldModel[i]);
			}
		}
	}

	void IWeapon::DifferentiateModel(Game::IW4::XModel* model)
	{
		const auto surfCount = model->numsurfs;
		for (size_t surfIndex = 0; surfIndex < surfCount; surfIndex++)
		{
			const auto material = model->materialHandles[surfIndex];
			std::string name = material->name;

			if (name.ends_with(IGfxImage::imageSuffix))
			{
				continue;
			}

			material->name = LocalAllocator.DuplicateString(std::format("{}{}", material->name, IGfxImage::imageSuffix));

			for (char textureIndex = 0; textureIndex < material->textureCount; textureIndex++)
			{
				const auto texture = &material->textureTable[textureIndex];
				const std::string textureName = texture->u.image->name;

				if (textureName.ends_with(IGfxImage::imageSuffix))
				{
					continue;
				}

				const auto newName = LocalAllocator.DuplicateString(std::format("{}{}", textureName, IGfxImage::imageSuffix));

				IGfxImage::AddRename(textureName, newName);

				texture->u.image->name = newName;
			}
		}
	}
}
