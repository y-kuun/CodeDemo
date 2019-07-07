@echo off
echo no more make system, bat is fast ?!


IF NOT EXIST ..\build mkdir ..\build
pushd ..\build
echo %cd%
REM cl -MT -nologo -Gm- -GR- -EHa- -Od -Oi -WX -W4 -wd4201 -wd4100 -wd4189 -DHANDMADE_INTERNAL=1 -DHANDMADE_SLOW=1 -DHANDMADE_WIN32=1 -FC -Z7 -Fmwin32_handmade.map ..\src\win32_handmade.cpp /link -opt:ref -subsystem:windows,5.1 user32.lib gdi32.li
cl -MT -nologo -Gm- -GR- -EHa- -Od -Oi -WX -W4 -wd4201 -wd4100 -wd4189 -DHANDMADE_INTERNAL=1 -DHANDMADE_SLOW=1 -DHANDMADE_WIN32=1 -FC -Z7 -Fmwin32_handmade.map ..\src\win32_handmade.cpp /link -opt:ref user32.lib gdi32.lib
popd
