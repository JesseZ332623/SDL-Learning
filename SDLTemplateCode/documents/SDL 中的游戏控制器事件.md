# SDL 中的游戏控制器事件

1. **SDL_CONTROLLERDEVICEADDED**：
   - 当一个新的游戏控制器被连接到系统时触发。
   - 事件包含控制器的索引 `which`，可以通过这个索引来打开新的控制器。

2. **SDL_CONTROLLERDEVICEREMOVED**：
   - 当一个已连接的游戏控制器从系统中断开时触发。
   - 同样包含控制器的索引 `which`，可以用来识别哪个控制器被断开了。

3. **SDL_CONTROLLERAXISMOTION**：
   - 当控制器的某个轴（如摇杆或模拟按钮）的值发生变化时触发。
   - 事件包含轴的索引 `axis` 和新的值 `value`。

4. **SDL_CONTROLLERBUTTONDOWN**：
   - 当控制器的一个按钮被按下时触发。
   - 事件包含按钮的索引 `button`。

5. **SDL_CONTROLLERBUTTONUP**：
   - 当控制器的一个按钮被释放时触发。
   - 事件包含按钮的索引 `button`。

6. **SDL_CONTROLLERTOUCHPADDOWN**：
   - 当控制器的触摸板被按下时触发（仅适用于支持触摸输入的控制器）。
   - 事件包含触摸点的 ID `touch_finger_id` 和触摸位置 `x` 和 `y`。

7. **SDL_CONTROLLERTOUCHPADMOTION**：
   - 当控制器的触摸板上的触摸点移动时触发（同上）。
   - 事件包含触摸点的 ID `touch_finger_id` 和触摸位置 `x` 和 `y`。

8. **SDL_CONTROLLERTOUCHPADUP**：
   - 当控制器的触摸板上的触摸点被抬起时触发（同上）。
   - 事件包含触摸点的 ID `touch_finger_id`。

9. **SDL_CONTROLLERSENSORUPDATE**：
   - 当控制器的传感器（如陀螺仪或加速度计）的数据更新时触发。
   - 事件包含传感器的类型 `sensor` 和新的数据。
