#include "sol/sol.hpp"
#include <iostream>

#define IUPLUA_NO_GL 1
#define IUPLUA_NO_CD 1

#include <iup.h>
#include "iuplua.h"

// gl
#if !IUPLUA_NO_GL
#include "iupgl.h"
#include "iupglcontrols.h"
#include "iupluagl.h"
#include "iupluaglcontrols.h"
#endif
// im
//#include "iupluaim.h"
// tuio
#include "iupluatuio.h"
// web
#include "iupluaweb.h"
// scintilla
#include "iuplua_scintilla.h"

#if !IUPLUA_NO_CD
// controls
#include "iupcontrols.h"
#include "iupluacontrols.h"
// plot
#include "iup_plot.h"
#include "iuplua_plot.h"
#endif

int main(int argc, char** argv)
{
	sol::state lua;
	lua.open_libraries();

    auto L = lua.lua_state();
    iuplua_open(L);
    // gl
#if !IUPLUA_NO_GL
    iupgllua_open(L);
    iupglcontrolslua_open(L);
#endif
    // im
    //iupimlua_open(L);
    // tuio
    iuptuiolua_open(L);
    // web
    iupweblua_open(L);
    // scintilla
    iup_scintillalua_open(L);
#if !IUPLUA_NO_CD
    // controls
    iupcontrolslua_open(L);
    // plot
    iup_plotlua_open(L);
#endif

    sol::load_result script = lua.load("main.lua");

    // 检查文件是否加载成功
    if (!script.valid()) 
    {
        sol::error err = script;
        // 文件加载失败，打印错误消息
        std::cerr << "Error loading Lua script: " << err.what() << std::endl;
        return EXIT_FAILURE;
    }

    // 执行 Lua 脚本
    sol::protected_function_result result = script();

    // 检查执行结果
    if (!result.valid()) 
    {
        sol::error err = result;
        // 打印错误消息
        std::cerr << "Error executing Lua script: " << err.what() << std::endl;
        return EXIT_FAILURE;
    }

	return EXIT_SUCCESS;
}