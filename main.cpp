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

    // ����ļ��Ƿ���سɹ�
    if (!script.valid()) 
    {
        sol::error err = script;
        // �ļ�����ʧ�ܣ���ӡ������Ϣ
        std::cerr << "Error loading Lua script: " << err.what() << std::endl;
        return EXIT_FAILURE;
    }

    // ִ�� Lua �ű�
    sol::protected_function_result result = script();

    // ���ִ�н��
    if (!result.valid()) 
    {
        sol::error err = result;
        // ��ӡ������Ϣ
        std::cerr << "Error executing Lua script: " << err.what() << std::endl;
        return EXIT_FAILURE;
    }

	return EXIT_SUCCESS;
}