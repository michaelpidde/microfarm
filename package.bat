@echo off

rmdir /S /Q release
rmdir /S /Q build
call build.bat
mkdir release
xcopy build\*.dll release
xcopy build\*.exe release
xcopy build\assets release

echo Done.