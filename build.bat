@echo off

mkdir build
xcopy /E /D /I /Y Game\assets build\assets
REM TODO: ONLY COPY THESE ON NON-RELEASE BUILD...
xcopy /E /D /I /Y MicroEngine\assets build\assets
xcopy /Y /D MicroEngine\_libs\SDL2-2.0.8\lib\x86\SDL2.dll build
xcopy /Y /D MicroEngine\_libs\SDL2_image-2.0.3\lib\x86\SDL2_image.dll build
xcopy /Y /D MicroEngine\_libs\SDL2_image-2.0.3\lib\x86\libpng16-16.dll build
xcopy /Y /D MicroEngine\_libs\SDL2_image-2.0.3\lib\x86\zlib1.dll build

echo.
echo Compile started %TIME%


REM Build Win32API.dll
gcc -std=c99 -c Common\win32api.c -o build\win32api.o
gcc -std=c99 -shared -o build\Win32API.dll build\win32api.o -Wl,--out-implib,build\Win32API.lib


REM Build MicroEngine.dll - depends on Win32API.lib
gcc -std=c99 -c MicroEngine\micro_engine.c -o build\micro_engine.o ^
-IMicroEngine\_libs\SDL2-2.0.8\include ^
-IMicroEngine\_libs\SDL2_image-2.0.3\include ^
-D DEBUG=1

gcc -std=c99 -shared -o build\MicroEngine.dll build\micro_engine.o ^
-LMicroEngine\_libs\SDL2-2.0.8\lib\x86 ^
-LMicroEngine\_libs\SDL2_image-2.0.3\lib\x86 ^
-Lbuild\ ^
-lSDL2 -lSDL2main -lSDL2_image -lWin32API ^
-Wl,--out-implib,build\MicroEngine.lib


REM Build microfarm.exe
REM Depends on MicroEngine.lib
gcc -std=c99 -c Game\microfarm.c -o build\microfarm.o -D MCR_APP=1
gcc -std=c99 -o build\microfarm.exe build\microfarm.o -Lbuild -lMicroEngine

echo Compile finished %TIME%
echo.