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
		free(outputBuffer);
		return NULL;
	}

	iconv_close(cd);

	outputLength = outBufLength - outputLength;
	return outputRawBuffer;
}

void tolua_utils(sol::state& lua)
{
    auto utils = lua["utils"].get_or_create<sol::table>();
    utils.set_function("nowEpochMS", []()->int64_t {
        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
    });

    utils.set_function("nowEpoch10m", []()->int64_t {
        typedef std::chrono::duration<long long, std::ratio<1LL, 10000000LL>> duration_10m;
        auto val = std::chrono::steady_clock::now();
        return std::chrono::duration_cast<duration_10m>(val.time_since_epoch()).count();
    });

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
				}

				lua_pushlstring(L, (const char*)outputData, outputLength);
				free(outputData);
			}
			else
			{
				lua_pushstring(L, "");
			}
			return 1;
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









	//// 输入的文本
	//const char* inputText = "hello";

	//// 设置输入和输出的编码
	//const char* fromEncoding = "UTF-8";
	//const char* toEncoding = "GB2312";

	//// 创建 iconv 转换句柄
	//iconv_t cd = iconv_open(toEncoding, fromEncoding);
	//if (cd == (iconv_t)-1) {
	//	std::cerr << "Failed to create iconv conversion descriptor" << std::endl;
	//	return;
	//}

	//// 输入和输出缓冲区
	//char outputBuffer[256];
	//char* outputPointer = outputBuffer;
	//size_t inputLength = strlen(inputText);
	//size_t outputLength = sizeof(outputBuffer);

	//// 进行编码转换
	//size_t result = iconv(cd, const_cast<const char**>(&inputText), &inputLength, &outputPointer, &outputLength);
	//if (result == static_cast<size_t>(-1)) {
	//	std::cerr << "Failed to convert encoding: " << strerror(errno) << std::endl;
	//	iconv_close(cd);
	//	return;
	//}

	//// 输出转换后的文本
	//std::cout << "Converted text: " << std::string(outputBuffer, sizeof(outputBuffer) - outputLength) << std::endl;

	//// 关闭 iconv 转换句柄
	//iconv_close(cd);
}
