@echo off

REM 源文件路径所在
SET SourceCode="./*.cpp"

REM SDL2 接口路径所在 
SET SDLHeadFile="F:\SDL\SDL2\include"

REM SDL_Image 接口路径所在
SET SDLImageHeadFile="F:\SDL\SDL2_image\include"

REM SDL_Net 接口路径所在
SET SDL_NetHeadFile="F:\SDL\SDL2_net\include"

REM SDL 的库文件，库名
SET SDL_LibPath="F:\SDL\SDL2\lib\x64"
SET SDL_LibName="SDL2"

REM SDL_Image 的库文件，库名
SET SDLImage_LibPath="F:\SDL\SDL2_image\lib\x64"
SET SDLImage_LibName="SDL2_image"

REM SDL_Net 的库文件，库名
SET SDLNet_LibPath="F:\SDL\SDL2_net\lib\x64"
SET SDLNet_LibName="SDL2_net"

REM 可执行文件路径
SET ExecutionFilePath="../lib/SDLTemplateCode.dll"

SET ALL_INCLUDE=-I%SDLHeadFile% -I%SDLImageHeadFile%
SET ALL_LIB=-L%SDL_LibPath% -L%SDLImage_LibPath%
SET ALL_LIBNAME=-l%SDL_LibName% -l%SDLImage_LibName%

g++ -fPIC -shared %SourceCode% %ALL_INCLUDE% %ALL_LIB% -o %ExecutionFilePath% %ALL_LIBNAME% -O3 -Wall -std=c++23