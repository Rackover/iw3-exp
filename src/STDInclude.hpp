#pragma once

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <wincrypt.h>
#include <time.h>
#include <timeapi.h>
#include <shellapi.h>
#include <WinSock2.h>
#include <assert.h>

#pragma warning(push)
#pragma warning(disable: 4244)
#include <map>
#include <mutex>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <utility>
#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>
#include <regex>
#include <thread>
#include <chrono>
#include <future>
#include <unordered_map>
#pragma warning(pop)

#define PC
#define COD4

// Usefull for debugging
template <size_t S> class Sizer { };
#define BindNum(x, y) Sizer<x> y;
#define SizeOf(x, y) BindNum(sizeof(x), y)
#define OffsetOf(x, y, z) BindNum(offsetof(x, y), z)

// Revision number
#define STRINGIZE_(x) #x
#define STRINGIZE(x) STRINGIZE_(x)
#define RAPIDJSON_STR(str) rapidjson::Value(str, strnlen_s(str, 128), allocator)
#define AssertSize(x, size) static_assert(sizeof(x) == size, STRINGIZE(x) " structure has an invalid size.")

// Version number
#include <version.hpp>

#include <s3tc.h>

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>

#include "Utils/Utils.hpp"
#include "Utils/Memory.hpp"
#include "Utils/Stream.hpp"
#include "Utils/Hooking.hpp"
#include "Utils/Entities.hpp"

#include "Game/Structs.hpp"
#include "Game/Functions.hpp"

#include "Components/Loader.hpp"

using namespace std::literals;
