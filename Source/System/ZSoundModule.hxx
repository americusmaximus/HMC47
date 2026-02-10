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

#include "LinkRefTab.hxx"
#include "ZModule.hxx"
#include "ZSound.hxx"

#pragma pack(push, 1)

struct ZSoundModuleConfiguration {
    u32 Unk0x0;                                                                 // 0x0
    u32 Unk0x4;                                                                 // 0x4
    u32 Unk0x8;                                                                 // 0x8
    u32 Unk0xC;                                                                 // 0xC
    u32 Buffers;                                                                // 0x10
    u8 HW;                                                                      // 0x14
    u8 EAX;                                                                     // 0x15
    u8 Unk0x16;                                                                 // 0x16
};

class ZSoundModule : public ZModule {
public:
    ZSoundModule();

public:
    virtual ~ZSoundModule();                                                    // 0x0
    virtual void Initialize();                                                  // 0x8
    virtual void Release();                                                     // 0xC
    virtual void Method0x10(const char* todo);                                  // 0x10
    virtual void Method0x14();                                                  // 0x14
    virtual void BeginInit();                                                   // 0x18
    virtual void EndInit();                                                     // 0x1C
    virtual void Method0x20();                                                  // 0x20
    virtual void Method0x28();                                                  // 0x28
    virtual void Method0x34();                                                  // 0x34
    virtual bool RenderFrame();                                                 // 0x38
    virtual void SetConfiguration(u8 hw, u8 eax, u32 buffers);                  // 0x3C
    virtual s32 Method0x40();                                                   // 0x40

public:
    inline ZSound* GetSound() {
        return this->Sound;
    }

protected:
    ZSoundModuleConfiguration* Configuration;                                   // 0xC
    s32 Unk0x10;                                                                // 0x10
    u32 Unk0x14;                                                                // 0x14
    LinkRefTab* Unk0x18;                                                        // 0x18
    ZSound* Sound;                                                              // 0x1C
};

#pragma pack(pop)

#if defined(_DEBUG) && !defined(_WIN64)
static_assert(sizeof(ZSoundModule)              == 0x20,    "ZSoundModule size mismatch.");
static_assert(sizeof(ZSoundModuleConfiguration) == 0x17,    "ZSoundModuleConfiguration size mismatch.");
#endif
