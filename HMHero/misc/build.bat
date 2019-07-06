@echo off
echo no more make system, bat is fast ?!


IF NOT EXIST ..\build mkdir ..\build
pushd ..\build
echo %cd%
cl /DHANDMADE_WIN32=1 /Zi ..\src\win32_handmade.cpp user32.lib gdi32.lib
popd
