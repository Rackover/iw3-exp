#pragma once

namespace Components
{
	class Logger : public Component
	{
	public:
		Logger();
		~Logger();
		const char* getName() override { return "Logger"; };

		static void Print(const char* message, ...);
		static void Error(const char* message, ...);
		static void SoftError(const char* message, ...);
		static bool IsConsoleReady();

		static void ErrorStub(int type, const char* message, ...);

	private:
	};
}
