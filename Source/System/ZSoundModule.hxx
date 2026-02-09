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

#include "ZModule.hxx"

#pragma pack(push, 1)

class ZSoundModule : public ZModule {
public:
    ZSoundModule();

public:
    virtual ~ZSoundModule();                                                    // 0x0
    virtual void Initialize();                                                  // 0x8
    virtual void Release();                                                     // 0xC
    virtual void Method0x10();                                                  // 0x10
    virtual void Method0x14();                                                  // 0x14
    virtual void Method0x18();                                                  // 0x18
    virtual void Method0x1C();                                                  // 0x1C
    virtual void Method0x20();                                                  // 0x20
    virtual void Method0x28();                                                  // 0x28
    virtual void Method0x34();                                                  // 0x34
    virtual bool Method0x38();                                                  // 0x38
    virtual bool Method0x3C();                                                  // 0x3C
    virtual bool Method0x40();                                                  // 0x40

public:
    inline void* GetUnk0xC() {
        return this->Unk0xC;
    }

    inline void* GetUnk0x1C() {
        return this->Unk0x1C;
    }

protected:
    void* Unk0xC;                                                               // 0xC
    u32 Unk0x10;                                                                // 0x10
    u32 Unk0x14;                                                                // 0x14
    void* Unk0x18;                                                              // 0x18
    void* Unk0x1C;                                                              // 0x1C
};

#pragma pack(pop)

#if defined(_DEBUG) && !defined(_WIN64)
static_assert(sizeof(ZSoundModule)  == 0x20,    "ZSoundModule size mismatch.");
#endif
