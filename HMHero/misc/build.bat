@echo off
echo no more make system, bat is fast ?!

mkdir ..\build
pushd ..\build
pwd
cl /Zi ..\src\win32_handmade.cpp user32.lib
popd