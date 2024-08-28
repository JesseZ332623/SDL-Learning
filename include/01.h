#ifndef __01_H__
#define __01_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 904

/**
 * @brief 该结构体用于管理各个事件的开关。
*/
struct EventsInfo
{
    bool ifQuit;
    /* ... */
};

/**
 * @brief 初始化 SDL，一个窗口，窗口平面信息和图像平面信息。
 * 
 * @return true     创建成功
 * @return false    创建失败
*/
bool init(void);

/**
 * @brief 加载图像信息。
*/
bool loadMedia(void);

/**
 * @brief 处理相关事件。
*/
void events(struct EventsInfo * __eventInfo);

/**
 * @brief 绘制。
*/
void rander(void);

/**
 * @brief 运行结束后销毁相关资源。
*/
void close(void);

#endif // __01_H__