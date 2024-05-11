#pragma once

#include "GPlatformMacros.h"
#include <vector>
#include <string>

struct GFileItem
{
	std::string name;
	bool is_dir;
};

class GFileSystem
{
public:
	static bool isFileExist(const std::string& filename);
	static bool isDirectoryExist(const std::string& path);

	static bool createDirectory(const std::string& path);
	static bool removeDirectory(const std::string& dirPath);

	static int64_t getFileSize(const std::string& path);
	static bool removeFile(const std::string &filepath);
	static bool renameFile(const std::string &oldfullpath, const std::string &newfullpath);

	static uint8_t* readFile(const std::string& filename, uint32_t& outSize);
	static std::string readStringFromFile(const std::string& filename);
	static bool readStringFromFileEx(const std::string& filename, std::string& outData);

	static bool writeFile(const std::string& path, uint8_t* buffer, uint32_t size);
	static bool writeTextFile(const std::string& path, const std::string& text);

	static void listFilesRecursively(const std::string& dirPath, std::vector<GFileItem>& files);

	static void listFiles(const std::string& dirPath, std::vector<GFileItem> & files);

	static std::string baseName(const std::string& filepath);
	static std::string dirName(const std::string& filepath);
	static std::string getFileExtension(std::string filePath);

	static std::string getExeDirectory();
	static std::string getExeName();

	static std::string getCwd();
	static void setCwd(const std::string& cwd);

	// D:\aaa\bbb\ccc\ddd\abc.txt --> D:/aaa/bbb/ccc/ddd/abc.txt
	static std::string convertPathFormatToUnixStyle(const std::string& path);
	static std::string convertPathFormatToWinStyle(const std::string& path);

	static bool isRelativePath(const char* path);
	static bool isAbsolutePath(const char* path);

};
