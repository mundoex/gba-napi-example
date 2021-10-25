#include "GameBoyAdvance.h"
#include <iostream>
using namespace Napi;

GameBoyAdvance::GameBoyAdvance(const Napi::CallbackInfo& info) : ObjectWrap(info) {
    Napi::Env env = info.Env();

    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
        return;
    }

    if (!info[0].IsNumber()) {
        Napi::TypeError::New(env, "pid must be a number").ThrowAsJavaScriptException();
        return;
    }

    if (!info[1].IsNumber()) {
        Napi::TypeError::New(env, "delay must be a number").ThrowAsJavaScriptException();
        return;
    }

    this->m_Pid = info[0].As<Napi::Number>().Int32Value();
    this->m_InputDelay = info[1].As<Napi::Number>().Int32Value();

    HWND window = NULL;
	int found = GetVisibleWindowFromProcessID(this->m_Pid, window);
	if (found > 0) this->m_Window = window;
}

HBITMAP GameBoyAdvance::GetBitMap()
{
	HDC hScreenDC = GetDC(this->m_Window); // CreateDC("DISPLAY",nullptr,nullptr,nullptr);
	HDC hMemoryDC = CreateCompatibleDC(hScreenDC);
	int width, height;
	std::tie(width, height) = GetHWNDSize(this->m_Window);

	HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, width, height);
	HBITMAP hOldBitmap = static_cast<HBITMAP>(SelectObject(hMemoryDC, hBitmap));
	BitBlt(hMemoryDC, 0, 0, width, height, hScreenDC, 0, 0, SRCCOPY);
	hBitmap = static_cast<HBITMAP>(SelectObject(hMemoryDC, hOldBitmap));
	DeleteDC(hMemoryDC);
	DeleteDC(hScreenDC);
	return hBitmap;
}

Napi::Value GameBoyAdvance::Focus(const Napi::CallbackInfo& info){
    SetForegroundWindow(this->m_Window);
	SetFocus(this->m_Window);
    return info.Env().Null();
}

Napi::Value GameBoyAdvance::GetPngBuffer(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();

    //bitmap
    this->Focus(info);
    HBITMAP bitmap=this->GetBitMap();

    //bitmap -> bitmap bytes
    std::vector<unsigned char> bitmapBytes=std::vector<unsigned char>();
    long width, height;
    GetHBITMAPPixelBytes(bitmap, width, height, bitmapBytes);
    
    // bitmap bytes -> png
    std::vector<unsigned char> pngBytes=std::vector<unsigned char>();
	unsigned long uWidth = 0;
	unsigned long uHeight = 0;
	BMPBufferToPNGBuffer(pngBytes, uWidth, uHeight, bitmapBytes);
    
    std::vector<unsigned char>* png=new std::vector<unsigned char>();
    lodepng::encode(*png, pngBytes, uWidth, uHeight);
    Napi::ArrayBuffer buffer=Napi::ArrayBuffer::New(env, png->data(), png->size());
    return buffer;
}

Napi::Value GameBoyAdvance::ScreenShot(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();

    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
        return env.Null();
    }

    if (!info[0].IsString()) {
        Napi::TypeError::New(env, "file path must be a string").ThrowAsJavaScriptException();
        return env.Null();
    }

    std::string filePath = info[0].ToString().Utf8Value();

    //bitmap
    this->Focus(info);
    HBITMAP bitmap=this->GetBitMap();

    //bitmap -> bitmap bytes
    std::vector<unsigned char> bitmapBytes=std::vector<unsigned char>();
    long width, height;
    GetHBITMAPPixelBytes(bitmap, width, height, bitmapBytes);

    // bitmap bytes -> png
    std::vector<unsigned char> pngBytes=std::vector<unsigned char>();
	unsigned long uWidth = 0;
	unsigned long uHeight = 0;
	BMPBufferToPNGBuffer(pngBytes, uWidth, uHeight, bitmapBytes);
    
    std::vector<unsigned char>* png=new std::vector<unsigned char>();
    lodepng::encode(*png, pngBytes, uWidth, uHeight);
    lodepng_save_file(png->data(), png->size(), filePath.c_str());
    return env.Null();
}

