set(lib_name curl)
set(target_name dep_${lib_name})

project(${lib_name})

include(../cmake/ConfigThirdparty.cmake)


add_library(${target_name} STATIC empty.cpp)


if(NOT WINDOWS)
  target_link_libraries(${target_name}
    "${platform_spec_path}/lib${lib_name}.a"
  )
else()
  target_link_libraries(${target_name}
    "${platform_spec_path}/lib${lib_name}_imp.lib"
  )
endif()


target_include_directories(${target_name}
    PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

if(WINDOWS)
add_custom_command(TARGET ${target_name} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${platform_spec_path}/lib${lib_name}.dll"
    ${OutBinDir}
)
endif()
