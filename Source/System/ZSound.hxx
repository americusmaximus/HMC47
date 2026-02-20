/*
Copyright (c) 2026 Americus Maximus

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once

#include "ZSoundBase.hxx"

#pragma pack(push, 1)

class ZSound : public ZSoundBase {
public:
    ZSound();

public:
    virtual bool Method0xC();                                                               // 0xC
    virtual ~ZSound();                                                                      // 0x2C
    virtual bool Init(HWND window);                                                         // 0x34
    virtual bool EndInit(const char* path);                                                 // 0x3C
    virtual bool RenderFrame();                                                             // 0x44
    virtual bool Method0x54(u32);                                                           // 0x54
    virtual void Method0xF4();                                                              // 0xF4
    virtual void Method0x114();                                                             // 0x114
    virtual void Method0x118();                                                             // 0x118
    virtual void Method0x11C();                                                             // 0x11C
    virtual void PrintStatus();                                                             // 0x128
    virtual void* Method0x198();                                                            // 0x198
    virtual void Method0x19C();                                                             // 0x19C
    virtual bool GetUseEAX();                                                               // 0x1CC
    virtual void SetUseHW(bool value);                                                      // 0x1D0
    virtual void SetUseEAX(bool value);                                                     // 0x1D4
    virtual bool CreatePrimaryBuffer();                                                     // 0x1D8
    virtual void Method0x1DC();                                                             // 0x1DC
    virtual void Method0x1E0();                                                             // 0x1E0

protected:
    bool FUN_0ff3d660();
    bool FUN_0ff3ed50();
    LPDIRECTSOUNDBUFFER InitSoundBuffer(u32 flags, const char* name, u32 size);

protected:
    bool UseEAX;                                                                            // 0x3F1
    LPDIRECTSOUND DirectSound;                                                              // 0x3F2
    bool Unk0x3F6;                                                                          // 0x3F6
    DSCAPS Caps;                                                                            // 0x3F7
    LPDIRECTSOUNDBUFFER DirectSoundBuffer;                                                  // 0x457
    u32 Unk0x45B;                                                                           // 0x45B
};

#pragma pack(pop)

#if defined(_DEBUG) && !defined(_WIN64)
static_assert(sizeof(ZSound)    == 0x45F,   "ZSound size mismatch.");
#endif
