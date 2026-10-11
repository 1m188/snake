/**
 * 常量模块。
 *
 * 本模块集中定义游戏的全局常量：场地尺寸、速度参数、方向枚举、
 * 游戏状态枚举与存储键名。浏览器环境下挂载到 window.SnakeConstants，
 * Node 环境下通过 module.exports 提供给测试。
 */

(function (global) {
    'use strict';

    const SnakeConstants = Object.freeze({
        /** 场地列数。 */
        GRID_COLS: 20,
        /** 场地行数。 */
        GRID_ROWS: 20,
        /** 单个格子的边长（像素），仅渲染层使用。 */
        CELL_SIZE: 24,
        /** 逻辑帧率：每秒推进的固定逻辑帧数。 */
        LOGIC_FPS: 60,
        /** 蛇的初始节数。 */
        INITIAL_SNAKE_LENGTH: 3,
        /** 初始移动间隔（逻辑帧数，约 150ms/格），值越小蛇越快。 */
        INITIAL_MOVE_INTERVAL: 10,
        /** 最快移动间隔（逻辑帧数），加速下限。 */
        FASTEST_MOVE_INTERVAL: 2,
        /** 每吃多少个食物提升一次速度。 */
        FOODS_PER_SPEEDUP: 3,

        /** 方向枚举。 */
        DIRECTION: Object.freeze({
            UP: 'up',
            DOWN: 'down',
            LEFT: 'left',
            RIGHT: 'right',
        }),

        /** 方向到坐标偏移的映射。 */
        DIRECTION_VECTORS: Object.freeze({
            up: Object.freeze({ x: 0, y: -1 }),
            down: Object.freeze({ x: 0, y: 1 }),
            left: Object.freeze({ x: -1, y: 0 }),
            right: Object.freeze({ x: 1, y: 0 }),
        }),

        /** 方向的反方向映射，用于 180 度防反判定。 */
        OPPOSITE_DIRECTION: Object.freeze({
            up: 'down',
            down: 'up',
            left: 'right',
            right: 'left',
        }),

        /** 游戏状态枚举。 */
        GAME_STATE: Object.freeze({
            START: 'start',
            PLAYING: 'playing',
            PAUSED: 'paused',
            GAME_OVER: 'game_over',
        }),

        /** 最高分在 localStorage 中的键名。 */
        HIGH_SCORE_KEY: 'snake_web_high_score',
    });

    if (typeof module !== 'undefined' && module.exports) {
        module.exports = SnakeConstants;
    } else {
        global.SnakeConstants = SnakeConstants;
    }
})(typeof window !== 'undefined' ? window : globalThis);
