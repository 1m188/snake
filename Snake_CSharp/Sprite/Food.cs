using System;

namespace Snake.Sprite
{
    /// <summary>
    /// 食物类
    /// </summary>
    class Food
    {
        //字段
        /// <summary>
        /// 当前食物的x坐标
        /// </summary>
        private int x;
        /// <summary>
        /// 当前食物的y坐标
        /// </summary>
        private int y;

        //属性
        /// <summary>
        /// 当前食物的x坐标
        /// </summary>
        public int X
        {
            get { return x; }
        }
        /// <summary>
        /// 当前食物的y坐标
        /// </summary>
        public int Y
        {
            get { return y; }
        }
        //这个属性没有定义字段，这个属性包含了get和set的两个方法，而且C#在后台为这个属性自动创建了字段，所有的方法都对这个默认字段操作
        /// <summary>
        /// 蛇吃到了这个食物之后增长的节数
        /// </summary>
        public int BodyGrowNum { get; set; }

        /// <summary>
        /// 构造函数
        /// </summary>
        /// <param name="x">初始的x坐标</param>
        /// <param name="y">初始的y坐标</param>
        /// <param name="BodyGrowNum">吃到这个食物的时候蛇增长节数</param>
        public Food(int x = 0, int y = 0, int BodyGrowNum = 1)
        {
            //初始化食物相关属性
            this.x = x;
            this.y = y;
            this.BodyGrowNum = BodyGrowNum;
        }

        //方法
        /// <summary>
        /// 是否显示食物坐标
        /// </summary>
        /// <param name="isDisplay"></param>
        public void display(bool isDisplay)
        {
            try
            {
                Console.SetCursorPosition(x, y);
            }
            catch (ArgumentOutOfRangeException) //倘若坐标超出界限就停止这一次的食物显示或者隐藏
            {
                return;
            }
            if (isDisplay) //显示/隐藏食物
            {
                Console.Write('*');
            }
            else
            {
                Console.Write(' ');
            }
        }

        /// <summary>
        /// 生成一个新的食物坐标
        /// </summary>
        public void getNewPos(int left, int right, int up, int down)
        {
            Random random = new Random(); //获取随机类
            int newX = random.Next(left + 1, right); //生成新的x坐标
            while (newX == x) //避免新的x坐标和现在的x坐标重复
            {
                newX = random.Next(left + 1, right);
            }
            int newY = random.Next(up + 1, down); //生成新的y坐标
            while (newY == y) //避免新的y坐标和现在的y坐标重复
            {
                newY = random.Next(up + 1, down);
            }
            //更新坐标
            x = newX;
            y = newY;
        }
    }
}
