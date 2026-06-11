"""
蛇实体模块。

本模块定义蛇的数据结构与行为，包括蛇身的存储表示、移动与生长逻辑、
方向控制以及碰撞检测。蛇身使用双端队列（deque）存储，蛇头为 '@'，
蛇身为 '#'。
"""

from collections import deque
from enum import Enum


class Direction(Enum):
    """移动方向枚举，值为 (dx, dy) 偏移量。"""

    UP = (0, -1)
    DOWN = (0, 1)
    LEFT = (-1, 0)
    RIGHT = (1, 0)


class Snake:
    """
    贪吃蛇实体。

    Attributes:
        body: 蛇身坐标双端队列，队首为蛇头，每项为 (x, y)。
        direction: 当前移动方向。
        _grow_pending: 下次移动时是否增长一节。
    """

    HEAD_SYMBOL: str = "@"
    BODY_SYMBOL: str = "#"

    def __init__(
        self,
        start_x: int,
        start_y: int,
        direction: Direction = Direction.RIGHT,
        length: int = 3,
    ) -> None:
        """
        初始化蛇。

        蛇身从蛇头位置向移动方向的反方向延伸。

        Args:
            start_x: 蛇头初始 x（游戏区域坐标）。
            start_y: 蛇头初始 y（游戏区域坐标）。
            direction: 初始移动方向，默认向右。
            length: 初始节数，默认 3。
        """
        dx, dy = direction.value
        self.body: deque[tuple[int, int]] = deque()
        for i in range(length):
            self.body.append((start_x - dx * i, start_y - dy * i))
        self.direction: Direction = direction
        self._grow_pending: bool = False

    @property
    def head(self) -> tuple[int, int]:
        """蛇头坐标 (x, y)。"""
        return self.body[0]

    def set_direction(self, new_direction: Direction) -> None:
        """
        设置移动方向。

        禁止 180 度掉头：若新方向与当前方向相反则忽略。

        Args:
            new_direction: 目标方向。
        """
        if (
            self.direction.value[0] + new_direction.value[0] == 0
            and self.direction.value[1] + new_direction.value[1] == 0
        ):
            return
        self.direction = new_direction

    def move(self) -> None:
        """
        沿当前方向移动一格。

        新蛇头插入队首；若无待处理的增长请求则移除队尾一节，
        实现蛇的正常滑动效果。
        """
        head_x, head_y = self.head
        dx, dy = self.direction.value
        new_head = (head_x + dx, head_y + dy)
        self.body.appendleft(new_head)
        if self._grow_pending:
            self._grow_pending = False
        else:
            self.body.pop()

    def grow(self) -> None:
        """标记在下次移动时增长一节（吃到食物时调用）。"""
        self._grow_pending = True

    def occupies(self, position: tuple[int, int]) -> bool:
        """
        判断蛇身是否占据给定坐标。

        Args:
            position: (x, y) 坐标。

        Returns:
            被占据时返回 True。
        """
        return position in self.body

    def check_self_collision(self) -> bool:
        """
        检测蛇头是否撞到自身。

        Returns:
            蛇头与身体任意节重叠时返回 True。
        """
        head = self.head
        for segment in list(self.body)[1:]:
            if head == segment:
                return True
        return False

    def __len__(self) -> int:
        return len(self.body)
