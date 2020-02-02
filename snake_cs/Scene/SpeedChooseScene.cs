using System;
using System.Threading;

namespace Snake.Scene
{
    /// <summary>
    /// 速度（难度）选择场景
    /// </summary>
    class SpeedChooseScene : Scene
    {
        /// <summary>
        /// 速度选择场景初始化
        /// </summary>
        public override void init()
        {
            Console.SetCursorPosition(Console.WindowWidth / 2 - 4, Console.WindowHeight / 3);
            Console.Write("速度选择");
            Console.SetCursorPosition(Console.WindowWidth / 2 - 4, Console.WindowHeight / 3 + 4);
            Console.Write("1. 慢速");
            Console.SetCursorPosition(Console.WindowWidth / 2 - 4, Console.WindowHeight / 3 + 8);
            Console.Write("2. 中速");
            Console.SetCursorPosition(Console.WindowWidth / 2 - 4, Console.WindowHeight / 3 + 12);
            Console.Write("3. 快速");
            Console.SetCursorPosition(Console.WindowWidth / 2 - 4, Console.WindowHeight / 3 + 16);
            Console.Write("4. 光速");
        }

        /// <summary>
        /// 运行速度选择场景
        /// </summary>
        /// <returns>运行相关信息</returns>
        public override int run()
        {
            while (true)
            {
                if (Console.KeyAvailable)
                {
                    int key = Console.ReadKey(true).KeyChar;
                    if (key == '1' || key == '2' || key == '3' || key == '4')
                    {
                        return key;
                    }
                }
                Thread.Sleep(1);
            }
        }
    }
}
