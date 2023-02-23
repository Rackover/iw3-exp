#pragma once

namespace Components
{
	class QuickPatch : public Component
	{
	public:
		QuickPatch();
		~QuickPatch();
		const char* GetName() override { return "QuickPatch"; };

	private:
		static void OnInitStub();
		static int SND_SetDataHook(Game::IW3::MssSound* a1, char* a2);
		static void PerformInit();
	};
}
