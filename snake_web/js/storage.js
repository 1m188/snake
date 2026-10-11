/**
 * 最高分存储模块。
 *
 * 本模块负责历史最高分的读写，底层使用 localStorage 持久化。
 * 对存储不可用（如隐私模式、file:// 受限）的场景安全降级：
 * 读取返回 0，写入静默忽略。存储实例可注入以便测试。
 */

(function (global) {
    'use strict';

    const C = global.SnakeConstants || require('./constants.js');

    class HighScoreStore {
        /**
         * 创建最高分存储。
         *
         * Args:
         *     storage: Storage 兼容对象（需提供 getItem/setItem），
         *              默认使用浏览器 localStorage，不可用时为 null。
         */
        constructor(storage) {
            this._storage =
                storage !== undefined
                    ? storage
                    : typeof localStorage !== 'undefined'
                      ? localStorage
                      : null;
        }

        /**
         * 读取历史最高分。
         *
         * Returns:
         *     最高分；无记录、值无效或存储不可用时返回 0。
         */
        get() {
            if (!this._storage) {
                return 0;
            }
            try {
                const value = parseInt(this._storage.getItem(C.HIGH_SCORE_KEY), 10);
                return Number.isFinite(value) && value > 0 ? value : 0;
            } catch (err) {
                return 0;
            }
        }

        /**
         * 保存本局得分，仅当它高于历史最高分时写入。
         *
         * Args:
         *     score: 本局得分。
         */
        save(score) {
            if (!this._storage) {
                return;
            }
            try {
                if (score > this.get()) {
                    this._storage.setItem(C.HIGH_SCORE_KEY, String(score));
                }
            } catch (err) {
                // 存储不可用时静默忽略，不影响游戏进行
            }
        }
    }

    if (typeof module !== 'undefined' && module.exports) {
        module.exports = HighScoreStore;
    } else {
        global.HighScoreStore = HighScoreStore;
    }
})(typeof window !== 'undefined' ? window : globalThis);
