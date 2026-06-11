"""
食物模块。

本模块定义食物的生成与位置管理逻辑。食物以 '*' 符号表示，
在游戏区域内随机生成，且避开蛇身占据的坐标。
"""

import random
from collections.abc import Iterable


class Food:
    """
    食物实体。

    Attributes:
        position: 当前食物在游戏区域内的 (x, y) 坐标，未生成时为 None。
    """

    SYMBOL: str = "*"

    def __init__(self) -> None:
        """初始化食物，初始位置置空。"""
        self.position: tuple[int, int] | None = None

    def spawn(
        self, width: int, height: int, occupied: Iterable[tuple[int, int]]
    ) -> bool:
        """
        在游戏区域内随机生成食物。

        优先尝试随机采样（最多 100 次）；若采样失败则遍历全部空闲
        位置并从中随机选取。此策略在场地空旷时高效，在蛇接近铺满
        场地时仍可正确生成。

        Args:
            width: 游戏区域宽度（格子数）。
            height: 游戏区域高度（格子数）。
            occupied: 被蛇身占据的坐标集合，食物不可生成于此。

        Returns:
            生成成功返回 True，无可用位置返回 False。
        """
        occupied_set = set(occupied)
        max_attempts = 100

        for _ in range(max_attempts):
            x = random.randint(0, width - 1)
            y = random.randint(0, height - 1)
            if (x, y) not in occupied_set:
                self.position = (x, y)
                return True

        available = [
            (x, y)
            for y in range(height)
            for x in range(width)
            if (x, y) not in occupied_set
        ]

        if not available:
            return False

        self.position = random.choice(available)
        return True
