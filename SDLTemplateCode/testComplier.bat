@echo off

REM 源文件路径所在
SET SourceCode="./SDLTemplateCodeTest2.cpp"

REM SDL2 接口路径所在 
SET SDLHeadFile="F:\SDL\SDL2\include"

REM SDL_Image 接口路径所在
SET SDLImageHeadFile="F:\SDL\SDL2_image\include"

REM SDL_Net 接口路径所在
SET SDL_NetHeadFile="F:\SDL\SDL2_net\include"

REM SDL_TTF 接口路径所在
SET SDLTTFHeadFile="F:\SDL\SDL2_ttf\include"

REM  SDL_GFX 接口路径所在
SET SDL_GFXHeadFile="F:\SDL\SDL_gfx\include"

REM PCG 随机数生成器的接口路径所在
SET PCGHeadFile="F:\pcg-c-basic\include"

REM FMT 日志库的接口路径所在
SET FMTHeadFile="F:\fmt\include\fmt"

REM SDL 的库文件，库名
SET SDL_LibPath="F:\SDL\SDL2\lib\x64"
SET SDL_LibName="SDL2"

REM SDL_Image 的库文件，库名
SET SDLImage_LibPath="F:\SDL\SDL2_image\lib\x64"
SET SDLImage_LibName="SDL2_image"

REM SDL_Net 的库文件，库名
SET SDLNet_LibPath="F:\SDL\SDL2_net\lib\x64"
SET SDLNet_LibName="SDL2_net"

REM SDL_GFX 的库文件，库名
SET SDLGFX_LibPath="F:\SDL\SDL_gfx\lib"
SET SDLGFX_LibName="SDL_gfx"

REM SDL_TTF 的库文件，库名
SET SDLTTF_LibPath="F:\SDL\SDL2_ttf\lib\x64"
SET SDLTTF_LibName="SDL2_ttf"

REM SDL 开发模板库的路径所在
SET SDLTemplate_LibPath="..\..\SDLTemplateCode\lib"
SET SDLTemplate_LibName="SDLTemplateCode"

REM PCG 随机数生成器的库文件路径所在
SET PCG_LibPath="F:\pcg-c-basic\lib\pcg_basic.o"

REM Cryptoc++ 加密库文件路径所在
SET Crypto_LibPath="../../lib/libcryptopp.a"

REM FMT 日志库的库文件路径所在
SET FMT_LibPath="F:\fmt\lib\libfmtd.a"

REM 可执行文件路径
SET ExecutionFilePath="../../bin/SDLTemplateCodeTest2.exe"

SET ALL_INCLUDE=-I%SDLHeadFile% -I%SDLImageHeadFile% -I%SDL_NetHeadFile% -I%SDL_GFXHeadFile% -I%SDLTTFHeadFile%
SET ALL_LIB=-L%SDLTemplate_LibPath% -L%SDL_LibPath% -L%SDLImage_LibPath% -L%SDLNet_LibPath% -L%SDLGFX_LibPath% -L%SDLTTF_LibPath%
SET ALL_LIBNAME=-l%SDLTemplate_LibName% -l%SDL_LibName% -l%SDLImage_LibName% -l%SDLNet_LibName% -l%SDLGFX_LibName% -l%SDLTTF_LibName%

g++ %SourceCode% %FMT_LibPath% %ALL_INCLUDE% %ALL_LIB% -o %ExecutionFilePath% -O3 -Wall %ALL_LIBNAME% -std=c++23