using System;
using System.Threading;

namespace Snake.Scene
{
    /// <summary>
    /// 游戏结束场景
    /// </summary>
    class GameEndScene : Scene
    {
        /// <summary>
        /// 游戏结束场景初始化
        /// </summary>
        /// <param name="score">最终得分</param>
        public void init(int score)
        {
            Console.SetCursorPosition(Console.WindowWidth / 2 - 4, Console.WindowHeight / 3);
            Console.Write("游戏结束");
            Console.SetCursorPosition(Console.WindowWidth / 2 - 6, Console.WindowHeight / 3 + 4);
            Console.Write("你的得分是：{0}", score);
            //如果为最高分则保存最高分并且输出相应的信息
            if (score > Config.HighestScore)
            {
                Config.HighestScore = score;
                Console.Write("  ------新的最高分！！！");
            }
            Console.SetCursorPosition(Console.WindowWidth / 2 - 10, Console.WindowHeight / 3 + 8);
            Console.Write("按任意键返回开始界面...");
        }

        /// <summary>
        /// 运行游戏结束场景
        /// </summary>
        /// <returns>场景运行相关信息</returns>
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
