/**
 * 蛇实体模块。
 *
 * 本模块定义蛇的数据结构与行为，包括蛇身的存储表示、移动与生长逻辑、
 * 方向控制以及碰撞检测。蛇身使用数组存储，队首（下标 0）为蛇头。
 * 坐标使用 { x, y } 点对象。渲染表现由渲染模块自行管理，与对象解耦。
 */

(function (global) {
    'use strict';

    const C = global.SnakeConstants || require('./constants.js');

    class Snake {
        /**
         * 初始化蛇。
         *
         * 蛇身从蛇头位置向移动方向的反方向延伸。
         *
         * Args:
         *     startX: 蛇头初始 x（游戏区域坐标）。
         *     startY: 蛇头初始 y（游戏区域坐标）。
         *     direction: 初始移动方向（'up' | 'down' | 'left' | 'right'）。
         *     length: 初始节数，默认取常量 INITIAL_SNAKE_LENGTH。
         */
        constructor(startX, startY, direction, length = C.INITIAL_SNAKE_LENGTH) {
            const vector = C.DIRECTION_VECTORS[direction];
            this._body = [];
            for (let i = 0; i < length; i++) {
                this._body.push({ x: startX - vector.x * i, y: startY - vector.y * i });
            }
            this._direction = direction;
            this._growPending = false;
        }

        /** 蛇头坐标 { x, y }。 */
        get head() {
            return this._body[0];
        }

        /** 蛇身坐标数组，下标 0 为蛇头，每项为 { x, y }。 */
        get body() {
            return this._body;
        }

        /** 当前移动方向。 */
        get direction() {
            return this._direction;
        }

        /** 蛇的节数。 */
        get length() {
            return this._body.length;
        }

        /**
         * 设置移动方向。
         *
         * 禁止 180 度掉头：若新方向与当前方向相反则忽略。
         *
         * Args:
         *     newDirection: 目标方向（'up' | 'down' | 'left' | 'right'）。
         */
        setDirection(newDirection) {
            if (newDirection === C.OPPOSITE_DIRECTION[this._direction]) {
                return;
            }
            this._direction = newDirection;
        }

        /**
         * 沿当前方向移动一格。
         *
         * 新蛇头插入队首；若无待处理的增长请求则移除队尾一节，
         * 实现蛇的正常滑动效果。
         */
        move() {
            const vector = C.DIRECTION_VECTORS[this._direction];
            const head = this._body[0];
            this._body.unshift({ x: head.x + vector.x, y: head.y + vector.y });
            if (this._growPending) {
                this._growPending = false;
            } else {
                this._body.pop();
            }
        }

        /** 标记在下次移动时增长一节（吃到食物时调用）。 */
        grow() {
            this._growPending = true;
        }

        /**
         * 判断蛇身是否占据给定坐标。
         *
         * Args:
         *     x: 格子 x 坐标。
         *     y: 格子 y 坐标。
         *
         * Returns:
         *     被占据时返回 true。
         */
        occupies(x, y) {
            return this._body.some(function (p) {
                return p.x === x && p.y === y;
            });
        }

        /**
         * 检测蛇头是否撞到自身。
         *
         * Returns:
         *     蛇头与身体任意一节重叠时返回 true。
         */
        checkSelfCollision() {
            const head = this._body[0];
            return this._body.slice(1).some(function (p) {
                return p.x === head.x && p.y === head.y;
            });
        }
    }

    if (typeof module !== 'undefined' && module.exports) {
        module.exports = Snake;
    } else {
        global.Snake = Snake;
    }
})(typeof window !== 'undefined' ? window : globalThis);
