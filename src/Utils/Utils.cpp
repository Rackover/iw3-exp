#include "STDInclude.hpp"

#define VA_BUFFER_COUNT		32
#define VA_BUFFER_SIZE		65536

namespace Utils
{
	const char *VA(const char *fmt, ...)
	{
		static char g_vaBuffer[VA_BUFFER_COUNT][VA_BUFFER_SIZE];
		static int g_vaNextBufferIndex = 0;

		va_list ap;
		va_start(ap, fmt);
		char* dest = g_vaBuffer[g_vaNextBufferIndex];
		vsnprintf(g_vaBuffer[g_vaNextBufferIndex], VA_BUFFER_SIZE, fmt, ap);
		g_vaNextBufferIndex = (g_vaNextBufferIndex + 1) % VA_BUFFER_COUNT;
		va_end(ap);
		return dest;
	}

	std::string StrToLower(std::string input)
	{
		std::transform(input.begin(), input.end(), input.begin(), ::tolower);
		return input;
	}

	bool EndsWith(std::string haystack, std::string needle)
	{
		return (strstr(haystack.data(), needle.data()) == (haystack.data() + haystack.size() - needle.size()));
	}

	// Complementary function for memset, which checks if a memory is set
	bool MemIsSet(void* mem, char chr, size_t length)
	{
		char* memArr = reinterpret_cast<char*>(mem);

		for (size_t i = 0; i < length; ++i)
		{
			if (memArr[i] != chr)
			{
				return false;
			}
		}

		return true;
	}

	std::vector<std::string> Explode(const std::string& str, char delim)
	{
		std::vector<std::string> result;
		std::istringstream iss(str);

		for (std::string token; std::getline(iss, token, delim);)
		{
			std::string _entry = std::move(token);

			// Remove trailing 0x0 bytes
			while (_entry.size() && !_entry[_entry.size() - 1])
			{
				_entry = _entry.substr(0, _entry.size() - 1);
			}

			result.push_back(_entry);
		}

		return result;
	}

	void Replace(std::string &string, std::string find, std::string replace)
	{
		size_t nPos = 0;

		while ((nPos = string.find(find, nPos)) != std::string::npos)
		{
			string = string.replace(nPos, find.length(), replace);
			nPos += replace.length();
		}
	}

	bool StartsWith(std::string haystack, std::string needle)
	{
		return (haystack.size() >= needle.size() && !strncmp(needle.data(), haystack.data(), needle.size()));
	}

	void EraseSubstring(std::string &base, std::string replace)
	{
		auto it = base.find(replace.c_str());
		if (it != std::string::npos)
		{
			base.erase(it, replace.size());
		}
	}

	unsigned int OneAtATime(const char *key, size_t len)
	{
		unsigned int hash, i;
		for (hash = i = 0; i < len; ++i)
		{
			hash += key[i];
			hash += (hash << 10);
			hash ^= (hash >> 6);
		}
		hash += (hash << 3);
		hash ^= (hash >> 11);
		hash += (hash << 15);
		return hash;
	}

	int IsSpace(int c)
	{
		if (c < -1) return 0;
		return _isspace_l(c, nullptr);
	}

	// trim from start
	std::string &LTrim(std::string &s)
	{
		s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int val)
		{
			return !IsSpace(val);
		}));
		return s;
	}

	// trim from end
	std::string &RTrim(std::string &s)
	{
		s.erase(std::find_if(s.rbegin(), s.rend(), [](int val)
		{
			return !IsSpace(val);
		}).base(), s.end());
		return s;
	}

	// trim from both ends
	std::string &Trim(std::string &s)
	{
		return LTrim(RTrim(s));
	}

	std::string FormatTimeSpan(int milliseconds)
	{
		int secondsTotal = milliseconds / 1000;
		int seconds = secondsTotal % 60;
		int minutesTotal = secondsTotal / 60;
		int minutes = minutesTotal % 60;
		int hoursTotal = minutesTotal / 60;

		return Utils::VA("%02d:%02d:%02d", hoursTotal, minutes, seconds);
	}

	std::string ParseChallenge(std::string data)
	{
		auto pos = data.find_first_of("\n ");
		if (pos == std::string::npos) return data;
		return data.substr(0, pos).data();
	}

	void CreateDir(std::string dir)
	{
		// Win32 compatibility
		for (unsigned int i = 0; i < dir.size(); ++i)
		{
			if (dir[i] == '/')
			{
				dir[i] = '\\';
			}
		}

		// Terminate the path.
		for (auto i = dir.begin(); i != dir.end(); ++i)
		{
			if(*i == '\\')
			{
				CreateDirectoryW(std::wstring(dir.begin(), i).data(), nullptr);
			}
		}

		CreateDirectoryW(std::wstring(dir.begin(), dir.end()).data(), nullptr);
	}

	// TODO: Use modern file reading methods
	bool FileExists(std::string file)
	{
		FILE* fp;
		fopen_s(&fp, file.data(), "r");

		if (fp)
		{
			fclose(fp);
			return true;
		}

		return false;
	}

	void WriteFile(std::string file, std::string data)
	{
		auto pos = file.find_last_of("/\\");
		if (pos != std::string::npos)
		{
			CreateDir(file.substr(0, pos));
		}

		std::ofstream stream(file, std::ios::binary);

		if (stream.is_open())
		{
			stream.write(data.data(), data.size());
			stream.close();
		}
	}

	std::string ReadFile(std::string file)
	{
		std::string buffer;

		if (FileExists(file))
		{
			std::streamsize size;
			std::ifstream stream(file, std::ios::binary);
			if (!stream.is_open()) return buffer;

			stream.seekg(0, std::ios::end);
			size = stream.tellg();
			stream.seekg(0, std::ios::beg);

			if (size > -1)
			{
				buffer.clear();
				buffer.resize(static_cast<uint32_t>(size));

				stream.read(const_cast<char*>(buffer.data()), size);
			}

			stream.close();
		}

		return buffer;
	}


	// Infostring class
	void InfoString::Set(std::string key, std::string value)
	{
		this->KeyValuePairs[key] = value;
	}

	std::string InfoString::Get(std::string key)
	{
		if (this->KeyValuePairs.find(key) != this->KeyValuePairs.end())
		{
			return this->KeyValuePairs[key];
		}

		return "";
	}

	std::string InfoString::Build()
	{
		std::string infoString;

		bool first = true;

		for (auto i = this->KeyValuePairs.begin(); i != this->KeyValuePairs.end(); ++i)
		{
			if (first) first = false;
			else infoString.append("\\");

			infoString.append(i->first); // Key
			infoString.append("\\");
			infoString.append(i->second); // Value
		}

		return infoString;
	}

	void InfoString::Dump()
	{
		for (auto i = this->KeyValuePairs.begin(); i != this->KeyValuePairs.end(); ++i)
		{
			OutputDebugStringA(Utils::VA("%s: %s", i->first.data(), i->second.data()));
		}
	}

	void InfoString::Parse(std::string buffer)
	{
		if (buffer[0] == '\\')
		{
			buffer = buffer.substr(1);
		}

		std::vector<std::string> KeyValues = Utils::Explode(buffer, '\\');

		for (unsigned int i = 0; i < (KeyValues.size() - 1); i+=2)
		{
			this->KeyValuePairs[KeyValues[i]] = KeyValues[i + 1];
		}
	}
}
