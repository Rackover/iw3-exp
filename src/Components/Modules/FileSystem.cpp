#include "STDInclude.hpp"

namespace Components
{
	void FileSystem::File::ReadOnHunk()
	{
		char* buffer = nullptr;
		int size = Game::FS_ReadFile(this->FilePath.data(), &buffer);

		this->Buffer.clear();

		if (size < 0)
		{
			if (buffer)
			{
				Game::FS_FreeFile(buffer);
			}
		}
		else
		{
			this->Buffer.append(buffer, size);
			Game::FS_FreeFile(buffer);
		}
	}

	void FileSystem::File::Read()
	{
		this->Buffer.clear();

		int handle;
		Game::FS_FOpenFileReadDatabase(this->FilePath.data(), &handle);

		if (handle > 0)
		{
			constexpr unsigned int BUFF_SIZE = 1024;

			while (true)
			{
				char buffer[BUFF_SIZE];
				auto size_read =Game::FS_Read(buffer, BUFF_SIZE, handle);

				this->Buffer.append(buffer, size_read);

				if (size_read < BUFF_SIZE)
				{
					// We're done!
					break;
				}
			}


			Game::FS_FCloseFile(handle);
		}
	}

	FileSystem::FileSystem()
	{

	}
}
