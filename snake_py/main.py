"""
snake_py 入口模块。

本模块是贪吃蛇游戏的程序入口，负责启动游戏主循环。
"""

import sys
import os

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

from game import Game


def main() -> None:
    """启动贪吃蛇游戏。"""
    game = Game()
    game.run()


if __name__ == "__main__":
    main()
