#include "STDInclude.hpp"

namespace Components
{
	std::vector<Component*> Loader::Components;
	Utils::Memory::Allocator Loader::MemAllocator;

	void Loader::Initialize()
	{
		Loader::MemAllocator.clear();

		Loader::Register(new Flags());

		Loader::Register(new Logger());
		Loader::Register(new Command());
		Loader::Register(new QuickPatch());
		Loader::Register(new AssetHandler());
		Loader::Register(new MapDumper());
	}

	void Loader::Uninitialize()
	{
		std::reverse(Loader::Components.begin(), Loader::Components.end());
		for (auto component : Loader::Components)
		{
#ifdef DEBUG
			Logger::Print("Unregistering component: %s\n", component->getName());
#endif
			delete component;
		}

		Loader::Components.clear();
		Loader::MemAllocator.clear();
		fflush(stdout);
		fflush(stderr);
	}

	void Loader::Register(Component* component)
	{
		if (component)
		{
#ifdef DEBUG
			Logger::Print("Component registered: %s\n", component->getName());
#endif
			Loader::Components.push_back(component);
		}
	}

	Utils::Memory::Allocator* Loader::GetAlloctor()
	{
		return &Loader::MemAllocator;
	}
}
