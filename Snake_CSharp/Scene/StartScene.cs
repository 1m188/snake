using System;
using System.Threading;

namespace Snake.Scene
{
    /// <summary>
    /// 开始场景
    /// </summary>
    class StartScene : Scene
    {
        /// <summary>
        /// 开始场景初始化
        /// </summary>
        public override void init()
        {
            Console.SetCursorPosition(Console.WindowWidth / 2 - 4, Console.WindowHeight / 3);
            Console.Write("贪  吃  蛇");
            Console.SetCursorPosition(Console.WindowWidth / 2 - 6, Console.WindowHeight / 3 + 4);
            Console.Write("1. 开始新游戏");
            Console.SetCursorPosition(Console.WindowWidth / 2 - 6, Console.WindowHeight / 3 + 8);
            Console.Write("2. 最高分");
            Console.SetCursorPosition(Console.WindowWidth / 2 - 6, Console.WindowHeight / 3 + 12);
            Console.Write("3. 设置");
            Console.SetCursorPosition(Console.WindowWidth / 2 - 6, Console.WindowHeight / 3 + 16);
            Console.Write("4. 退出游戏");
        }

        /// <summary>
        /// 运行开始场景
        /// </summary>
        /// <returns></returns>
        public override int run()
        {
            while (true)
            {
                if (Console.KeyAvailable)
                {
                    int key = Console.ReadKey(true).KeyChar;
                    //判定按键是否选择了某个菜单项
                    if (key == '1' || key == '2' || key == '3' || key == '4')
                    {
                        return key; //返回用作判定
                    }
                }
                Thread.Sleep(1);
            }
        }
    }
}
