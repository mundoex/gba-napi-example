const {GameBoyAdvance, KeyCode}=require("./lib/binding");
const path=require("path");
const {spawn} = require('child_process');
const fs = require('fs');

const HOME_DIR = require('os').homedir();
const EMULATOR_PATH=path.join(HOME_DIR, "Desktop", "VisualBoyAdvance.exe");
const ROM_PATH=path.join(HOME_DIR, "Desktop", "rom.gbc");
const SCREENSHOT_OUT_PATH=path.join(HOME_DIR, "Desktop", "screenshot.png");
const INPUT_DELAY_MS=200;

const emulatorProcess=spawn(EMULATOR_PATH, [ROM_PATH]);
emulatorProcess.on("spawn",()=>{
    //wait abit for window to pop
    setTimeout(()=>{
        const gameboy=new GameBoyAdvance(emulatorProcess.pid, INPUT_DELAY_MS);

        console.log("taking screenshot with buffer");
        //take screenshot
        gameboy.focus();
        const pngBuffer=gameboy.getPngBuffer();
        const byteBuffer=Buffer.from(pngBuffer);
        fs.writeFileSync(SCREENSHOT_OUT_PATH, byteBuffer);

        console.log("inputing A");
        //send input
        gameboy.focus();
        gameboy.pressKeyCode(KeyCode.GBA_A);
    },5_000, emulatorProcess);
});