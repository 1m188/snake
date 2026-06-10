/// 终端渲染模块
///
/// 负责将游戏状态绘制到终端界面，包括边界绘制、蛇体绘制、
/// 食物绘制、分数显示以及游戏结束结算画面。
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
/// ASCII 字符计为 1 列，其他字符（中文等）统一计为 2 列。
/// 此估算对本游戏中的中文字符串足够准确。
fn display_width(text: &str) -> u16 {
    text.chars()
        .map(|c| if c.is_ascii() { 1u16 } else { 2u16 })
        .sum()
}

/// 绘制游戏进行中的完整界面
///
/// 包含带边界的棋盘、蛇（'@' 为蛇头、'#' 为蛇身）、食物（'*'），
/// 并在顶部边框右侧覆盖显示当前得分。
pub fn render<W: Write>(writer: &mut W, game: &Game) -> io::Result<()> {
    execute!(writer, Clear(ClearType::All))?;

    let w = game.width;
    let h = game.height;

    // ── 顶部边框，右侧覆写得分 ──
    writer.queue(MoveTo(0, 0))?;
    writer.queue(Print('+'))?;
    for _ in 0..w {
        writer.queue(Print('-'))?;
    }
    writer.queue(Print('+'))?;

    // 在顶部边框行上覆盖显示分数（右侧对齐）
    let score_text = format!("得分: {}", game.score);
    let dw = display_width(&score_text);
    if dw < w {
        // 分数从右 '+' 前方开始覆盖，右侧留 1 个 '-' 间距
        let score_x = w - dw;
        writer.queue(MoveTo(score_x + 1, 0))?;
        writer.queue(Print(&score_text))?;
    }

    // ── 中部各行：左右边框与游戏内容 ──
    for y in 0..h {
        writer.queue(MoveTo(0, y + 1))?;
        writer.queue(Print('|'))?;

        // 逐列绘制该行内容（Print 会自动移动光标，无需每次 MoveTo）
        for x in 0..w {
            let pos = (x, y);
            let ch = if pos == game.snake[0] {
                '@' // 蛇头
            } else if game.snake.contains(&pos) {
                '#' // 蛇身
            } else if pos == game.food {
                '*' // 食物
            } else {
                ' ' // 空白
            };
            writer.queue(Print(ch))?;
        }

        // 绘制右侧边框（此时光标已在 w+1 列）
        writer.queue(Print('|'))?;
    }

    // ── 底部边框 ──
    writer.queue(MoveTo(0, h + 1))?;
    writer.queue(Print('+'))?;
    for _ in 0..w {
        writer.queue(Print('-'))?;
    }
    writer.queue(Print('+'))?;

    writer.flush()
}

/// 绘制游戏结束结算画面
///
/// 清屏后在屏幕正中分三行显示"游戏结束！"、最终得分和"按任意键重新开始…"。
pub fn render_game_over<W: Write>(writer: &mut W, game: &Game) -> io::Result<()> {
    execute!(writer, Clear(ClearType::All))?;

    let (term_w, term_h) = crossterm::terminal::size()?;
    let cx = term_w / 2;
    let cy = term_h / 2;

    let msg1 = "游戏结束！";
    let msg2 = format!("最终得分: {}", game.score);
    let msg3 = "按任意键重新开始…";

    // 三行文字水平居中，纵向间隔一行
    let dw1 = display_width(msg1);
    let dw2 = display_width(&msg2);
    let dw3 = display_width(msg3);
    writer.queue(MoveTo(cx.saturating_sub(dw1 / 2), cy.saturating_sub(2)))?;
    writer.queue(Print(msg1))?;
    writer.queue(MoveTo(cx.saturating_sub(dw2 / 2), cy))?;
    writer.queue(Print(&msg2))?;
    writer.queue(MoveTo(cx.saturating_sub(dw3 / 2), cy + 2))?;
    writer.queue(Print(msg3))?;

    writer.flush()
}
