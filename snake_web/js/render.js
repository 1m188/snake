/**
 * Canvas 渲染模块。
 *
 * 本模块负责把游戏状态绘制到画布上：棋盘背景与网格、食物、蛇身
 * 与带朝向眼睛的蛇头。渲染与游戏逻辑完全解耦，仅读取状态、
 * 不修改状态；每帧整体重绘（Canvas 天然双缓冲，避免闪烁）。
 */

(function (global) {
    'use strict';

    const C = global.SnakeConstants;

    /** 渲染层自有配色，与游戏逻辑解耦。 */
    const COLORS = Object.freeze({
        boardBg: '#0f172a',
        gridLine: 'rgba(148, 163, 184, 0.10)',
        snakeHead: '#34d399',
        snakeBody: '#10b981',
        snakeHeadDead: '#f87171',
        snakeEye: '#0f172a',
        food: '#fb7185',
        foodGlow: 'rgba(251, 113, 133, 0.5)',
    });

    /** 蛇身格子的内边距（像素），留出格线间的缝隙。 */
    const SEGMENT_INSET = 2;
    /** 蛇身圆角半径（像素）。 */
    const SEGMENT_RADIUS = 6;
    /** 蛇头眼睛相对头的偏移与半径（像素）。 */
    const EYE_OFFSET_ALONG = 4;
    const EYE_OFFSET_SIDE = 5;
    const EYE_RADIUS = 2.2;

    class Renderer {
        /**
         * 创建渲染器。
         *
         * 按设备像素比放大画布分辨率，保证高分屏下图形清晰；
         * 显示尺寸由 CSS 控制（width: 100%）。
         *
         * Args:
         *     canvas: 目标画布元素。
         */
        constructor(canvas) {
            this._canvas = canvas;
            this._ctx = canvas.getContext('2d');
            this._size = C.GRID_COLS * C.CELL_SIZE;
            const dpr = global.devicePixelRatio || 1;
            canvas.width = this._size * dpr;
            canvas.height = this._size * dpr;
            this._ctx.scale(dpr, dpr);
        }

        /**
         * 绘制一帧画面。
         *
         * Args:
         *     game: 游戏对象；开始界面（尚无蛇）时仅绘制空棋盘。
         */
        draw(game) {
            this._drawBoard();
            if (!game.snake) {
                return;
            }
            this._drawFood(game.food);
            this._drawSnake(game.snake, game.state === C.GAME_STATE.GAME_OVER);
        }

        /** 绘制棋盘背景与网格线。 */
        _drawBoard() {
            const ctx = this._ctx;
            ctx.fillStyle = COLORS.boardBg;
            ctx.fillRect(0, 0, this._size, this._size);
            ctx.strokeStyle = COLORS.gridLine;
            ctx.lineWidth = 1;
            ctx.beginPath();
            for (let i = 1; i < C.GRID_COLS; i++) {
                const offset = i * C.CELL_SIZE;
                ctx.moveTo(offset, 0);
                ctx.lineTo(offset, this._size);
                ctx.moveTo(0, offset);
                ctx.lineTo(this._size, offset);
            }
            ctx.stroke();
        }

        /** 绘制食物：带柔光的圆形。 */
        _drawFood(food) {
            if (!food || !food.position) {
                return;
            }
            const ctx = this._ctx;
            const cx = (food.position.x + 0.5) * C.CELL_SIZE;
            const cy = (food.position.y + 0.5) * C.CELL_SIZE;
            const radius = C.CELL_SIZE * 0.32;
            ctx.save();
            ctx.shadowColor = COLORS.foodGlow;
            ctx.shadowBlur = 12;
            ctx.fillStyle = COLORS.food;
            ctx.beginPath();
            ctx.arc(cx, cy, radius, 0, Math.PI * 2);
            ctx.fill();
            ctx.restore();
        }

        /**
         * 绘制蛇身：圆角方块逐节绘制，蛇头颜色不同并带朝向眼睛。
         *
         * Args:
         *     snake: 蛇实体。
         *     isDead: 游戏结束时蛇头变为红色。
         */
        _drawSnake(snake, isDead) {
            const ctx = this._ctx;
            for (let i = snake.body.length - 1; i >= 0; i--) {
                const segment = snake.body[i];
                const px = segment.x * C.CELL_SIZE + SEGMENT_INSET;
                const py = segment.y * C.CELL_SIZE + SEGMENT_INSET;
                const size = C.CELL_SIZE - SEGMENT_INSET * 2;
                ctx.fillStyle =
                    i === 0
                        ? isDead
                            ? COLORS.snakeHeadDead
                            : COLORS.snakeHead
                        : COLORS.snakeBody;
                this._roundRect(px, py, size, size, SEGMENT_RADIUS);
                ctx.fill();
            }
            this._drawEyes(snake.head, snake.direction, isDead);
        }

        /** 在蛇头上绘制两只朝向移动方向或死亡 X 眼。 */
        _drawEyes(head, direction, isDead) {
            const ctx = this._ctx;
            const vector = C.DIRECTION_VECTORS[direction];
            const sideX = -vector.y;
            const sideY = vector.x;
            const cx = (head.x + 0.5) * C.CELL_SIZE;
            const cy = (head.y + 0.5) * C.CELL_SIZE;
            ctx.fillStyle = COLORS.snakeEye;
            for (const sign of [-1, 1]) {
                const ex =
                    cx +
                    vector.x * EYE_OFFSET_ALONG +
                    sideX * EYE_OFFSET_SIDE * sign;
                const ey =
                    cy +
                    vector.y * EYE_OFFSET_ALONG +
                    sideY * EYE_OFFSET_SIDE * sign;
                ctx.beginPath();
                ctx.arc(ex, ey, EYE_RADIUS, 0, Math.PI * 2);
                ctx.fill();
            }
        }

        /**
         * 描出圆角矩形路径（不填充）。
         *
         * 兼容不支持 ctx.roundRect 的浏览器。
         */
        _roundRect(x, y, width, height, radius) {
            const ctx = this._ctx;
            ctx.beginPath();
            ctx.moveTo(x + radius, y);
            ctx.arcTo(x + width, y, x + width, y + height, radius);
            ctx.arcTo(x + width, y + height, x, y + height, radius);
            ctx.arcTo(x, y + height, x, y, radius);
            ctx.arcTo(x, y, x + width, y, radius);
            ctx.closePath();
        }
    }

    if (typeof module !== 'undefined' && module.exports) {
        module.exports = Renderer;
    } else {
        global.Renderer = Renderer;
    }
})(typeof window !== 'undefined' ? window : globalThis);
