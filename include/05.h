#ifndef __04_H__
#define __04_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define SCREEN_WIDTH  1350
#define SCREEN_HEIGHT 760

//#define IMAGE_INIT_FLAGS ((IMG_INIT_PNG) | (IMG_INIT_JPG))

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
 * 本例是在窗口内绘制图形，不需要加载图像。
*/
#if false
/**
 * @brief 加载 __imagePath 指定的图像。
*/
bool loadMedia(const char * __imagePath, bool __isReload);
#endif

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

#endif // __04_H__