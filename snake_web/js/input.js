/**
 * 输入模块。
 *
 * 本模块负责把原始输入事件（键盘、触屏滑动、虚拟方向键）翻译成
 * 与具体游戏状态无关的三种意图，交给装配层分发：
 *
 * - onDirection(dir)：方向意图（WASD / 方向键 / 滑动 / 方向键按钮）。
 * - onPrimary()：主操作意图（空格 / 回车 / 画布轻点 / 中央按钮），
 *   含义随游戏状态变化：开始、暂停 / 继续、结束后重开。
 * - onBack()：返回意图（ESC）。
 *
 * 本模块不直接操作游戏对象，保持与游戏逻辑解耦。
 */

(function (global) {
    'use strict';

    /** 键盘按键到方向的映射（大小写均支持）。 */
    const KEY_TO_DIRECTION = Object.freeze({
        ArrowUp: 'up',
        ArrowDown: 'down',
        ArrowLeft: 'left',
        ArrowRight: 'right',
        w: 'up',
        s: 'down',
        a: 'left',
        d: 'right',
        W: 'up',
        S: 'down',
        A: 'left',
        D: 'right',
    });

    /** 触屏滑动判定阈值（像素），低于该值视为轻点。 */
    const SWIPE_THRESHOLD_PX = 24;

    class InputController {
        /**
         * 创建输入控制器并绑定事件。
         *
         * Args:
         *     handlers: 意图回调集合
         *               { onDirection(dir), onPrimary(), onBack() }。
         *     canvas: 触摸滑动监听的画布元素，可不传。
         *     touchControls: 虚拟方向键容器元素，传入时在触屏设备上显示。
         */
        constructor(handlers, canvas, touchControls) {
            this._handlers = handlers;
            this._bindKeyboard();
            if (canvas) {
                this._bindTouchSwipe(canvas);
            }
            if (touchControls) {
                this._bindTouchButtons(touchControls);
                if (isTouchDevice()) {
                    touchControls.hidden = false;
                }
            }
        }

        /** 绑定键盘事件：方向、主操作、返回。 */
        _bindKeyboard() {
            document.addEventListener('keydown', function (event) {
                const direction = KEY_TO_DIRECTION[event.key];
                if (direction) {
                    event.preventDefault();
                    this._handlers.onDirection(direction);
                    return;
                }
                if (event.key === ' ' || event.key === 'Enter') {
                    event.preventDefault();
                    this._handlers.onPrimary();
                    return;
                }
                if (event.key === 'Escape') {
                    this._handlers.onBack();
                }
            }.bind(this));
        }

        /** 绑定画布触摸滑动：滑动为方向意图，轻点为主操作意图。 */
        _bindTouchSwipe(canvas) {
            let startX = 0;
            let startY = 0;
            let tracking = false;

            canvas.addEventListener('touchstart', function (event) {
                const touch = event.changedTouches[0];
                startX = touch.clientX;
                startY = touch.clientY;
                tracking = true;
            }, { passive: true });

            // 阻止滑动画布时页面跟随滚动
            canvas.addEventListener('touchmove', function (event) {
                event.preventDefault();
            }, { passive: false });

            canvas.addEventListener('touchend', function (event) {
                if (!tracking) {
                    return;
                }
                tracking = false;
                const touch = event.changedTouches[0];
                const dx = touch.clientX - startX;
                const dy = touch.clientY - startY;
                if (Math.max(Math.abs(dx), Math.abs(dy)) < SWIPE_THRESHOLD_PX) {
                    this._handlers.onPrimary();
                    return;
                }
                if (Math.abs(dx) > Math.abs(dy)) {
                    this._handlers.onDirection(dx > 0 ? 'right' : 'left');
                } else {
                    this._handlers.onDirection(dy > 0 ? 'down' : 'up');
                }
            }.bind(this));
        }

        /** 绑定虚拟方向键按钮：点击产生方向或主操作意图。 */
        _bindTouchButtons(container) {
            container.addEventListener('click', function (event) {
                const button = event.target.closest('[data-intent]');
                if (!button) {
                    return;
                }
                const intent = button.getAttribute('data-intent');
                if (intent === 'primary') {
                    this._handlers.onPrimary();
                } else {
                    this._handlers.onDirection(intent);
                }
            }.bind(this));
        }
    }

    /** 判断当前设备是否支持触屏。 */
    function isTouchDevice() {
        return 'ontouchstart' in global || (global.navigator.maxTouchPoints || 0) > 0;
    }

    if (typeof module !== 'undefined' && module.exports) {
        module.exports = InputController;
    } else {
        global.InputController = InputController;
    }
})(typeof window !== 'undefined' ? window : globalThis);
