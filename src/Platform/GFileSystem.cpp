#include "GFileSystem.h"
#include "tinydir/tinydir.h"
#include "Utils/GStringUtils.h"


void GFileSystem::listFilesRecursively(const std::string& dirPath, std::vector<GFileItem>& files)
{
	std::string fullpath = dirPath;

	tinydir_dir dir;
#if defined _MSC_VER || defined __MINGW32__
#ifdef _UNICODE
	auto fullpathstr = StringUtils::stringUtf8ToWideChar(fullpath);
#else
	auto& fullpathstr = fullpath;
#endif
#else
	auto& fullpathstr = fullpath;
#endif

	if (tinydir_open(&dir, &fullpathstr[0]) != -1)
	{
		while (dir.has_next)
		{
			tinydir_file file;
			if (tinydir_readfile(&dir, &file) == -1)
			{
				// Error getting file
				break;
			}
#if defined _MSC_VER || defined __MINGW32__
#ifdef _UNICODE
			std::string fileName = StringUtils::stringWideCharToUtf8(file.name);
#else
			std::string fileName = file.name;
#endif
#else
			std::string fileName = file.name;
#endif

			if (fileName != "." && fileName != "..")
			{
#if defined _MSC_VER || defined __MINGW32__
#ifdef _UNICODE
				std::string filepath = StringUtils::stringWideCharToUtf8(file.path);
#else
				std::string filepath = file.path;
#endif
#else
				std::string filepath = file.path;
#endif
				if (file.is_dir)
				{
					filepath.append("/");

					GFileItem item;
					item.name = filepath;
					item.is_dir = true;
					files.push_back(item);
					listFilesRecursively(filepath, files);
				}
				else
				{
					GFileItem item;
					item.name = filepath;
					item.is_dir = false;
					files.push_back(item);
				}
			}

			if (tinydir_next(&dir) == -1)
			{
				// Error getting next file
				break;
			}
		}
	}
	tinydir_close(&dir);
}

void GFileSystem::listFiles(const std::string& dirPath, std::vector<GFileItem>& files)
{
	std::string fullpath = dirPath;

	tinydir_dir dir;
#if defined _MSC_VER || defined __MINGW32__
#ifdef _UNICODE
	auto fullpathstr = StringUtils::stringUtf8ToWideChar(fullpath);
#else
	auto& fullpathstr = fullpath;
#endif
#else
	auto &fullpathstr = fullpath;
#endif

	if (tinydir_open(&dir, &fullpathstr[0]) != -1)
	{
		while (dir.has_next)
		{
			tinydir_file file;
			if (tinydir_readfile(&dir, &file) == -1)
			{
				// Error getting file
				break;
			}
#if defined _MSC_VER || defined __MINGW32__
#ifdef _UNICODE
			std::string fileName = StringUtils::stringWideCharToUtf8(file.name);
#else
			std::string fileName = file.name;
#endif
#else
			std::string fileName = file.name;
#endif
			if (fileName != "." && fileName != "..")
			{
#if defined _MSC_VER || defined __MINGW32__
#ifdef _UNICODE
				std::string filepath = StringUtils::stringWideCharToUtf8(file.path);
#else
				std::string filepath = file.path;
#endif
#else
				std::string filepath = file.path;
#endif
				if (file.is_dir)
				{
					filepath.append("/");
					GFileItem item;
					item.name = filepath;
					item.is_dir = true;
					files.push_back(item);
				}
				else
				{
					GFileItem item;
					item.name = filepath;
					item.is_dir = false;
					files.push_back(item);
				}
			}

			if (tinydir_next(&dir) == -1)
			{
				// Error getting next file
				break;
			}
		}
	}
	tinydir_close(&dir);
}

std::string GFileSystem::baseName(const std::string& filepath)
{
	size_t pos = filepath.find_last_of("/\\");
	if (pos != std::string::npos)
	{
		return filepath.substr(pos + 1);
	}

	return filepath;
}

std::string GFileSystem::dirName(const std::string& filepath)
{
	size_t pos = filepath.find_last_of("/\\");
	if (pos != std::string::npos)
	{
		return filepath.substr(0, pos + 1);
	}

	return filepath;
}

std::string GFileSystem::getFileExtension(std::string filePath)
{
	std::string fileExtension;
	size_t pos = filePath.find_last_of('.');
	if (pos != std::string::npos)
	{
		fileExtension = filePath.substr(pos, filePath.length());

		std::transform(fileExtension.begin(), fileExtension.end(), fileExtension.begin(), ::tolower);
	}

	return fileExtension;
}

// D:\aaa\bbb\ccc\ddd\abc.txt --> D:/aaa/bbb/ccc/ddd/abc.txt
std::string GFileSystem::convertPathFormatToUnixStyle(const std::string& path)
{
	std::string ret{ path };
	int len = ret.length();
	std::replace(ret.begin(), ret.end(), '\\', '/');
	return ret;
}

std::string GFileSystem::convertPathFormatToWinStyle(const std::string& path)
{
	std::string ret{ path };
	int len = ret.length();
	std::replace(ret.begin(), ret.end(), '/', '\\');
	return ret;
}

bool GFileSystem::isRelativePath(const char* path)
{
	if(!path || path[0] == '/' || path[0] == '\\')
	{
		return false;
	}

	if(strlen(path) >= 2 && isalpha(path[0]) && path[1] == ':')
	{
		return false;
	}

	return true;
}

bool GFileSystem::isAbsolutePath(const char* path)
{
	if(!path)
	{
		return false;
	}

	return !isRelativePath(path);
}