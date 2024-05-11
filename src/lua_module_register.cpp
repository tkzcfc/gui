#include "lua_module_register.h"

//////////////////////////////////////////////////////////////// IUP ////////////////////////////////////////////////////////////////


#define IUPLUA_IMGLIB 1
#define IUPLUA_TUIO 1
#define IUPLUA_WEB 1
#define IUPLUA_SCINTILLA 1
#define USE_LUAGL 0

#include <iup.h>
#include "iuplua.h"

#if !IUPLUA_NO_GL
// gl
#include "iupgl.h"
#include "iupglcontrols.h"
#include "iupluagl.h"
#include "iupluaglcontrols.h"
#if USE_LUAGL
#include "luagl.h"
#endif
#endif

#if !IUPLUA_NO_IM
// im
#include "iupluaim.h"
#include "im.h"
#include "im_image.h"
#include "imlua.h"
#endif

#if IUPLUA_TUIO
#include "iupluatuio.h"
#endif

#if IUPLUA_WEB
#include "iupluaweb.h"
#endif

#if IUPLUA_SCINTILLA
#include "iuplua_scintilla.h"
#endif

#if !IUPLUA_NO_CD
#include "iupcontrols.h"
#include "iupluacontrols.h"
#include "iup_plot.h"
#include "iuplua_plot.h"
#include <cd.h>
#include <cdlua.h>
#include <cdluaiup.h>
#endif

#if !IUPLUA_NO_IM
#if !IUPLUA_NO_CD
#include <cdluaim.h>
#endif
#endif

#if IUPLUA_IMGLIB
extern "C" int luaopen_iupluaimglib(lua_State * L);
#endif

void lua_register_iup(lua_State* L)
{
    iuplua_open(L);

#if IUPLUA_IMGLIB
    luaopen_iupluaimglib(L);
#endif

#if IUPLUA_TUIO
    // tuio
    iuptuiolua_open(L);
#endif
#if IUPLUA_WEB
    iupweblua_open(L);
#endif
#if IUPLUA_SCINTILLA
    iup_scintillalua_open(L);
#endif

#if !IUPLUA_NO_GL
    iupgllua_open(L);
    iupglcontrolslua_open(L);
#if USE_LUAGL
    luaopen_luagl(L);
#endif
#endif
#if !IUPLUA_NO_CD
    iupcontrolslua_open(L);
    iup_plotlua_open(L);
    cdlua_open(L);
    cdluaiup_open(L);
    cdInitContextPlus();
#endif

#if !IUPLUA_NO_IM
    // im
    iupimlua_open(L);
    imlua_open(L);
    imlua_open_process(L);
#endif

#if !IUPLUA_NO_IM
#if !IUPLUA_NO_CD
    cdluaim_open(L);
#endif
#endif
}

//////////////////////////////////////////////////////////////// cjson ////////////////////////////////////////////////////////////////

extern "C" int luaopen_cjson(lua_State* l);

//////////////////////////////////////////////////////////////// luasocket ////////////////////////////////////////////////////////////////

extern "C" {
#include "luasocket/luasocket.h"
#include "luasocket/mime.h"
#include "luasocket/luasocket_scripts.h"
}  // extern "C"

//////////////////////////////////////////////////////////////// register ////////////////////////////////////////////////////////////////

#include "Core/tolua_core.h"
#include "Crypto/tolua_crypto.h"
#include "Logger/tolua_logger.h"
#include "Network/tolua_network.h"
#include "Platform/tolua_platform.h"
#include "Utils/tolua_utils.h"

void lua_module_register(sol::state& lua)
{
    lua.open_libraries();
    tolua_core(lua);
    tolua_crypto(lua);
    tolua_logger(lua);
    tolua_network(lua);
    tolua_platform(lua);
    tolua_utils(lua);

    auto L = lua.lua_state();

    lua_register_iup(L);

    static luaL_Reg lua_exts[] =
    {
        {"cjson", luaopen_cjson},
        {"socket.core", luaopen_socket_core},
        {"mime.core", luaopen_mime_core},
        {NULL, NULL}
    };
    lua_getglobal(L, "package");
    lua_getfield(L, -1, "preload");
    auto lib = lua_exts;
    for (; lib->func; ++lib)
    {
        lua_pushcfunction(L, lib->func);
        lua_setfield(L, -2, lib->name);
    }
    lua_pop(L, 2);
}

