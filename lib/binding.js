const addon = require('../build/Release/object-wrap-demo-native');

function GameBoyAdvance(pid, delay) {
    this.focus = function() {
        return _addonInstance.focus();
    }

    this.pressKeyCode = function(keyCode){
        return _addonInstance.pressKeyCode(keyCode);
    }

    this.getPngBuffer = function(){
        return _addonInstance.getPngBuffer();
    }

    this.screenshot = function(filePath){
        return _addonInstance.screenshot(filePath);
    }

    var _addonInstance = new addon.GameBoyAdvance(pid, delay);
}

module.exports.GameBoyAdvance = GameBoyAdvance;
module.exports.KeyCode = {
    GBA_A: addon.GBA_A,
    GBA_B: addon.GBA_B,
    GBA_R: addon.GBA_R,
    GBA_L: addon.GBA_L,
    GBA_SELECT: addon.GBA_SELECT,
    GBA_START: addon.GBA_START,
    GBA_RIGHT: addon.GBA_RIGHT,
    GBA_LEFT: addon.GBA_LEFT,
    GBA_UP: addon.GBA_UP,
    GBA_DOWN: addon.GBA_DOWN,
}