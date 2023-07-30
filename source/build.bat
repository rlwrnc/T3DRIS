@echo off

set EXTERNAL_INCLUDE=..\external\include
set EXTERNAL_LIBRARY=..\external\libraries
set out=..\build
set source=..\source

set defines=/D TED_RELEASE=0

set warning_ignores=/wd4100
set warning_ignores=%warning_ignores% /wd4189

REM statically link with CRT
set compiler_options=/MT
REM set warning level 4
set compiler_options=%compiler_options% /W4
REM treat warnings are errors
set compiler_options=/WX
REM add ignored warnings
set compiler_options=%compiler_options% %warning_ignores%
REM no microsoft banner
set compiler_options=%compiler_options% /nologo

REM compile debugging information
set debug_options=/Z7

set static_libs=user32.lib
set static_libs=%static_libs% shell32.lib
set static_libs=%static_libs% gdi32.lib
set static_libs=%static_libs% glfw3.lib

set linker_options=/libpath:"%EXTERNAL_LIBRARY%"
set linker_options=%linker_options% /nodefaultlib:LIBCMTD
set linker_options=%linker_options% /opt:ref

set dll_exports=/EXPORT:game_update_and_render
set dll_exports=%dll_exports% /EXPORT:game_initialize

if not exist %out%\ (mkdir %out%)
pushd %out%
del *.pdb > NUL
cl -Od %source%\t3dris.c %compiler_options% /Fmgame.map %defines% /I %EXTERNAL_INCLUDE% %debug_options% /Fogame.obj /LD /link /pdb:game%random%.pdb %linker_options% /out:game.dll %dll_exports%
cl -Od %source%\main.c %compiler_options% /FmT3DRIS.map %defines% /I %EXTERNAL_INCLUDE% %debug_options% /FoT3DRIS.obj %static_libs% /link %linker_options% /out:T3DRIS.exe
popd
