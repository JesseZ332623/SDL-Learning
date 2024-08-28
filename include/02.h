#ifndef __02_H__
#define __02_H__

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 480

/**
 * @brief 该枚举类型标记了不同的键位操作。
*/
enum __KeyPressSurface
{
    KEY_PRESS_SURFACE_DEFAULT = 0,
    KEY_PRESS_SURFACE_UP,
    KEY_PRESS_SURFACE_DOWN,
    KEY_PRESS_SURFACE_LEFT,
    KEY_PRESS_SURFACE_RIGHT,
    KEY_PRESS_SURFACE_TOTAL     // 当前标记的所有键位，为 5
};

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
 * @brief 事件判断完成后，渲染当前画面。
*/
void rander(void);

/**
 * @brief 运行结束后销毁相关资源。
*/
void close(void);

#endif // __02_H__