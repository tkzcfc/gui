
set(lib_name iup)
set(target_name ${lib_name})

project(${lib_name})


option(IUP_WITH_IM "Build with iup im support" OFF)
option(IUP_WITH_CD "Build with iup cd support" OFF)
option(IUP_WITH_GL "Build with iup gl support" OFF)
option(IUP_WITH_SCINTILLA "Build with iup scintilla support" OFF)
option(IUP_WITH_WEB "Build with iup web support" OFF)
option(IUP_WITH_IMGLIB "Build with iup img support" ON)
option(IUP_WITH_TUIO "Build with iup tuio support" OFF)
option(IUP_WITH_OLE "Build with iup ole support" OFF)



include(../cmake/ConfigThirdparty.cmake)

add_library(${target_name} STATIC empty.cpp)


if(IUP_WITH_IM)
  target_compile_definitions(${target_name} PUBLIC IUP_USE_IM=1)
else()
  target_compile_definitions(${target_name} PUBLIC IUP_USE_IM=0)
endif(IUP_WITH_IM)

if(IUP_WITH_CD)
  target_compile_definitions(${target_name} PUBLIC IUP_USE_CD=1)
else()
  target_compile_definitions(${target_name} PUBLIC IUP_USE_CD=0)
endif(IUP_WITH_CD)

if(IUP_WITH_GL)
  target_compile_definitions(${target_name} PUBLIC IUP_USE_GL=1)
else()
  target_compile_definitions(${target_name} PUBLIC IUP_USE_GL=0)
endif(IUP_WITH_GL)

if(IUP_WITH_SCINTILLA)
  target_compile_definitions(${target_name} PUBLIC IUP_USE_SCINTILLA=1)
else()
  target_compile_definitions(${target_name} PUBLIC IUP_USE_SCINTILLA=0)
endif(IUP_WITH_SCINTILLA)

if(IUP_WITH_WEB)
  target_compile_definitions(${target_name} PUBLIC IUP_USE_WEB=1)
else()
  target_compile_definitions(${target_name} PUBLIC IUP_USE_WEB=0)
endif(IUP_WITH_WEB)

if(IUP_WITH_IMGLIB)
  target_compile_definitions(${target_name} PUBLIC IUP_USE_IMGLIB=1)
else()
  target_compile_definitions(${target_name} PUBLIC IUP_USE_IMGLIB=0)
endif(IUP_WITH_IMGLIB)

if(IUP_WITH_TUIO)
  target_compile_definitions(${target_name} PUBLIC IUP_USE_TUIO=1)
else()
  target_compile_definitions(${target_name} PUBLIC IUP_USE_TUIO=0)
endif(IUP_WITH_TUIO)


