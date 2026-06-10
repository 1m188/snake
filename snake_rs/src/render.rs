/// 终端渲染模块
///
/// 负责将游戏状态绘制到终端界面。采用双缓冲渲染策略：
/// 在内存中逐行构建画面字符串，通过 `queue` 排队所有绘制命令，
/// 最后一次性 `flush` 到终端，避免画面闪烁或撕裂。
///
/// 游戏结束时在屏幕中央显示结算画面，包含最终得分及操作提示。
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
/// 此估算对本游戏中的中文字符串足够准确。
fn display_width(text: &str) -> u16 {
    text.chars()
        .map(|c| if c.is_ascii() { 1u16 } else { 2u16 })
        .sum()
}

/// 构建顶部边框字符串（含右侧得分）
fn build_top_border(game: &Game) -> String {
    let w = game.width as usize;
    let score_text = format!("得分: {}", game.score);
    let dw = display_width(&score_text) as usize;
    let mut line = String::with_capacity(w + 2);

    line.push('+');
    if dw + 1 < w {
        let dash_count = w - dw - 1;
        for _ in 0..dash_count {
            line.push('-');
        }
        line.push_str(&score_text);
        line.push('-');
    } else {
        for _ in 0..w {
            line.push('-');
        }
    }
    line.push('+');
    line
}

/// 构建棋盘中部某行的字符串（含左右边框）
fn build_row(game: &Game, y: u16) -> String {
    let w = game.width as usize;
    let mut line = String::with_capacity(w + 2);

    line.push('|');
    for x in 0..w {
        let pos = (x as u16, y);
        if pos == game.snake[0] {
            line.push('@'); // 蛇头
        } else if game.snake.contains(&pos) {
            line.push('#'); // 蛇身
        } else if pos == game.food {
            line.push('*'); // 食物
        } else {
            line.push(' '); // 空白
        }
    }
    line.push('|');
    line
}

/// 构建底部边框字符串
fn build_bottom_border(game: &Game) -> String {
    let w = game.width as usize;
    let mut line = String::with_capacity(w + 2);

    line.push('+');
    for _ in 0..w {
        line.push('-');
    }
    line.push('+');
    line
}

/// 渲染游戏进行中的完整界面（双缓冲）
///
/// 在内存中逐行构建画面字符串并排队所有绘制命令，
/// 最终通过一次 `flush` 整体输出到终端。
pub fn render<W: Write>(writer: &mut W, game: &Game) -> io::Result<()> {
    let h = game.height;

    // 顶部边框
    let top = build_top_border(game);
    writer.queue(MoveTo(0, 0))?;
    writer.queue(Print(&top))?;

    // 中部各行
    for y in 0..h {
        let row = build_row(game, y);
        writer.queue(MoveTo(0, y + 1))?;
        writer.queue(Print(&row))?;
    }

    // 底部边框
    let bottom = build_bottom_border(game);
    writer.queue(MoveTo(0, h + 1))?;
    writer.queue(Print(&bottom))?;

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

    // 4 行文本，纵向居中，相邻行间隔 2 行（1 行空白）
    for (i, line) in lines.iter().enumerate() {
        let dw = display_width(line);
        let row = cy.saturating_sub(3) + (i * 2) as u16;
        writer.queue(MoveTo(cx.saturating_sub(dw / 2), row))?;
        writer.queue(Print(*line))?;
    }

    writer.flush()
}
