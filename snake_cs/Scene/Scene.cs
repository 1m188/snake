namespace Snake.Scene
{
    /// <summary>
    /// 基础场景类
    /// </summary>
    class Scene
    {
        /// <summary>
        /// 场景初始化
        /// </summary>
        public virtual void init()
        {

        }

        /// <summary>
        /// 运行场景
        /// </summary>
        /// <returns>表示运行结果或者某些运行信息的值</returns>
        public virtual int run()
        {
            return 0;
        }
    }
}