if(WINDOWS)
  # iup-3.31-Lua54_Win64_vc17_lib
  target_link_libraries(${target_name}
    ${platform_spec_path}/iup.lib
    ${platform_spec_path}/lua54/iuplua54.lib
  )

  # 暂时不知道哪个模块依赖的
  #${platform_spec_path}/iupstub.lib
  #${platform_spec_path}/lua54/iupluafiledlg54.lib
  #${platform_spec_path}/lua54/iupluascripterdlg54.lib


  if(IUP_WITH_SCINTILLA)
  target_link_libraries(${target_name}
    ${platform_spec_path}/iup_scintilla.lib
    ${platform_spec_path}/lua54/iuplua_scintilla54.lib
  )
  endif(IUP_WITH_SCINTILLA)

  if(IUP_WITH_WEB)
  target_link_libraries(${target_name}
    ${platform_spec_path}/iupweb.lib
    ${platform_spec_path}/lua54/iupluaweb54.lib
  )
  endif(IUP_WITH_WEB)

  if(IUP_WITH_IMGLIB)
  target_link_libraries(${target_name}
    ${platform_spec_path}/iupimglib.lib
    ${platform_spec_path}/lua54/iupluaimglib54.lib
  )
  endif(IUP_WITH_IMGLIB)

  if(IUP_WITH_TUIO)
  target_link_libraries(${target_name}
    ${platform_spec_path}/iuptuio.lib
    ${platform_spec_path}/lua54/iupluatuio54.lib
  )
  endif(IUP_WITH_TUIO)

  if(IUP_WITH_OLE)
  target_link_libraries(${target_name}
    ${platform_spec_path}/iupole.lib
    ${platform_spec_path}/lua54/iupluaole54.lib
  )
  endif(IUP_WITH_OLE)

  if(IUP_WITH_GL)
    target_link_libraries(${target_name}
      ${platform_spec_path}/freetype6.lib
      ${platform_spec_path}/ftgl.lib
      ${platform_spec_path}/iupcd.lib
      ${platform_spec_path}/iupcontrols.lib
      ${platform_spec_path}/iupfiledlg.lib
      ${platform_spec_path}/iupgl.lib
      ${platform_spec_path}/iupglcontrols.lib
      ${platform_spec_path}/iup_mglplot.lib
      ${platform_spec_path}/lua54/iupluagl54.lib
      ${platform_spec_path}/lua54/iupluaglcontrols54.lib
      ${platform_spec_path}/lua54/iuplua_mglplot54.lib
    )
  endif(IUP_WITH_GL)

  if(IUP_WITH_IM)
    target_link_libraries(${target_name}
        ${platform_spec_path}/imlua_avi54.lib
        ${platform_spec_path}/imlua_capture54.lib
        ${platform_spec_path}/imlua_fftw354.lib
        ${platform_spec_path}/imlua_jp254.lib
        ${platform_spec_path}/imlua_process_omp54.lib
        ${platform_spec_path}/imlua_process54.lib
        ${platform_spec_path}/imlua_wmv54.lib
        ${platform_spec_path}/imlua54.lib
        ${platform_spec_path}/im.lib
        ${platform_spec_path}/im_avi.lib
        ${platform_spec_path}/im_capture.lib
        ${platform_spec_path}/im_fftw3.lib
        ${platform_spec_path}/im_jp2.lib
        ${platform_spec_path}/im_process.lib
        ${platform_spec_path}/im_process_omp.lib
        ${platform_spec_path}/im_wmv.lib
        ${platform_spec_path}/iupim.lib
        ${platform_spec_path}/lua54/iupluaim54.lib
      )
  endif(IUP_WITH_IM)

  if(IUP_WITH_CD)
    target_link_libraries(${target_name}
      ${platform_spec_path}/cd.lib
      ${platform_spec_path}/cdcairo.lib
      ${platform_spec_path}/cdcontextplus.lib
      ${platform_spec_path}/cddirect2d.lib
      ${platform_spec_path}/cdgl.lib
      ${platform_spec_path}/cdim.lib
      ${platform_spec_path}/cdpdf.lib
      ${platform_spec_path}/pdflib.lib
      ${platform_spec_path}/iup_plot.lib
      ${platform_spec_path}/lua54/iupluacd54.lib
      ${platform_spec_path}/Lua54/cdlua54.lib
      ${platform_spec_path}/Lua54/cdluacairo54.lib
      ${platform_spec_path}/Lua54/cdluacontextplus54.lib
      ${platform_spec_path}/Lua54/cdluadirect2d54.lib
      ${platform_spec_path}/Lua54/cdluagl54.lib
      ${platform_spec_path}/Lua54/cdluaim54.lib
      ${platform_spec_path}/Lua54/cdluapdf54.lib
      ${platform_spec_path}/lua54/iupluacontrols54.lib
      ${platform_spec_path}/lua54/iuplua_plot54.lib
    )
  endif(IUP_WITH_CD)

  if(IUP_WITH_IM OR IUP_WITH_CD)
    target_link_libraries(${target_name}
      ${platform_spec_path}/zlib1.lib
    )
  endif()


  if(IUP_WITH_GL OR IUP_WITH_CD)
    target_link_libraries(${target_name}
      ${platform_spec_path}/freetype6.lib
    )
  endif()

else()
endif()


target_include_directories(${target_name}
    PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}/include" 
    "${CMAKE_CURRENT_SOURCE_DIR}/im/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/cd/include"
)


