#pragma once

// 游戏场景
// 传入游戏模式（手动操控/AI操控）
// 返回获得分数
const int gameScene(const int mode);

// 在另一个线程之中用来确定帧率的计时函数
// 传入用作指示是否执行游戏流程的计数器指针
void timing(void *num);