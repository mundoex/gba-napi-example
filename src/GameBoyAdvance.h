#pragma once

#include <napi.h>
#include "windows-helpers.h"
#include "./includes/lodepng/examples/bmp_buffer2png_buffer.h"

#define GBA_A 0x5A //z
#define GBA_B 0x58 //x
#define GBA_START VK_RETURN //enter
#define GBA_SELECT VK_BACK //backspace
#define GBA_L 0x41 //a
#define GBA_R 0x53 //s
#define GBA_UP VK_UP //up arrow
#define GBA_DOWN VK_DOWN //down arrow
#define GBA_RIGHT VK_RIGHT //right arrow
#define GBA_LEFT VK_LEFT //left arrow

class GameBoyAdvance : public Napi::ObjectWrap<GameBoyAdvance>
{
public:
    GameBoyAdvance(const Napi::CallbackInfo&);

	Napi::Value PressKeyCode(const Napi::CallbackInfo&);
	Napi::Value Focus(const Napi::CallbackInfo&);
    HBITMAP GetBitMap();
    Napi::Value GetPngBuffer(const Napi::CallbackInfo&);
	Napi::Value ScreenShot(const Napi::CallbackInfo&);

    static Napi::Function GetClass(Napi::Env);

private:
    int32_t m_Pid;
    int32_t m_InputDelay;
    HWND m_Window;
};