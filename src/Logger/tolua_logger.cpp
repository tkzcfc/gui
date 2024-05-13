#include "tolua_logger.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"


static int get_string_for_print(lua_State* L, std::string* out)
{
    int n = lua_gettop(L); /* number of arguments */
    int i;

    lua_getglobal(L, "tostring");
    for (i = 1; i <= n; i++)
    {
        const char* s;
        lua_pushvalue(L, -1); /* function to be called */
        lua_pushvalue(L, i);  /* value to print */
        lua_call(L, 1, 1);
        size_t sz;
        s = lua_tolstring(L, -1, &sz); /* get result */
        if (s == NULL)
            return luaL_error(L, "'tostring' must return a string to 'print'");
        if (i > 1)
            out->append("\t");
        out->append(s, sz);
        lua_pop(L, 1); /* pop result */
    }
    return 0;
}

void tolua_logger(sol::state& lua)
{
    auto utils = lua["logger"].get_or_create<sol::table>();

    spdlog::stdout_color_mt("lua");


    utils.set_function("setLevel", [](int level)->void {
        if (level >= 0 && level < spdlog::level::level_enum::n_levels)
        {
            spdlog::get("lua")->set_level((spdlog::level::level_enum)level);
        }
    });

    utils.set_function("setRotating", [](const std::string& logFileName, size_t maxFileSize, size_t maxFiles)->void {
        spdlog::drop("lua");
        auto logger = spdlog::rotating_logger_mt("lua", logFileName, maxFileSize, maxFiles);
        logger->flush_on(spdlog::level::level_enum::err);
    });

    utils.set_function("trace", [](lua_State* L)->int {
        std::string t;
        get_string_for_print(L, &t);
        spdlog::get("lua")->trace("{}", t);
        return 0;
    });
    
    utils.set_function("debug", [](lua_State* L)->int {
        std::string t;
        get_string_for_print(L, &t);
        spdlog::get("lua")->debug("{}", t);
        return 0;
    });

    utils.set_function("info", [](lua_State* L)->int {
        std::string t;
        get_string_for_print(L, &t);
        spdlog::get("lua")->info("{}", t);
        return 0;
    });

    utils.set_function("warn", [](lua_State* L)->int {
        std::string t;
        get_string_for_print(L, &t);
        spdlog::get("lua")->warn("{}", t);
        return 0;
    });

    utils.set_function("error", [](lua_State* L)->int {
        std::string t;
        get_string_for_print(L, &t);
        spdlog::get("lua")->error("{}", t);
        return 0;
    });

    utils.set_function("critical", [](lua_State* L)->int {
        std::string t;
        get_string_for_print(L, &t);
        spdlog::get("lua")->critical("{}", t);
        return 0;
    });
}
