#include "tolua_platform.h"
#include "GPlatformConfig.h"
#include "GFileSystem.h"


#ifdef G_TARGET_PLATFORM == G_PLATFORM_WIN32
#include "windows.h"
#endif

void tolua_platform(sol::state& lua)
{
    auto platform = lua["platform"].get_or_create<sol::table>();

    platform.set_function("ShowConsole", [](bool value) {
#ifdef G_TARGET_PLATFORM == G_PLATFORM_WIN32
        HWND hWnd = GetConsoleWindow();
    ShowWindow(hWnd, value ? SW_SHOW : SW_HIDE);
#endif
    });


    auto filesystem = lua["filesystem"].get_or_create<sol::table>();
    filesystem.set_function("isFileExist", GFileSystem::isFileExist);
    filesystem.set_function("isDirectoryExist", GFileSystem::isDirectoryExist);
    filesystem.set_function("createDirectory", GFileSystem::createDirectory);
    filesystem.set_function("removeDirectory", GFileSystem::removeDirectory);
    filesystem.set_function("getFileSize", GFileSystem::getFileSize);
    filesystem.set_function("removeFile", GFileSystem::removeFile);
    filesystem.set_function("renameFile", GFileSystem::renameFile);
    filesystem.set_function("readDataFromFile", GFileSystem::readStringFromFile);
    filesystem.set_function("writeDataToFile", GFileSystem::writeTextFile);
    filesystem.set_function("baseName", GFileSystem::baseName);
    filesystem.set_function("dirName", GFileSystem::dirName);
    filesystem.set_function("getFileExtension", GFileSystem::getFileExtension);
    filesystem.set_function("getExeDirectory", GFileSystem::getExeDirectory);
    filesystem.set_function("getExeName", GFileSystem::getExeName);
    filesystem.set_function("getCwd", GFileSystem::getCwd);
    filesystem.set_function("setCwd", GFileSystem::setCwd);
    filesystem.set_function("convertPathFormatToUnixStyle", GFileSystem::convertPathFormatToUnixStyle);
    filesystem.set_function("convertPathFormatToWinStyle", GFileSystem::convertPathFormatToWinStyle);
    filesystem.set_function("isRelativePath", GFileSystem::isRelativePath);
    filesystem.set_function("isAbsolutePath", GFileSystem::isAbsolutePath);

    filesystem.set_function("listFilesRecursively", [](sol::this_state ts, const std::string& dirPath)->sol::table {
        std::vector<GFileItem> files;
        GFileSystem::listFilesRecursively(dirPath, files);

        sol::state_view lua(ts);
        sol::table data = lua.create_table();
        for (size_t i = 0; i < files.size(); ++i)
        {
            auto& file = files[i];

            auto item = lua.create_table();
            item["name"] = file.name;
            item["is_dir"] = file.is_dir;
            data[i] = item;
        }
        return data;
    });

    filesystem.set_function("listFiles", [](sol::this_state ts, const std::string& dirPath)->sol::table {
        std::vector<GFileItem> files;
        GFileSystem::listFiles(dirPath, files);

        sol::state_view lua(ts);
        sol::table data = lua.create_table();
        for (size_t i = 0; i < files.size(); ++i)
        {
            auto& file = files[i];

            auto item = lua.create_table();
            item["name"] = file.name;
            item["is_dir"] = file.is_dir;
            data[i] = item;
        }
        return data;
    });
}
