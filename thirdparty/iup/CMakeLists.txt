
set(lib_name iup)
set(target_name ${lib_name})

project(${lib_name})


include(../cmake/ConfigThirdparty.cmake)

add_library(${target_name} STATIC empty.cpp)

if(NOT WINDOWS)
else()
  # iup-3.31-Lua54_Win64_vc17_lib
  target_link_libraries(${target_name}
    ${platform_spec_path}/freetype6.lib
    ${platform_spec_path}/ftgl.lib
    ${platform_spec_path}/iup.lib
    ${platform_spec_path}/iupcd.lib
    ${platform_spec_path}/iupcontrols.lib
    ${platform_spec_path}/iupfiledlg.lib
    ${platform_spec_path}/iupgl.lib
    ${platform_spec_path}/iupglcontrols.lib
    ${platform_spec_path}/iupim.lib
    ${platform_spec_path}/iupimglib.lib
    ${platform_spec_path}/iupole.lib
    ${platform_spec_path}/iupstub.lib
    ${platform_spec_path}/iuptuio.lib
    ${platform_spec_path}/iupweb.lib
    ${platform_spec_path}/iup_mglplot.lib
    ${platform_spec_path}/iup_plot.lib
    ${platform_spec_path}/iup_scintilla.lib
    ${platform_spec_path}/zlib1.lib
    ${platform_spec_path}/lua54/iuplua54.lib
    ${platform_spec_path}/lua54/iupluacd54.lib
    ${platform_spec_path}/lua54/iupluacontrols54.lib
    ${platform_spec_path}/lua54/iupluafiledlg54.lib
    ${platform_spec_path}/lua54/iupluagl54.lib
    ${platform_spec_path}/lua54/iupluaglcontrols54.lib
    ${platform_spec_path}/lua54/iupluaim54.lib
    ${platform_spec_path}/lua54/iupluaimglib54.lib
    ${platform_spec_path}/lua54/iupluaole54.lib
    ${platform_spec_path}/lua54/iupluascripterdlg54.lib
    ${platform_spec_path}/lua54/iupluatuio54.lib
    ${platform_spec_path}/lua54/iupluaweb54.lib
    ${platform_spec_path}/lua54/iuplua_mglplot54.lib
    ${platform_spec_path}/lua54/iuplua_plot54.lib
    ${platform_spec_path}/lua54/iuplua_scintilla54.lib
    comctl32.lib
    comdlg32.lib
    gdi32.lib
    ws2_32.lib
    winmm.lib
    ole32.lib
    opengl32.lib
    glu32.lib
    gdiplus.lib
    imm32.lib
    uuid.lib
    Kernel32.lib
  )
endif()


set_target_properties(${target_name} PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/include"
)


add_custom_command(TARGET ${target_name} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${platform_spec_path}/freetype6.dll
    ${platform_spec_path}/ftgl.dll
    ${platform_spec_path}/iup.dll
    ${platform_spec_path}/iupcd.dll
    ${platform_spec_path}/iupcontrols.dll
    ${platform_spec_path}/iupfiledlg.dll
    ${platform_spec_path}/iupgl.dll
    ${platform_spec_path}/iupglcontrols.dll
    ${platform_spec_path}/iupim.dll
    ${platform_spec_path}/iupimglib.dll
    ${platform_spec_path}/iupole.dll
    ${platform_spec_path}/iuptuio.dll
    ${platform_spec_path}/iupweb.dll
    ${platform_spec_path}/iup_mglplot.dll
    ${platform_spec_path}/iup_plot.dll
    ${platform_spec_path}/iup_scintilla.dll
    ${platform_spec_path}/zlib1.dll
    ${platform_spec_path}/lua54/iuplua54.dll
    ${platform_spec_path}/lua54/iupluacd54.dll
    ${platform_spec_path}/lua54/iupluacontrols54.dll
    ${platform_spec_path}/lua54/iupluafiledlg54.dll
    ${platform_spec_path}/lua54/iupluagl54.dll
    ${platform_spec_path}/lua54/iupluaglcontrols54.dll
    ${platform_spec_path}/lua54/iupluaim54.dll
    ${platform_spec_path}/lua54/iupluaimglib54.dll
    ${platform_spec_path}/lua54/iupluaole54.dll
    ${platform_spec_path}/lua54/iupluascripterdlg54.dll
    ${platform_spec_path}/lua54/iupluatuio54.dll
    ${platform_spec_path}/lua54/iupluaweb54.dll
    ${platform_spec_path}/lua54/iuplua_mglplot54.dll
    ${platform_spec_path}/lua54/iuplua_plot54.dll
    ${platform_spec_path}/lua54/iuplua_scintilla54.dll
    ${OutBinDir}
)

