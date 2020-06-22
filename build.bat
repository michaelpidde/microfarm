@echo off

mkdir build
xcopy /E /D /I /Y assets build\assets
xcopy /Y /D _libs\SDL2-2.0.8\lib\x86\SDL2.dll build
xcopy /Y /D _libs\SDL2_image-2.0.3\lib\x86\SDL2_image.dll build
xcopy /Y /D _libs\SDL2_image-2.0.3\lib\x86\libpng16-16.dll build
xcopy /Y /D _libs\SDL2_image-2.0.3\lib\x86\zlib1.dll build

echo.
echo Compile started %TIME%
echo.

REM Build Win32API.dll
cl /nologo /LD /FC /Zi /Fo"build\\" /Fe"build\\Win32API.dll" win32api.c

REM Build MicroEngine.dll - depends on Win32API.lib
cl /nologo /LD /FC /Zi /Fo"build\\" /Fe"build\\MicroEngine.dll" micro_engine.c ^
/I _libs\SDL2-2.0.8\include ^
/I _libs\SDL2_image-2.0.3\include ^
/link /NOLOGO /out:build\MicroEngine.dll ^
/LIBPATH:_libs\SDL2-2.0.8\lib\x86 ^
/LIBPATH:_libs\SDL2_image-2.0.3\lib\x86 ^
/LIBPATH:build ^
SDL2.lib SDL2main.lib SDL2_image.lib Win32API.lib

REM Build microfarm.exe
REM Depends on MicroEngine.lib
cl /nologo /Zi /FC /Fo"build\\" /Fe"build\\microfarm.exe" microfarm.c ^
/link /NOLOGO /out:build\microfarm.exe ^
/LIBPATH:build ^
MicroEngine.lib

echo.
echo Compile finished %TIME%
echo.