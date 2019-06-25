using System;
using System.IO;

namespace Snake
{
    /// <summary>
    /// 全局设置类
    /// </summary>
    static class Config
    {
        /// <summary>
        /// 最高分
        /// </summary>
        public static int HighestScore { get; set; }
        /// <summary>
        /// 控制台前景色
        /// </summary>
        public static ConsoleColor foregroundColor { get; set; }
        /// <summary>
        /// 控制台背景色
        /// </summary>
        public static ConsoleColor backgroundColor { get; set; }

        /// <summary>
        /// 静态构造函数
        /// </summary>
        static Config()
        {
            //默认初始化各种设置
            HighestScore = 0;
            foregroundColor = Console.ForegroundColor;
            backgroundColor = Console.BackgroundColor;
        }

        /// <summary>
        /// 初始化设置
        /// </summary>
        public static void init()
        {
            //如果文件存在则直接打开读取相关设置
            if (File.Exists("config.ini"))
            {
                FileStream fs = new FileStream("config.ini", FileMode.Open, FileAccess.Read);
                StreamReader sr = new StreamReader(fs);
                HighestScore = Convert.ToInt32(sr.ReadLine()); //读取最高分
                foregroundColor = (ConsoleColor)Convert.ToInt32(sr.ReadLine()); //读取前景色
                backgroundColor = (ConsoleColor)Convert.ToInt32(sr.ReadLine()); //读取背景色
                sr.Close();
                fs.Close();
            }
            //否则创建一个新的文件，并使用默认初始化的设置进行游戏
            else
            {
                FileStream fs = new FileStream("config.ini", FileMode.CreateNew);
                fs.Close();
            }

            //初始化设置控制台的颜色
            Console.ForegroundColor = foregroundColor;
            Console.BackgroundColor = backgroundColor;
        }

        /// <summary>
        /// 保存设置
        /// </summary>
        public static void uninit()
        {
            FileStream fs = new FileStream("config.ini", FileMode.Truncate, FileAccess.Write);
            StreamWriter sw = new StreamWriter(fs);
            sw.WriteLine(HighestScore); //保存最高分
            sw.WriteLine((int)foregroundColor); //保存前景色
            sw.WriteLine((int)backgroundColor); //保存背景色
            sw.Close();
            fs.Close();
        }
    }
}
