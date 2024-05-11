set(lib_name lua_md5)
set(target_name ${lib_name})

project(${lib_name})

set(${target_name}_src
  compat-5.2.c
  des56.c
  ldes56.c
  md5.c
  md5lib.c
  md5.def
)

add_library(${target_name} STATIC
  ${${target_name}_src}
)

target_include_directories(${target_name} PRIVATE ../${LUA_ENGINE})
#target_include_directories(${target_name} PUBLIC .)

set_target_properties(${target_name}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    FOLDER "ThirdParty"
)
