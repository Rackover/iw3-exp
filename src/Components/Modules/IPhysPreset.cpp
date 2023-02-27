#include "STDInclude.hpp"

namespace Components
{
	Game::IW4::PhysPreset* IPhysPreset::Convert(Game::IW3::PhysPreset* physPreset)
	{
		if (!physPreset) return nullptr;

		auto iw4Preset = LocalAllocator.Allocate<Game::IW4::PhysPreset>();
		static_assert(sizeof Game::IW3::PhysPreset == sizeof Game::IW4::PhysPreset);

		std::memcpy(iw4Preset, physPreset, sizeof Game::IW3::PhysPreset);
		iw4Preset->perSurfaceSndAlias = false;

		return iw4Preset;
	}

	IPhysPreset::IPhysPreset()
	{
	}

	IPhysPreset::~IPhysPreset()
	{
	}
}
