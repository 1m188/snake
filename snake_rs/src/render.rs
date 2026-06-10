/// 终端渲染模块
///
/// 采用面向对象的组件化渲染方式，将地图边框、蛇、食物、分数
/// 各自封装为独立的渲染函数。所有组件通过 `queue` 排队绘制命令，
/// 最终由 `render` 编排函数按层叠顺序调用并一次性 `flush` 到终端，
/// 实现无闪烁的双缓冲渲染。
///
/// 游戏结束界面独立于组件体系，使用专门的结算渲染函数。
use crossterm::{
    QueueableCommand,
    cursor::MoveTo,
    execute,
    style::Print,
    terminal::{Clear, ClearType},
};
use std::io::{self, Write};

use crate::game::Game;

/// 估算字符串在终端中的显示宽度
///
/// ASCII 字符计为 1 列，全角字符（中文等）统一计为 2 列。
fn display_width(text: &str) -> u16 {
    text.chars()
        .map(|c| if c.is_ascii() { 1u16 } else { 2u16 })
        .sum()
}

/// 渲染地图边框
///
/// `corner` 为四角符号，`h_edge` 为水平边线，`v_edge` 为垂直边线。
/// 不处理内部区域清空——由 `render` 统一清屏。
fn render_border<W: Write>(
    writer: &mut W,
    game: &Game,
    corner: char,
    h_edge: char,
    v_edge: char,
) -> io::Result<()> {
    let w = game.width;
    let h = game.height;

    // 顶部边框
    writer.queue(MoveTo(0, 0))?;
    writer.queue(Print(corner))?;
    for _ in 0..w {
        writer.queue(Print(h_edge))?;
    }
    writer.queue(Print(corner))?;

    // 左右边框
    for y in 0..h {
        writer.queue(MoveTo(0, y + 1))?;
        writer.queue(Print(v_edge))?;
        writer.queue(MoveTo(w + 1, y + 1))?;
        writer.queue(Print(v_edge))?;
    }

    // 底部边框
    writer.queue(MoveTo(0, h + 1))?;
    writer.queue(Print(corner))?;
    for _ in 0..w {
        writer.queue(Print(h_edge))?;
    }
    writer.queue(Print(corner))?;

    Ok(())
}

/// 渲染食物
///
/// 在食物坐标处绘制 `symbol`。
fn render_food<W: Write>(writer: &mut W, game: &Game, symbol: char) -> io::Result<()> {
    let (fx, fy) = game.food;
    writer.queue(MoveTo(fx + 1, fy + 1))?;
    writer.queue(Print(symbol))?;
    Ok(())
}

/// 渲染蛇
///
/// 在蛇头坐标处绘制 `head`，在其余身体节坐标处绘制 `body`。
fn render_snake<W: Write>(writer: &mut W, game: &Game, head: char, body: char) -> io::Result<()> {
    for (i, &(sx, sy)) in game.snake.iter().enumerate() {
        writer.queue(MoveTo(sx + 1, sy + 1))?;
        writer.queue(Print(if i == 0 { head } else { body }))?;
    }
    Ok(())
}

/// 渲染分数
///
/// 在顶部边框行的右侧覆盖显示当前得分，天然遮盖其后的 `-` 边框。
fn render_score<W: Write>(writer: &mut W, game: &Game) -> io::Result<()> {
    let score_text = format!("得分: {}", game.score);
    let dw = display_width(&score_text);
    let w = game.width;

    if dw + 1 < w {
        // 从顶部边框行右侧写入，覆盖该位置原有的 `-`
        let score_x = w - dw;
        writer.queue(MoveTo(score_x + 1, 0))?;
        writer.queue(Print(&score_text))?;
    }
    Ok(())
}

/// 编排渲染各组件并按层叠顺序整体输出（双缓冲）
///
/// 首先清空终端画面以消除上一帧残留，然后按层叠顺序绘制各组件：
/// 边框（底层）→ 食物 → 蛇 → 分数（顶层，覆写边框 `-`）。
/// 所有渲染字符在调用点集中管理，组件函数只关心"在哪画"。
/// 所有命令排队后一次性 `flush`，避免闪烁。
pub fn render<W: Write>(writer: &mut W, game: &Game) -> io::Result<()> {
    // 清空终端，消除上一帧残留
    writer.queue(Clear(ClearType::All))?;
    render_border(writer, game, '+', '-', '|')?;
    render_food(writer, game, '*')?;
    render_snake(writer, game, '@', '#')?;
    render_score(writer, game)?;
    writer.flush()
}

/// 渲染游戏结束结算画面
///
/// 清屏后在屏幕正中央纵向排列 4 行居中文本：
/// "游戏结束！"、"最终得分: N"、"按下任意键继续"、"按下esc键退出游戏"。
/// 每两行之间间隔一行空白，保持画面匀称。
pub fn render_game_over<W: Write>(writer: &mut W, game: &Game) -> io::Result<()> {
    execute!(writer, Clear(ClearType::All))?;

    let (term_w, term_h) = crossterm::terminal::size()?;
    let cx = term_w / 2;
    let cy = term_h / 2;

    let lines = [
        "游戏结束！",
        &format!("最终得分: {}", game.score),
        "按下任意键继续",
        "按下esc键退出游戏",
    ];

    for (i, line) in lines.iter().enumerate() {
        let dw = display_width(line);
        let row = cy.saturating_sub(3) + (i * 2) as u16;
        writer.queue(MoveTo(cx.saturating_sub(dw / 2), row))?;
        writer.queue(Print(*line))?;
    }

    writer.flush()
}
