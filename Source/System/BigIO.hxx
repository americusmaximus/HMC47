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

#include "Common.hxx"

#pragma pack(push, 1)

/*
It seems that BigIO and ZipIO should have somewhat similar inheritance structures.
*/

class BigIO {
public:
    BigIO();

public:
    virtual void Method0x0();                                                       // 0x0
    virtual void Method0x4(u32, u32);                                               // 0x4
    virtual void Method0x8(u32);                                                    // 0x8
    virtual u32 Method0xC(u32);                                                     // 0xC
    virtual bool Exists(const char*);                                               // 0x10
    virtual u32 Method0x14(u32, u32, u32, u32);                                     // 0x14
    virtual ~BigIO();                                                               // 0x18

protected:
    u32 Mode;                                                                       // 0x4
    u32 Unk0x8;                                                                     // 0x8
    u32 Unk0xC;                                                                     // 0xC
    u32 Unk0x10;                                                                    // 0x10
    u32 Unk0x14;                                                                    // 0x14
    u32 Unk0x18;                                                                    // 0x18

public:
    char* Name;                                                                     // 0x1C
    bool Init;                                                                      // 0x20
};

#pragma pack(pop)

#if defined(_DEBUG) && !defined(_WIN64)
static_assert(sizeof(BigIO)     == 0x21,    "BigIO size mismatch.");
#endif
