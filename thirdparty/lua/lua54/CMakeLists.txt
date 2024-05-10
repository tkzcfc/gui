
set (AX_LUA_VERSION "5.4.4")

set(LUA_SRC_PATH ${CMAKE_CURRENT_LIST_DIR})

set(lib_name ${LUA_ENGINE})
set(target_name ${lib_name})

project(${lib_name})

aux_source_directory(${LUA_SRC_PATH} LUA_CORE)
list(REMOVE_ITEM LUA_CORE ${LUA_SRC_PATH}/lua.c ${LUA_SRC_PATH}/luac.c)


set(LUA_BUILD_AS_DLL TRUE)

if(WIN32)
  if(LUA_BUILD_AS_DLL)
    add_library(${target_name} SHARED ${LUA_CORE} ${LUA_SRC_PATH}/plainlua.def)
  else()
    add_library(${target_name} ${LUA_CORE})
  endif()
else()
    add_library(${target_name} ${LUA_CORE})
endif()

target_compile_definitions(${target_name} 
    PRIVATE _CRT_SECURE_NO_WARNINGS 
    PRIVATE _WINSOCK_DEPRECATED_NO_WARNINGS
    )

target_compile_definitions(${target_name} 
    PUBLIC LUA_COMPAT_APIINTCASTS=1
    PUBLIC LUA_COMPAT_MATHLIB=1
  )

if(LUA_BUILD_AS_DLL)
    target_compile_definitions(${target_name} PUBLIC LUA_BUILD_AS_DLL=1)
endif()

set_target_properties(${target_name} PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${LUA_SRC_PATH}"
)

set_target_properties(${target_name}
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY                "${OutBinDir}"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG          "${OutBinDir}"
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${OutBinDir}"
    RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL     "${OutBinDir}"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE        "${OutBinDir}"
    VS_DEBUGGER_WORKING_DIRECTORY           ${OutBinDir}
    FOLDER "ThirdParty"
)