if(WINDOWS)
  add_custom_command(TARGET ${target_name} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
      ${platform_spec_path}/iup.dll
      ${platform_spec_path}/lua54/iuplua54.dll

      ${OutBinDir}
  )

  # 暂时不知道哪个模块依赖的
  # ${platform_spec_path}/lua54/iupluafiledlg54.dll
  # ${platform_spec_path}/lua54/iupluascripterdlg54.dll

  if(IUP_WITH_SCINTILLA)
    add_custom_command(TARGET ${target_name} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${platform_spec_path}/iup_scintilla.dll
        ${platform_spec_path}/lua54/iuplua_scintilla54.dll

        ${OutBinDir}
    )
  endif(IUP_WITH_SCINTILLA)

  if(IUP_WITH_WEB)
    add_custom_command(TARGET ${target_name} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${platform_spec_path}/iupweb.dll
        ${platform_spec_path}/lua54/iupluaweb54.dll

        ${OutBinDir}
    )
  endif(IUP_WITH_WEB)

  if(IUP_WITH_IMGLIB)
    add_custom_command(TARGET ${target_name} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${platform_spec_path}/iupimglib.dll
        ${platform_spec_path}/lua54/iupluaimglib54.dll

        ${OutBinDir}
    )
  endif(IUP_WITH_IMGLIB)

  if(IUP_WITH_TUIO)
    add_custom_command(TARGET ${target_name} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${platform_spec_path}/iuptuio.dll
        ${platform_spec_path}/lua54/iupluatuio54.dll

        ${OutBinDir}
    )
  endif(IUP_WITH_TUIO)

  if(IUP_WITH_GL)
    add_custom_command(TARGET ${target_name} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${platform_spec_path}/ftgl.dll
        ${platform_spec_path}/iupcd.dll
        ${platform_spec_path}/iupcontrols.dll
        ${platform_spec_path}/iupfiledlg.dll
        ${platform_spec_path}/iupgl.dll
        ${platform_spec_path}/iupglcontrols.dll
        ${platform_spec_path}/iup_mglplot.dll
        ${platform_spec_path}/lua54/iupluagl54.dll
        ${platform_spec_path}/lua54/iupluaglcontrols54.dll
        ${platform_spec_path}/lua54/iuplua_mglplot54.dll

        ${OutBinDir}
    )
  endif(IUP_WITH_GL)

  if(IUP_WITH_OLE)
    add_custom_command(TARGET ${target_name} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${platform_spec_path}/iupole.dll
        ${platform_spec_path}/lua54/iupluaole54.dll

        ${OutBinDir}
    )
  endif(IUP_WITH_OLE)

  if(IUP_WITH_IM)
    add_custom_command(TARGET ${target_name} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${platform_spec_path}/imlua_avi54.dll
        ${platform_spec_path}/imlua_capture54.dll
        ${platform_spec_path}/imlua_fftw354.dll
        ${platform_spec_path}/imlua_jp254.dll
        ${platform_spec_path}/imlua_process_omp54.dll
        ${platform_spec_path}/imlua_process54.dll
        ${platform_spec_path}/imlua_wmv54.dll
        ${platform_spec_path}/imlua54.dll
        ${platform_spec_path}/im.dll
        ${platform_spec_path}/im_avi.dll
        ${platform_spec_path}/im_capture.dll
        ${platform_spec_path}/im_fftw3.dll
        ${platform_spec_path}/im_jp2.dll
        ${platform_spec_path}/im_process.dll
        ${platform_spec_path}/im_process_omp.dll
        ${platform_spec_path}/im_wmv.dll
        ${platform_spec_path}/iupim.dll
        ${platform_spec_path}/lua54/iupluaim54.dll

        ${OutBinDir}
    )
  endif(IUP_WITH_IM)

  if(IUP_WITH_CD)
    add_custom_command(TARGET ${target_name} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${platform_spec_path}/cd.dll
        ${platform_spec_path}/cdcairo.dll
        ${platform_spec_path}/cdcontextplus.dll
        ${platform_spec_path}/cddirect2d.dll
        ${platform_spec_path}/cdgl.dll
        ${platform_spec_path}/cdim.dll
        ${platform_spec_path}/cdpdf.dll
        ${platform_spec_path}/pdflib.dll
        ${platform_spec_path}/iup_plot.dll
        ${platform_spec_path}/lua54/iupluacd54.dll
        ${platform_spec_path}/Lua54/cdlua54.dll
        ${platform_spec_path}/Lua54/cdluacairo54.dll
        ${platform_spec_path}/Lua54/cdluacontextplus54.dll
        ${platform_spec_path}/Lua54/cdluadirect2d54.dll
        ${platform_spec_path}/Lua54/cdluagl54.dll
        ${platform_spec_path}/Lua54/cdluaim54.dll
        ${platform_spec_path}/Lua54/cdluapdf54.dll
        ${platform_spec_path}/lua54/iupluacontrols54.dll
        ${platform_spec_path}/lua54/iuplua_plot54.dll
        
        ${OutBinDir}
    )
  endif(IUP_WITH_CD)

  if(IUP_WITH_IM OR IUP_WITH_CD)
    add_custom_command(TARGET ${target_name} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${platform_spec_path}/zlib1.dll
        
        ${OutBinDir}
    )
  endif()

  if(IUP_WITH_GL OR IUP_WITH_CD)
    add_custom_command(TARGET ${target_name} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${platform_spec_path}/freetype6.dll
        
        ${OutBinDir}
    )
  endif()

endif()

