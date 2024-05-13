#include "tolua_utils.h"
#include <chrono>
#include "uchardet.h"
#include "spdlog/spdlog.h"
#include <fstream>
#include "iconv.h"
#include "Platform/GFileSystem.h"

static std::string detectFileCode(const std::string& filename)
{
	std::ifstream file(filename.c_str(), std::ios::binary);
	if (!file.is_open())
	{
		spdlog::error("Error opening file: {}", filename);
		return "unknown";
	}

	uchardet_t handle = uchardet_new();
	const int buffer_size = 16 * 1024; // 16kb
	auto buffer = new char[buffer_size];

	while (file)
	{
		file.read(buffer, buffer_size);

		std::streamsize bytes_read = file.gcount();
		int retval = uchardet_handle_data(handle, buffer, bytes_read);
		if (retval != 0)
		{
			delete[] buffer;
			file.close();
			return "unknown";
		}
	}
	delete[] buffer;
	file.close();

	uchardet_data_end(handle);

	std::string out = "unknown";

	const char* charset = uchardet_get_charset(handle);
	if (*charset)
		out = charset;

	uchardet_delete(handle);

	return out;
}


static std::string detectStringCode(const std::string& str)
{
	uchardet_t handle = uchardet_new();
	int retval = uchardet_handle_data(handle, str.data(), str.length());
	if (retval != 0)
	{
		return "unknown";
	}

	uchardet_data_end(handle);

	std::string out = "unknown";
	const char* charset = uchardet_get_charset(handle);
	if (*charset)
		out = charset;

	uchardet_delete(handle);

	return out;
}

static char* convertEncoding(const char* inputText, size_t inputLength, const char* fromEncoding, const char* toEncoding, size_t& outputLength)
{
	iconv_t cd = iconv_open(toEncoding, fromEncoding);
	if (cd == (iconv_t)-1) 
	{
		spdlog::error("Failed to create iconv conversion descriptor");
		return NULL;
	}

	const size_t outBufLength = inputLength * 3;
	char* outputBuffer = (char*)malloc(outBufLength);
	char* outputRawBuffer = outputBuffer;
	if (outputBuffer == NULL)
	{
		spdlog::error("Not enough storage");
		return outputBuffer;
	}

	const char** pin = &inputText;
	char** pout = &outputBuffer;

	outputLength = outBufLength;
	size_t result = iconv(cd, pin, &inputLength, pout, &outputLength);
	if (result == static_cast<size_t>(-1))
	{
		spdlog::error("Failed to convert encoding: {}", strerror(errno));
		iconv_close(cd);
		free(outputRawBuffer);
		return NULL;
	}

	iconv_close(cd);

	outputLength = outBufLength - outputLength;
	return outputRawBuffer;
}

#if G_TARGET_PLATFORM == G_PLATFORM_WIN32
std::string utf8_to_ansi(const std::string& utf8_text) 
{
	int wlen = MultiByteToWideChar(CP_UTF8, 0, utf8_text.c_str(), -1, NULL, 0);
	std::vector<wchar_t> wbuf(wlen);
	MultiByteToWideChar(CP_UTF8, 0, utf8_text.c_str(), -1, wbuf.data(), wlen);

	int len = WideCharToMultiByte(CP_ACP, 0, wbuf.data(), -1, NULL, 0, NULL, NULL);
	std::vector<char> buf(len);
	WideCharToMultiByte(CP_ACP, 0, wbuf.data(), -1, buf.data(), len, NULL, NULL);

	return std::string(buf.data());
}
std::string ansi_to_utf8(const std::string& ansi_text) 
{
	int wlen = MultiByteToWideChar(CP_ACP, 0, ansi_text.c_str(), -1, NULL, 0);
	std::vector<wchar_t> wbuf(wlen);
	MultiByteToWideChar(CP_ACP, 0, ansi_text.c_str(), -1, wbuf.data(), wlen);

	int len = WideCharToMultiByte(CP_UTF8, 0, wbuf.data(), -1, NULL, 0, NULL, NULL);
	std::vector<char> buf(len);
	WideCharToMultiByte(CP_UTF8, 0, wbuf.data(), -1, buf.data(), len, NULL, NULL);

	return std::string(buf.data());
}
#else
std::string utf8_to_ansi(const std::string& utf8_text)
{
	return utf8_text;
}
std::string ansi_to_utf8(const std::string& ansi_text)
{
	return utf8_text;
}
#endif

