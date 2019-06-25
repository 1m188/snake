using System;
using System.Threading;

namespace Snake.Scene
{
    /// <summary>
    /// 颜色（前景色、背景色）选择场景
    /// </summary>
    class ColorChooseScene : Scene
    {
        /// <summary>
        /// 前、背景色选择
        /// </summary>
        private int settingChoose = '1';

        /// <summary>
        /// 初始化场景
        /// </summary>
        /// <param name="settingChoose">选择的是前景色还是背景色</param>
        public void init(int settingChoose)
        {
            this.settingChoose = settingChoose;
            Console.SetCursorPosition(Console.WindowWidth / 2 - 4, Console.WindowHeight / 3);
            if (settingChoose == '1')
            {
                Console.Write("前");
            }
            else if (settingChoose == '2')
            {
                Console.Write("背");
            }
            Console.Write("景色选择");
            Console.SetCursorPosition(Console.WindowWidth / 2 - 10, Console.WindowHeight / 3 + 4);
            Console.Write("0. 黑色");
            Console.SetCursorPosition(Console.WindowWidth / 2 - 10, Console.WindowHeight / 3 + 7);
            Console.Write("1. 藏蓝色");
            Console.SetCursorPosition(Console.WindowWidth / 2 - 10, Console.WindowHeight / 3 + 10);
            Console.Write("2. 深绿色");
            Console.SetCursorPosition(Console.WindowWidth / 2 - 10, Console.WindowHeight / 3 + 13);
            Console.Write("3. 深紫色");
            Console.SetCursorPosition(Console.WindowWidth / 2 - 10, Console.WindowHeight / 3 + 16);
            Console.Write("4. 深红色");
            Console.SetCursorPosition(Console.WindowWidth / 2 - 10, Console.WindowHeight / 3 + 19);
            Console.Write("5. 深紫红色");
            Console.SetCursorPosition(Console.WindowWidth / 2 - 10, Console.WindowHeight / 3 + 22);
            Console.Write("6. 深黄色");
            Console.SetCursorPosition(Console.WindowWidth / 2 - 10, Console.WindowHeight / 3 + 25);
            Console.Write("7. 灰色");
            Console.SetCursorPosition(Console.WindowWidth / 2 + 5, Console.WindowHeight / 3 + 4);
            Console.Write("8. 深灰色");
            Console.SetCursorPosition(Console.WindowWidth / 2 + 5, Console.WindowHeight / 3 + 7);
            Console.Write("9. 蓝色");
            Console.SetCursorPosition(Console.WindowWidth / 2 + 5, Console.WindowHeight / 3 + 10);
            Console.Write("a. 绿色");
            Console.SetCursorPosition(Console.WindowWidth / 2 + 5, Console.WindowHeight / 3 + 13);
            Console.Write("b. 青色");
            Console.SetCursorPosition(Console.WindowWidth / 2 + 5, Console.WindowHeight / 3 + 16);
            Console.Write("c. 红色");
            Console.SetCursorPosition(Console.WindowWidth / 2 + 5, Console.WindowHeight / 3 + 19);
            Console.Write("d. 紫红色");
            Console.SetCursorPosition(Console.WindowWidth / 2 + 5, Console.WindowHeight / 3 + 22);
            Console.Write("e. 黄色");
            Console.SetCursorPosition(Console.WindowWidth / 2 + 5, Console.WindowHeight / 3 + 25);
            Console.Write("f. 白色");
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
                    //只有按键为存在的选择项才可以进入后面的判断
                    if ((key >= '0' && key <= '9') || (key >= 'a' && key <= 'f'))
                    {
                        //颜色枚举值
                        ConsoleColor colorChoose = ConsoleColor.White;
                        //将key转换成相应的颜色枚举值
                        if (key >= '0' && key <= '9')
                        {
                            colorChoose = (ConsoleColor)(key - '0');
                        }
                        else
                        {
                            colorChoose = (ConsoleColor)(key - 'a' + 10);
                        }
                        //判定改变前景色还是背景色
                        if (settingChoose == '1')
                        {
                            Console.ForegroundColor = colorChoose;
                            Config.foregroundColor = colorChoose;
                        }
                        else if (settingChoose == '2')
                        {
                            Console.BackgroundColor = colorChoose;
                            Config.backgroundColor = colorChoose;
                        }
                        break;
                    }
                }
                Thread.Sleep(1);
            }
            return base.run();
        }
    }
}
