# Tetris （代码有注释）
C++控制台版俄罗斯方块 2.0.2

若要转载，请经过我的同意并在显眼处标注出处。

不建议用这个应付作业。

> Dev-C++ 中若不能正常运行，请进入 工具(T) -> 编译选项 -> 编译器 -> 编译时加入以下命令 -> 输入“-std=c++11”（没有引号）即可。
---

相较于1.0.0版本，有这些较为明显的改动：
- 方块的颜色，L因控制台不支持橙色而改为灰色外，与Tetris Guideline 基本一致
- 计分方式，按照任天堂版本的计分系统，加分在原有的基础上再 * (level + 1)
- 硬降（Hard drop）改为与原版一致，下降后直接锁定


修补了一些特性，措施如下：
- 为防止游戏结束后不小心结束程序，只能按Esc或F1结束程序
- 暂停后锁定延迟重新记录
- 随机时第一包的第一个方块可以为O


效果演示：https://www.bilibili.com/video/BV17f4y147vN 

.exe文件同目录下必须有一个名为Highest.dat的文件才可以保存最高纪录

Tetris.zip压缩包中包含可执行文件与Highest.dat

![GitHub Logo](/pic-display.png)
（我最高纪录是145320）
