@echo off

REM 源文件路径所在
SET SourceCode="./extra_server.cpp" "./defs_server.cpp"

REM SDL2 接口路径所在 
SET SDLHeadFile="F:\SDL\SDL2\include"

REM SDL_Image 接口路径所在
SET SDLImageHeadFile="F:\SDL\SDL2_image\include"

REM SDL_Net 接口路径所在
SET SDL_NetHeadFile="F:\SDL\SDL2_net\include"

REM PCG 随机数生成器的接口路径所在
SET PCGHeadFile="F:\pcg-c-basic\include"

REM SDL 的库文件，库名
SET SDL_LibPath="F:\SDL\SDL2\lib\x64"
SET SDL_LibName="SDL2"

REM SDL_Image 的库文件，库名
SET SDLImage_LibPath="F:\SDL\SDL2_image\lib\x64"
SET SDLImage_LibName="SDL2_image"

REM SDL_Net 的库文件，库名
SET SDLNet_LibPath="F:\SDL\SDL2_net\lib\x64"
SET SDLNet_LibName="SDL2_net"

REM PCG 随机数生成器的库文件路径所在
SET PCG_LibPath="F:\pcg-c-basic\lib\pcg_basic.o"

REM 可执行文件路径
SET ExecutionFilePath="../../bin/extra_server.exe"

SET ALL_INCLUDE=-I%SDLHeadFile% -I%SDLImageHeadFile% -I%PCGHeadFile% -I%SDL_NetHeadFile%
SET ALL_LIB=-L%SDL_LibPath% -L%SDLImage_LibPath% -L%SDLNet_LibPath%
SET ALL_LIBNAME=-l%SDL_LibName% -l%SDLImage_LibName% -l%SDLNet_LibName%

g++ %SourceCode% %PCG_LibPath% %ALL_INCLUDE% %ALL_LIB% -o %ExecutionFilePath% -O3 -Wall %ALL_LIBNAME% -std=c++23