void tolua_utils(sol::state& lua)
{
	UINT codePage = GetACP();

	CPINFOEX cpInfo;
	if (GetCPInfoEx(codePage, 0, &cpInfo)) {
		std::string codePageName = cpInfo.CodePageName;
		std::cout << "Current system code page name: " << cpInfo.CodePageName << std::endl;
	}
	else {
		std::cerr << "Failed to get code page information." << std::endl;
	}

    auto utils = lua["utils"].get_or_create<sol::table>();
    utils.set_function("nowEpochMS", []()->int64_t {
        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
    });

    utils.set_function("nowEpoch10m", []()->int64_t {
        typedef std::chrono::duration<long long, std::ratio<1LL, 10000000LL>> duration_10m;
        auto val = std::chrono::steady_clock::now();
        return std::chrono::duration_cast<duration_10m>(val.time_since_epoch()).count();
    });

	utils.set_function("utf8_to_ansi", utf8_to_ansi);
	utils.set_function("ansi_to_utf8", ansi_to_utf8);
    utils.set_function("detectFileCode", detectFileCode);
	utils.set_function("detectStringCode", detectStringCode);
	utils.set_function("convertStringEncoding", [](lua_State* L) -> int
	{
		auto top = lua_gettop(L);
		if (top == 3 && lua_isstring(L, 1) && lua_isstring(L, 2) && lua_isstring(L, 3))
		{
			size_t s_len = 0;
			const char* s = lua_tolstring(L, 1, &s_len);

			const char* fromEncoding = lua_tostring(L, 2);
			const char* toEncoding = lua_tostring(L, 3);

			if (s_len > 0)
			{
				size_t outputLength;
				auto outputData = convertEncoding((const char*)s, s_len, fromEncoding, toEncoding, outputLength);
				if (outputData == NULL)
				{
					lua_pushnil(L);
					return 1;
				}

				lua_pushlstring(L, (const char*)outputData, outputLength);
				free(outputData);
				return 1;
			}
			else
			{
				lua_pushstring(L, "");
				return 1;
			}
		}
		spdlog::error("Encoding conversion function parameter error");
		lua_pushnil(L);
		return 1;
	});
	utils.set_function("convertFileEncoding", [](lua_State* L) -> int
	{
		auto top = lua_gettop(L);
		if (top >= 3 && lua_isstring(L, 1) && lua_isstring(L, 2) && lua_isstring(L, 3))
		{
			const char* filename = lua_tostring(L, 1);
			const char* fromEncoding = lua_tostring(L, 2);
			const char* toEncoding = lua_tostring(L, 3);

			std::string fileData;
			if (GFileSystem::readStringFromFileEx(filename, fileData))
			{
				size_t outputLength;
				auto outputData = convertEncoding((const char*)fileData.data(), fileData.size(), fromEncoding, toEncoding, outputLength);
				if (outputData == NULL)
				{
					lua_pushboolean(L, false);
					return 1;
				}

				if (lua_isstring(L, 4))
				{
					const char* toFileName = lua_tostring(L, 4);
					lua_pushboolean(L, GFileSystem::writeFile(toFileName, (uint8_t*)outputData, outputLength));
					free(outputData);
					return 1;
				}
				else
				{
					lua_pushboolean(L, true);
					lua_pushlstring(L, (const char*)outputData, outputLength);
					free(outputData);
					return 2;
				}
			}
			else
			{
				lua_pushboolean(L, false);
			}
			return 1;
		}
		spdlog::error("Encoding conversion function parameter error");
		lua_pushnil(L);
		return 1;
	});
}
