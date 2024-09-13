#ifndef __FRAME_CONTROL_H__
#define __FRAME_CONTROL_H__

#include "SDL_stdinc.h"

/**
 * @brief 维持帧数在一个固定值。
 * 
 * @param __startFrameTick       某一帧开始渲染前的时间刻
 * @param __frameRenderTime      维持一定帧数渲染一帧所需要的标准时间
*/
void frameControl(Uint64 & __startFrameTick, const Uint64 __frameRenderTime);

#endif // __FRAME_CONTROL_H__