/**
 * 食物实体模块。
 *
 * 本模块定义食物的数据结构与生成逻辑：食物随机出现在场地中
 * 一个未被蛇身占据的格子内，被吃掉后可再次生成。
 */

(function (global) {
    'use strict';

    class Food {
        /** 初始化食物，位置为 null（尚未生成）。 */
        constructor() {
            this._position = null;
        }

        /** 食物当前坐标 { x, y }，未生成时为 null。 */
        get position() {
            return this._position;
        }

        /**
         * 在场地内随机生成食物。
         *
         * 收集所有未被占用的格子，再按注入的随机函数均匀选取一个，
         * 保证食物永远不会出现在被占用的格子上。
         *
         * Args:
         *     cols: 场地列数。
         *     rows: 场地行数。
         *     occupied: 被占用格子的 Set（键为 "x,y"）。
         *     random: 随机函数，返回 [0, 1)，默认 Math.random，可注入以便测试。
         */
        spawn(cols, rows, occupied, random = Math.random) {
            const free = [];
            for (let y = 0; y < rows; y++) {
                for (let x = 0; x < cols; x++) {
                    if (!occupied.has(x + ',' + y)) {
                        free.push({ x: x, y: y });
                    }
                }
            }
            if (free.length === 0) {
                return; // 场地已被填满的边界情况，保持原位置
            }
            const index = Math.min(
                free.length - 1,
                Math.floor(random() * free.length)
            );
            this._position = free[index];
        }
    }

    if (typeof module !== 'undefined' && module.exports) {
        module.exports = Food;
    } else {
        global.Food = Food;
    }
})(typeof window !== 'undefined' ? window : globalThis);
