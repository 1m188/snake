using System;
using System.Threading;

namespace Snake.Scene
{
    /// <summary>
    /// 最高分场景
    /// </summary>
    class HighestScoreScene : Scene
    {
        /// <summary>
        /// 初始化最高分场景
        /// </summary>
        public override void init()
        {
            Console.SetCursorPosition(Console.WindowWidth / 2 - 4, Console.WindowHeight / 3);
            Console.Write("最高分");
            Console.SetCursorPosition(Console.WindowWidth / 2 - 3, Console.WindowHeight / 3 + 4);
            Console.Write("{0}分", Config.HighestScore);
            Console.SetCursorPosition(Console.WindowWidth / 2 - 11, Console.WindowHeight / 3 + 8);
            Console.Write("按任意键回到开始界面...");
        }

        /// <summary>
        /// 运行最高分场景
        /// </summary>
        /// <returns>运行相关信息</returns>
        public override int run()
        {
            while (true)
            {
                if (Console.KeyAvailable)
                {
                    Console.ReadKey(true);
                    break;
                }
                Thread.Sleep(1);
            }
            return base.run();
        }
    }
}
