project(thirdparty)

option(GUI_WITH_UNZIP "Build with internal unzip support" ON)
option(GUI_WITH_CONVERTUTF "Build with internal ConvertUTF support" ON)
option(GUI_WITH_UCHARDET "Build with internal uchardet support" ON)
option(GUI_WITH_CURL "Build with internal curl support" OFF)
option(GUI_WITH_RUST_NET "Build with internal rust_net support" ON)


function(configure_target_outdir target)
    set_target_properties(${target} PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
        FOLDER "ThirdParty"
    )
endfunction()

add_library(thirdparty STATIC empty.cpp)

add_subdirectory(zlib)
target_link_libraries(thirdparty
    dep_zlib
)


add_subdirectory(iconv)
target_link_libraries(thirdparty iconv)
configure_target_outdir(iconv)



if(GUI_WITH_CURL)
add_subdirectory(curl)
target_link_libraries(thirdparty
    dep_curl
)
endif(GUI_WITH_CURL)

if(GUI_WITH_RUST_NET)
    add_subdirectory(rust_net)
    target_link_libraries(thirdparty dep_rust_net)
endif(GUI_WITH_RUST_NET)

add_subdirectory(iup)
target_link_libraries(thirdparty iup)
configure_target_outdir(iup)

if(GUI_WITH_CONVERTUTF)
    add_subdirectory(ConvertUTF)
    target_link_libraries(thirdparty ConvertUTF)
    configure_target_outdir(ConvertUTF)
endif(GUI_WITH_CONVERTUTF)


if(GUI_WITH_UCHARDET)
    add_subdirectory(uchardet)
    target_link_libraries(thirdparty uchardet)
    configure_target_outdir(uchardet)
endif(GUI_WITH_UCHARDET)

set(LUA_ENGINE lua54)
add_subdirectory(lua/${LUA_ENGINE})
add_subdirectory(lua/lua_cjson)
add_subdirectory(lua/luasocket)
target_link_libraries(thirdparty ${LUA_ENGINE} lua_cjson luasocket)
target_include_directories(thirdparty PUBLIC ${CMAKE_CURRENT_LIST_DIR}/lua)


# unzip depend on zlib
if(GUI_WITH_UNZIP)
    add_subdirectory(unzip)
    target_link_libraries(thirdparty unzip)
    configure_target_outdir(unzip)
endif(GUI_WITH_UNZIP)


# put "thirdparty" into thirdparty folder, too
configure_target_outdir(thirdparty)
