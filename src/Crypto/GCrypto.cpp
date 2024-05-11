#include "GCrypto.h"
#include "crc32.hpp"
#include "base64.hpp"
#include "sha1.hpp"
#include "md5.h"
#include <iostream>
#include "spdlog/spdlog.h"

uint32_t GCrypto::CRC32(const std::string& str)
{
	return NFrame::CRC32(str);
}

std::string GCrypto::encodeBase64(const std::string& plaintext)
{
	auto len = base64::encoded_size(plaintext.size());

	std::string ciphertext;
	ciphertext.reserve(len);
	
	if (base64::encode((void*)ciphertext.data(), (const void*)plaintext.data(), plaintext.size()) != len)
	{
		spdlog::error("encode base64 error: {}", plaintext);
	}
	return std::string(ciphertext.data(), len);
}

std::string GCrypto::decodedBase64(const std::string& ciphertext)
{
	auto len = base64::decoded_size(ciphertext.size());

	std::string plaintext;
	plaintext.reserve(len);

	auto result = base64::decode((void*)plaintext.data(), ciphertext.data(), ciphertext.size());
	if (result.first <= 0)
	{
		spdlog::error("decode base64 error: {}", ciphertext);
	}
	return std::string(plaintext.data(), len);
}

std::string GCrypto::SHA1(const std::string& str)
{
	::SHA1 checksum;
	checksum.update(str);
	return checksum.final();
}

std::string GCrypto::SHA1_from_file(const std::string& filename)
{
	return SHA1::from_file(filename);
}

std::string GCrypto::md5(const std::string& str)
{
	return md5Data((uint8_t*)str.c_str(), (int32_t)str.size());
}

std::string GCrypto::md5Data(uint8_t* data, int32_t len)
{
	static const unsigned int MD5_DIGEST_LENGTH = 16;

	md5_state_t state;
	md5_byte_t digest[MD5_DIGEST_LENGTH];
	char hexOutput[(MD5_DIGEST_LENGTH << 1) + 1] = { 0 };

	md5_init(&state);
	md5_append(&state, (const md5_byte_t*)data, len);
	md5_finish(&state, digest);

	for (int di = 0; di < 16; ++di)
		sprintf(hexOutput + di * 2, "%02x", digest[di]);
	return hexOutput;
}

std::string GCrypto::md5_from_file(const std::string& filename)
{
	std::ifstream file(filename.c_str(), std::ios::binary);
	if (!file.is_open())
	{
		spdlog::error("Error opening file: {}", filename);
		return "";
	}

	static const unsigned int MD5_DIGEST_LENGTH = 16;

	md5_state_t state;
	md5_byte_t digest[MD5_DIGEST_LENGTH];
	char hexOutput[(MD5_DIGEST_LENGTH << 1) + 1] = { 0 };

	md5_init(&state);

	const int buffer_size = 16 * 1024; // 16kb
	auto buffer = new char[buffer_size];

	// 按照指定大小读取文件数据，直到文件末尾
	while (file) 
	{
		file.read(buffer, buffer_size);

		// 获取实际读取的字节数
		std::streamsize bytes_read = file.gcount();

		md5_append(&state, (const md5_byte_t*)buffer, bytes_read);
	}
	delete[] buffer;
	file.close();
	
	md5_finish(&state, digest);

	for (int di = 0; di < 16; ++di)
		sprintf(hexOutput + di * 2, "%02x", digest[di]);
	return hexOutput;
}
