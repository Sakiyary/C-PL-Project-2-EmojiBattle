# Sakiyary$ Emoji Battle

——Sakiyary写于2022/2/2凌晨

##一. 编译环境与选项

1. 可直接在**Windows x64**下点击`Sakiyary_EmojiBattle.exe`运行体验游戏。
2. 若需编译，请先配置好**`SDL2`+`SDL2_image`+`SDL2_ttf`+`SDL2_mixer`**的环境，**Linux**下可直接通过命令行配置，**Windows**的**Mingw**条件下可参考自制的[配置SDL2环境的教程](https://www.bilibili.com/video/BV1oq4y1q72r)，**VC**条件下可参考[CSDN教程](https://blog.csdn.net/mingzhiqing/article/details/81071063)。
3. **命令行编译**(Mingw)，请用一堆`-l`链接各个库。
4. **Cmake编译**(Mingw)，可参考文件夹内附带的`CmakeList.txt`，使用Clion时注意设置**配置-工作目录**。
5. **Makefile编译**(VC)，请自行用`VS 20XX`设置项目选项，可参考上述CSDN教程。

##二. 游戏指南

0. **注意**: 设置的窗口大小为**960*1080**，当使用**1080p**的屏幕并将屏幕缩放调至大于**100%**时，游戏窗口会超出屏幕！同理，使用**2k**、**4k**屏幕时请将屏幕缩放调小一些，使得游戏窗口可在屏幕内完全展现。
1. 请根据**游戏内提示**体验游戏！
1. 若用**集成显卡**体验游戏，在大量弹幕出现时帧率会明显下降，难度降低。若用**独立显卡**体验游戏，帧率可以稳定在40FPS以上，难度升高！

##三. 作者叨叨

​	已经能较为熟练地运用纯C语言下的SDL2主体了！ (这也导致我碰也不想碰隔壁C++的easyX)

​	代码并没有分成多个文件，因为我的函数之间关系有点乱，一时捋不清(~~反正代码本体1000行都没到，放一个文件里又有什么大碍呢？~~(逃))。所以代码重构还是不到位，导致代码风格有点臭，但本人已经尽我所能减少屎山与表里不一、前后不一了！(~~说到底还是懒~~~)

​	以前犯过的那些问题(内存泄漏/交互卡顿/etc)这次不会再犯啦！

​	请体验一下我整的**烂活**吧！

​	感谢游玩！