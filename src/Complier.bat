@echo off

REM 源文件路径所在
SET SourceCode="./*.c"

REM SDL2 接口路径所在 
SET SDLHeadFile="F:\SDL\SDL2\include"

REM SDL_Image 接口路径所在
SET SDLImageHeadFile="F:\SDL\SDL2_image\include"

REM PCG 随机数生成器的接口路径所在
SET PCGHeadFile="F:\pcg-c-basic\include"

REM SDL 的库文件，库名
SET SDL_LibPath="F:\SDL\SDL2\lib\x64"
SET SDL_LibName="SDL2"

REM SDL_Image 的库文件，库名
SET SDLImage_LibPath="F:\SDL\SDL2_image\lib\x64"
SET SDLImage_LibName="SDL2_image"

REM PCG 随机数生成器的库文件路径所在
SET PCG_LibPath="F:\pcg-c-basic\lib\pcg_basic.o"

REM 可执行文件路径
SET ExecutionFilePath="../../bin/05.exe"

gcc %SourceCode% %PCG_LibPath% -I%SDLHeadFile% -I%SDLImageHeadFile% -I%PCGHeadFile% -L%SDL_LibPath% -L%SDLImage_LibPath%  -o %ExecutionFilePath% -O3 -Wall -l%SDL_LibName% -l%SDLImage_LibName% -std=c23