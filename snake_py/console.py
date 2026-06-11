"""
终端控制工具模块。

本模块将所有终端 IO 和 ANSI 转义序列封装为语义清晰的函数。
外部只需调用 write / move / home 等函数往内部缓冲区追加内容，
最后通过 flush() 一次性输出到终端，实现双缓冲渲染。

Attributes：
    _buf (list[str]): 模块级输出缓冲区，flush() 后清空。
"""

import shutil
import sys

_buf = []


def home():
    """光标复位到终端左上角 (1, 1)。"""
    _buf.append("\033[H")


def move(row, col):
    """
    将光标移动到指定位置。坐标 1-indexed。

    Args:
        row (int): 目标行号。
        col (int): 目标列号。
    """
    _buf.append(f"\033[{row};{col}H")


def write(s):
    """
    向缓冲区追加任意字符串。不校验内容，调用方保证正确性。

    Args:
        s (str): 待写入的字符串。
    """
    _buf.append(str(s))


def hide_cursor():
    """隐藏终端光标。"""
    _buf.append("\033[?25l")


def show_cursor():
    """显示终端光标。"""
    _buf.append("\033[?25h")


def clear():
    """清空整个终端屏幕（不清缓冲区）。"""
    _buf.append("\033[2J")


def flush():
    """将缓冲区中所有内容一次性写入 stdout 并清空缓冲区。"""
    sys.stdout.write("".join(_buf))
    sys.stdout.flush()
    _buf.clear()


def term_size():
    """
    获取当前终端尺寸。

    Returns:
        tuple: (列数, 行数)。
    """
    size = shutil.get_terminal_size()
    return size.columns, size.lines
