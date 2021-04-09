#pragma once

namespace Components
{
	class Component
	{
	public:
		Component() {};
		virtual ~Component() {};
		virtual const char* getName() { return "Unknown"; };
	};

	class Loader
	{
	public:
		static void Initialize();
		static void Uninitialize();
		static void Register(Component* component);

		static Utils::Memory::Allocator* GetAlloctor();

	private:
		static std::vector<Component*> Components;
		static Utils::Memory::Allocator MemAllocator;
	};
}

#include "Modules/Flags.hpp"
#include "Modules/Logger.hpp"
#include "Modules/Command.hpp"
#include "Modules/FileSystem.hpp"
#include "Modules/QuickPatch.hpp"
#include "Modules/AssetHandler.hpp"

#include "Modules/IXModel.hpp"
#include "Modules/IRawFile.hpp"
#include "Modules/IMapEnts.hpp"
#include "Modules/IComWorld.hpp"
#include "Modules/IGfxImage.hpp"
#include "Modules/IGfxWorld.hpp"
#include "Modules/IMaterial.hpp"
#include "Modules/IclipMap_t.hpp"
#include "Modules/IXAnimParts.hpp"
#include "Modules/IFxEffectDef.hpp"
#include "Modules/IGfxLightDef.hpp"
#include "Modules/IMaterialTechniqueSet.hpp"
#include "Modules/Isnd_alias_list_t.hpp"
#include "Modules/ILoadedSound.hpp"
