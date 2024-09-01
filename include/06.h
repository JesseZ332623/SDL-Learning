#ifndef __06_H__
#define __06_H__

/**
 * @file   06.h
 * @author Jesse 
 * @date   2024-09-01
 * 
 * @copyright Copyright (c) 2024
 * 
 * @brief 本节主要使用 SDL_RenderSetViewport() 接口，去设置图片渲染的位置和缩放。
*/

#include <stdbool.h>
#include <corecrt.h>

#define SCREEN_WIDTH  1000
#define SCREEN_HEIGHT 600

/**
 * @brief 图片标志位
*/
#define IMAGE_INIT_FLAGS ((IMG_INIT_PNG) | (IMG_INIT_JPG))

/**
 * @brief 该结构体用于管理各个事件的开关。
*/
struct EventsInfo
{
    bool ifQuit;
    /* ... */
};

/**
 * @brief 输出分割线。
*/
void printSplitLine(size_t __len, const char __style);

/**
 * @brief 初始化 SDL，创建一个窗口，一个渲染器，以及 Image 模块的初始化。
 * 
 * @brief - 初始化顺序为：
 * 
 * @brief 0. 随机数引擎初始化
 * @brief 1. SDL 初始化
 * @brief 2. 窗口创建
 * @brief 3. 渲染器创建
 * @brief 4. 纹理创建
 * 
 * @return true     全部初始化成功
 * @return false    全部初始化失败
*/
bool init(void);

/**
 * @brief 加载 __imagePath 指定的图像。
*/
bool loadMedia(const char * __imagePath);

/**
 * @brief 事件控制
*/
void eventControl(struct EventsInfo * __info);

/**
 * @brief 绘制操作
*/
void rander(void);

/**
 * @brief 运行结束前释放所有加载的资源。 
*/
void freeResource(void);

#endif // __06_H__