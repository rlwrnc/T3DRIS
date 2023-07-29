@echo off

if not exist ..\build\ (mkdir ..\build)
set EXTERNAL_INCLUDE=..\external\include
set EXTERNAL_LIBRARY=C:\Users\rlwrnc\Projects\t3dris\external\libraries
cl main.c /D TED_RELEASE=0 /I %EXTERNAL_INCLUDE% /Zi /Fd..\build\vc140.pdb /Fo..\build\ user32.lib shell32.lib gdi32.lib glfw3.lib /link /LIBPATH:"%EXTERNAL_LIBRARY%" /NODEFAULTLIB:LIBCMTD /out:..\build\t3dris.exe
