#include "tolua_core.h"
#include "spdlog/spdlog.h"
#include "GScheduler.h"

static int schedulerGlobalCount = 0;

inline std::string schduer_key()
{
    return fmt::format("lua_schduer_{}", schedulerGlobalCount);
}

void tolua_core(sol::state& lua)
{
    auto core = lua["core"].get_or_create<sol::table>();

    core.set_function("setTimeout", [](sol::main_protected_function callback, float delay) -> auto {
        auto key = schduer_key();
        GScheduler::getInstance()->scheduleGlobalOnce(callback.as<duSchedulerFunc>(), delay, key);
        return key;
    });

    core.set_function("setInterval", [](sol::main_protected_function callback, float delay) -> auto {
        auto key = schduer_key();
        GScheduler::getInstance()->scheduleGlobal(callback.as<duSchedulerFunc>(), delay, false, key);
        return key;
    });

    core.set_function("clearInterval", [](std::string key) -> void {
        GScheduler::getInstance()->unScheduleGlobal(key);
    });
}
