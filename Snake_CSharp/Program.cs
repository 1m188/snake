using System;
using Snake.Scene;

namespace Snake
{
    class Program
    {
        static void Main(string[] args) //主方法
        {
            //初始化某些基本设置
            Config.init();

            //控制台基本设置
            Console.Title = "贪吃蛇"; //设置控制台标题
            Console.SetWindowSize(100, 50); //设置控制台窗口大小
            Console.SetBufferSize(101, 51); //设置控制台缓冲区大小
            Console.CursorVisible = false; //隐藏光标

            //进入游戏主循环
            while (true)
            {
                //开始场景
                StartScene startScene = new StartScene();
                startScene.init();
                int startSceneReturn = startScene.run(); //获取开始场景的运行结果（即按下的是哪个键）
                Console.Clear();
                //判定接下来走哪个场景
                //开始新游戏
                if (startSceneReturn == '1')
                {
                    //速度选择场景
                    SpeedChooseScene speedChooseScene = new SpeedChooseScene();
                    speedChooseScene.init();
                    int difficulty = speedChooseScene.run(); //获取所选择的难度
                    Console.Clear();
                    //游戏主场景
                    GameScene gameScene = new GameScene();
                    gameScene.init(difficulty - '0');
                    int score = gameScene.run(); //获取最终得分
                    Console.Clear();
                    //游戏结束场景
                    GameEndScene gameEndScene = new GameEndScene();
                    gameEndScene.init(score);
                    gameEndScene.run();
                    Console.Clear();
                }
                //最高分
                else if (startSceneReturn == '2')
                {
                    //最高分场景
                    HighestScoreScene highestScoreScene = new HighestScoreScene();
                    highestScoreScene.init();
                    highestScoreScene.run();
                    Console.Clear();
                }
                //设置
                else if (startSceneReturn == '3')
                {
                    //设置界面
                    SettingScene settingScene = new SettingScene();
                    settingScene.init();
                    int settingChoose = settingScene.run(); //获取选择了哪个选项
                    Console.Clear();
                    //颜色选择界面
                    ColorChooseScene colorChooseScene = new ColorChooseScene();
                    colorChooseScene.init(settingChoose);
                    colorChooseScene.run();
                    Console.Clear();
                }
                //退出游戏
                else if (startSceneReturn == '4')
                {
                    break;
                }
            }

            //保存相关设置
            Config.uninit();
        }
    }
}
