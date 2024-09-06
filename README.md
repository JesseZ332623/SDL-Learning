# SDL（Simple DirectMedia Layer） 图形库学习笔记

## 01. 在窗口内渲染一张 BMP 格式的图片

详见：[Getting Image On Screen](https://github.com/JesseZ332623/SDL-Learning/tree/master/src/01)

## 02. 键盘事件控制

详见：[Key Press](https://github.com/JesseZ332623/SDL-Learning/tree/master/src/02)

## 03. 使用 `SDL_Image` 模块加载非 BMP 格式的图片

详见：[PNG Loaded](https://github.com/JesseZ332623/SDL-Learning/tree/master/src/03)

## 04. 图片纹理的加载与渲染

详见：[Texture Loading and Rendering](https://github.com/JesseZ332623/SDL-Learning/tree/master/src/04)

## 05. 几何图形的渲染

详见：[Geometry Rendering](https://github.com/JesseZ332623/SDL-Learning/tree/master/src/05)

## 06. 图片的位置与缩放

详见：[View Point](https://github.com/JesseZ332623/SDL-Learning/blob/master/src/06)

## 07. 色键的设置

详见：[Color Key](https://github.com/JesseZ332623/SDL-Learning/blob/master/src/07)

## 08. 剪辑渲染（Clip Render）

需要解决的问题：在一个平面内随机选取 n 个点，如何让它们彼此之间尽可能保持分散？

- 解法 1：简单的随机分布 + 调整：
  - 在平面上随机放置点
  - 每放置一个点，就需要检查它与其他以放置的点的距离是否大于预设好的距离
  - 否则就重新选取点，进行上一条检查，直到满足条件为止

缺点：算法复杂度 `O(n ^ 2)`，如果障碍物过多，会非常消耗时间。
  
- 解法 2：泊松圆盘采样：

详见：[Clip Render](https://github.com/JesseZ332623/SDL-Learning/blob/master/src/08)

## 课外. 使用 `SDL_Net` 模块搭建一个 TCP/IP 服务器

服务器代码详见：[Server](https://github.com/JesseZ332623/SDL-Learning/tree/master/src/extra/defs_server.cpp)

客户端代码详见：[Client](https://github.com/JesseZ332623/SDL-Learning/tree/master/src/extra/defs_client.cpp)

### Latest Update: 2024.09.03

### LICENCE: [MIT LICENCE](https://github.com/JesseZ332623/SDL-Learning/blob/master/LICENSE)
