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

#include "FastLookup.hxx"

#pragma pack(push, 1)

class ZSysActionContainer {
public:
    ZSysActionContainer();
    ~ZSysActionContainer();

public:
    virtual void Method0x0();                                                       // 0x0
    virtual void Method0x4();                                                       // 0x4
    virtual void Method0x8();                                                       // 0x8
    virtual void Method0xC();                                                       // 0xC
    virtual void Method0x10();                                                      // 0x10
    virtual void Method0x14();                                                      // 0x14
    virtual void Method0x18();                                                      // 0x18
    virtual void Method0x1C();                                                      // 0x1C
    virtual void Method0x20();                                                      // 0x20
    virtual void Method0x24();                                                      // 0x24
    virtual void Method0x28();                                                      // 0x28
    virtual void Method0x2C();                                                      // 0x2C
    virtual void Method0x30();                                                      // 0x30
    virtual void Method0x34();                                                      // 0x34
    virtual void Method0x38();                                                      // 0x38
    virtual void Method0x3C();                                                      // 0x3C
    virtual void Method0x40();                                                      // 0x40
    virtual void Method0x44();                                                      // 0x44
    virtual void Method0x48();                                                      // 0x48
    virtual void Method0x4C();                                                      // 0x4C
    virtual void Method0x50();                                                      // 0x50
    virtual void Method0x54();                                                      // 0x54

protected:
    u32 Unk0x4;                                                                     // 0x4
    u32 Unk0x8;                                                                     // 0x8
    u32 Unk0xC;                                                                     // 0xC
    u32 Unk0x10;                                                                    // 0x10
    RefTab* Unk0x14;                                                                // 0x14
    RefTab* Unk0x18;                                                                // 0x18
    u32 Unk0x1C;                                                                    // 0x1C
    f32 MouseWheel;                                                                 // 0x20
    u32 Unk0x24;                                                                    // 0x24
    u32 Unk0x28;                                                                    // 0x28
    FastLookup* Unk0x2C;                                                            // 0x2C
    u32 Unk0x30;                                                                    // 0x30
    u32 Unk0x34;                                                                    // 0x34
};

#pragma pack(pop)

#if defined(_DEBUG) && !defined(_WIN64)
static_assert(sizeof(ZSysActionContainer)   == 0x38,    "ZSysActionContainer size mismatch.");
#endif
