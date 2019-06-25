using System;
using System.Threading;

namespace Snake.Scene
{
    /// <summary>
    /// 设置场景
    /// </summary>
    class SettingScene : Scene
    {
        /// <summary>
        /// 初始化场景
        /// </summary>
        public override void init()
        {
            Console.SetCursorPosition(Console.WindowWidth / 2 - 4, Console.WindowHeight / 3);
            Console.Write("设置选择");
            Console.SetCursorPosition(Console.WindowWidth / 2 - 7, Console.WindowHeight / 3 + 4);
            Console.Write("1. 前景色设置");
            Console.SetCursorPosition(Console.WindowWidth / 2 - 7, Console.WindowHeight / 3 + 8);
            Console.Write("2. 背景色设置");
        }

        /// <summary>
        /// 运行场景
        /// </summary>
        /// <returns>运行相关信息</returns>
        public override int run()
        {
            while (true)
            {
                if (Console.KeyAvailable)
                {
                    int key = Console.ReadKey(true).KeyChar;
                    if (key == '1' || key == '2')
                    {
                        return key;
                    }
                }
                Thread.Sleep(1);
            }
        }
    }
}
