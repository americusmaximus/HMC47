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

class ZEngineData {
public:
    ZEngineData();

public:
    virtual ~ZEngineData();                                                                 // 0x0
    virtual void Method0x4();                                                               // 0x4
    virtual void Method0x8();                                                               // 0x8
    virtual void Method0xC();                                                               // 0xC
    virtual void* Method0x10();                                                             // 0x10
    virtual void Method0x14();                                                              // 0x14
    virtual void Method0x18();                                                              // 0x18
    virtual void Method0x1C();                                                              // 0x1C
    virtual void Method0x20();                                                              // 0x20
    virtual void Method0x24();                                                              // 0x24
    virtual void Method0x28();                                                              // 0x28
    virtual void Method0x2C();                                                              // 0x2C
    virtual void Method0x30();                                                              // 0x30
    virtual void Method0x34();                                                              // 0x34
    virtual void Method0x38();                                                              // 0x38
    virtual void Method0x3C();                                                              // 0x3C
    virtual void Method0x40();                                                              // 0x40
    virtual void Method0x44();                                                              // 0x44
    virtual void Method0x48();                                                              // 0x48
    virtual void Method0x4C();                                                              // 0x4C
    virtual void Method0x50();                                                              // 0x50
    virtual void Method0x54();                                                              // 0x54
    virtual void Method0x58();                                                              // 0x58
    virtual void Method0x5C();                                                              // 0x5C
    virtual void Method0x60();                                                              // 0x60
    virtual void Method0x64();                                                              // 0x64
    virtual void Method0x68();                                                              // 0x68
    virtual void Method0x6C();                                                              // 0x6C
    virtual void Method0x70();                                                              // 0x70
    virtual void Method0x74();                                                              // 0x74
    virtual void Method0x78();                                                              // 0x78
    virtual void Method0x7C();                                                              // 0x7C
    virtual void Method0x80();                                                              // 0x80
    virtual void Method0x84();                                                              // 0x84
    virtual void Method0x88();                                                              // 0x88
    virtual void Method0x8C();                                                              // 0x8C
    virtual void Method0x90();                                                              // 0x90
    virtual void Method0x94();                                                              // 0x94
    virtual void Method0x98();                                                              // 0x98
    virtual void Method0x9C();                                                              // 0x9C
    virtual void Method0xA0();                                                              // 0xA0
    virtual void Method0xA4();                                                              // 0xA4
    virtual void Method0xA8();                                                              // 0xA8
    virtual void Method0xAC();                                                              // 0xAC
    virtual void Method0xB0();                                                              // 0xB0
    virtual void Method0xB4();                                                              // 0xB4
    virtual void Method0xB8();                                                              // 0xB8
    virtual void Method0xBC();                                                              // 0xBC
    virtual void Method0xC0();                                                              // 0xC0
    virtual void Method0xC4();                                                              // 0xC4
    virtual void Method0xC8();                                                              // 0xC8
    virtual void Method0xCC();                                                              // 0xCC
    virtual void Method0xD0();                                                              // 0xD0
    virtual void Method0xD4();                                                              // 0xD4
    virtual void Method0xD8();                                                              // 0xD8
    virtual void Method0xDC();                                                              // 0xDC
    virtual void Method0xE0();                                                              // 0xE0
    virtual void Method0xE4();                                                              // 0xE4
    virtual void Method0xE8();                                                              // 0xE8
    virtual void Method0xEC();                                                              // 0xEC
    virtual void Method0xF0();                                                              // 0xF0
    virtual void Method0xF4();                                                              // 0xF4
    virtual void Method0xF8();                                                              // 0xF8
    virtual void Method0xFC();                                                              // 0xFC
    virtual void Method0x100();                                                             // 0x100
    virtual void Method0x104();                                                             // 0x104
    virtual void Method0x108();                                                             // 0x108
    virtual void Method0x10C();                                                             // 0x10C
    virtual void Method0x110();                                                             // 0x110
    virtual void Method0x114();                                                             // 0x114
    virtual void Method0x118();                                                             // 0x118
    virtual void Method0x11C();                                                             // 0x11C
    virtual void Method0x120();                                                             // 0x120
    virtual void Method0x124();                                                             // 0x124
    virtual void Method0x128();                                                             // 0x128
    virtual void Method0x12C();                                                             // 0x12C
    virtual void Method0x130();                                                             // 0x130
    virtual void Method0x134();                                                             // 0x134
    virtual void Method0x138();                                                             // 0x138
    virtual void Method0x13C();                                                             // 0x13C

protected:
    void* Unk0x4;                                                                           // 0x4
    u32 Unk0x8;                                                                             // 0x8
    void* Unk0xC;                                                                           // 0xC
    
    bool Unk_0_15[15]; // TODO

    void* Unk0x1F;                                                                          // 0x1F

    bool Unk_0_14[14]; // TODO

    void* Unk0x31;                                                                          // 0x31
    void* Unk0x35;                                                                          // 0x35
    void* Unk0x39;                                                                          // 0x39
    void* Unk0x3D;                                                                          // 0x3D
    void* Unk0x41;                                                                          // 0x41
    void* Unk0x45;                                                                          // 0x45
    void* Unk0x49;                                                                          // 0x49
    void* Unk0x4D;                                                                          // 0x4D
    void* Unk0x51;                                                                          // 0x51
    void* Unk0x55;                                                                          // 0x55
    void* Unk0x59;                                                                          // 0x59
    void* Unk0x5D;                                                                          // 0x5D

    bool Unk_1_29[29]; // TODO

    void* Unk0x7E;                                                                          // 0x7E

    bool Unk_2_132[132]; // TODO

    void* Unk0x106;                                                                         // 0x106

    bool Unk_3_52[52]; // TODO

    u32 Unk0x13E;                                                                           // 0x13E
    u32 Unk0x142;                                                                           // 0x142
    bool Unk0x146;                                                                          // 0x146
    u32 Unk0x147;                                                                           // 0x147
    u32 Unk0x14B;                                                                           // 0x14B
    bool Unk0x14F;                                                                          // 0x14F
    void* Unk0x150;                                                                         // 0x150

    bool Unk_4_17[17]; // TODO

    void* Unk0x165;                                                                         // 0x165
    u32 Unk0x169;                                                                           // 0x169
    u32 Unk0x16D;                                                                           // 0x16D
    u32 Unk0x171;                                                                           // 0x171
    void* Unk0x175;                                                                         // 0x175

    bool Unk_5_72[72]; // TODO
};

#pragma pack(pop)

#if defined(_DEBUG) && !defined(_WIN64)
static_assert(sizeof(ZEngineData) == 0x1C1, "ZEngineData size mismatch.");
#endif
