#include "STDInclude.hpp"

namespace Components
{
	bool Logger::IsConsoleReady()
	{
		return (IsWindow(*reinterpret_cast<HWND*>(0xCC1AD98)) != FALSE);
	}

	void Logger::Print(const char* message, ...)
	{
		char buffer[0x1000] = { 0 };

		va_list ap;
		va_start(ap, message);
		vsprintf_s(buffer, message, ap);
		va_end(ap);

		if(Flags::HasFlag("stdout"))
		{
			printf("%s", buffer);
			fflush(stdout);
		}
		else if (Logger::IsConsoleReady())
		{
			Game::Com_PrintMessage(0, buffer, 0);
		}
		else
		{
			OutputDebugStringA(buffer);
		}
	}

	void Logger::Error(const char* message, ...)
	{
		char buffer[0x1000] = { 0 };

		va_list ap;
		va_start(ap, message);
		vsprintf_s(buffer, message, ap);
		va_end(ap);

		Game::Com_Error(0, "%s", buffer);
	}

	void Logger::SoftError(const char* message, ...)
	{
		char buffer[0x1000] = { 0 };

		va_list ap;
		va_start(ap, message);
		vsprintf_s(buffer, message, ap);
		va_end(ap);

		Game::Com_Error(2, "%s", buffer);
	}

	void Logger::ErrorStub(int type, const char* message, ...)
	{
		char buffer[0x1000] = { 0 };

		va_list ap;
		va_start(ap, message);
		vsprintf_s(buffer, message, ap);
		va_end(ap);

		printf("%s", buffer);
		fflush(stdout);

		if (!type) ExitProcess(1);
	}

	Logger::Logger()
	{
		if(Flags::HasFlag("stdout"))
		{
			Utils::Hook(Game::Com_Error, Logger::ErrorStub, HOOK_JUMP).install()->quick();
			Utils::Hook(0x4FCC80, Logger::ErrorStub, HOOK_JUMP).install()->quick();
			Utils::Hook(0x4FCD30, Logger::ErrorStub, HOOK_JUMP).install()->quick();

			// Don't create splash screen
			Utils::Hook::Nop(0x57761C, 5);

			// Don't create console
			Utils::Hook::Set<BYTE>(0x57A430, 0xC3);
		}
		else
		{
			FreeConsole();
		}
	}

	Logger::~Logger()
	{

	}
}