Napi::Value GameBoyAdvance::PressKeyCode(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();

    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
        return env.Null();
    }

    if (!info[0].IsNumber()) {
        Napi::TypeError::New(env, "keycode must be a number").ThrowAsJavaScriptException();
        return env.Null();
    }

    const int keyCode=info[0].As<Napi::Number>().Int32Value();

    INPUT ipDown;
	ipDown.type = INPUT_KEYBOARD;
	ipDown.ki.wScan = 0;
	ipDown.ki.dwFlags = 0;
	ipDown.ki.wVk = keyCode;
	ipDown.ki.dwExtraInfo = 0;
	ipDown.ki.time = 0;

	INPUT ipUp;
	ipUp.type = INPUT_KEYBOARD;
	ipUp.ki.wScan = 0;
	ipUp.ki.dwFlags = KEYEVENTF_KEYUP;
	ipUp.ki.wVk = keyCode;
	ipUp.ki.dwExtraInfo = 0;
	ipDown.ki.time = 0;

	SendInput(1, &ipDown, sizeof(INPUT));
	Sleep(m_InputDelay);
	SendInput(1, &ipUp, sizeof(INPUT));
	Sleep(m_InputDelay/2);

    return env.Null();
}

// Napi::Value GameBoyAdvance::Greet(const Napi::CallbackInfo& info) {
//     Napi::Env env = info.Env();

//     // if (info.Length() < 1) {
//     //     Napi::TypeError::New(env, "Wrong number of arguments")
//     //       .ThrowAsJavaScriptException();
//     //     return env.Null();
//     // }

//     // if (!info[0].IsString()) {
//     //     Napi::TypeError::New(env, "You need to introduce yourself to greet")
//     //       .ThrowAsJavaScriptException();
//     //     return env.Null();
//     // }

//     return Napi::Number::New(env, this->m_Pid);
// }

Napi::Function GameBoyAdvance::GetClass(Napi::Env env) {
    return DefineClass(env, "GameBoyAdvance", {
       GameBoyAdvance::InstanceMethod("focus", &GameBoyAdvance::Focus),
       GameBoyAdvance::InstanceMethod("pressKeyCode", &GameBoyAdvance::PressKeyCode),
       GameBoyAdvance::InstanceMethod("getPngBuffer", &GameBoyAdvance::GetPngBuffer),
       GameBoyAdvance::InstanceMethod("screenshot", &GameBoyAdvance::ScreenShot)
    });
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    //export class
    exports.Set(Napi::String::New(env, "GameBoyAdvance"), GameBoyAdvance::GetClass(env));

    //export defined gba keys
    exports.Set(Napi::String::New(env, "GBA_A"), Napi::Number::New(env, GBA_A));
    exports.Set(Napi::String::New(env, "GBA_B"), Napi::Number::New(env, GBA_B));
    exports.Set(Napi::String::New(env, "GBA_L"), Napi::Number::New(env, GBA_L));
    exports.Set(Napi::String::New(env, "GBA_R"), Napi::Number::New(env, GBA_R));
    exports.Set(Napi::String::New(env, "GBA_START"), Napi::Number::New(env, GBA_START));
    exports.Set(Napi::String::New(env, "GBA_SELECT"), Napi::Number::New(env, GBA_SELECT));
    exports.Set(Napi::String::New(env, "GBA_UP"), Napi::Number::New(env, GBA_UP));
    exports.Set(Napi::String::New(env, "GBA_DOWN"), Napi::Number::New(env, GBA_DOWN));
    exports.Set(Napi::String::New(env, "GBA_LEFT"), Napi::Number::New(env, GBA_LEFT));
    exports.Set(Napi::String::New(env, "GBA_RIGHT"), Napi::Number::New(env, GBA_RIGHT));
    return exports;
}

NODE_API_MODULE(addon, Init)