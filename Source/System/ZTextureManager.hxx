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

class ZTextureManager {
public:
    ZTextureManager();

public:
    virtual ~ZTextureManager();                                                         // 0x0
    virtual void Method0x4() = 0;                                                       // 0x4
    virtual void Method0x8() = 0;                                                       // 0x8
    virtual void Method0xC() = 0;                                                       // 0xC
    virtual void Method0x10() = 0;                                                      // 0x10
    virtual void Method0x14() = 0;                                                      // 0x14
    virtual u32 Method0x18(u32);                                                        // 0x18
    virtual void Method0x1C() = 0;                                                      // 0x1C
    virtual void Method0x20() = 0;                                                      // 0x20
    virtual void Method0x24() = 0;                                                      // 0x24
    virtual void Method0x28() = 0;                                                      // 0x28
    virtual void Method0x2C() = 0;                                                      // 0x2C
    virtual void Method0x30() = 0;                                                      // 0x30
};

#pragma pack(pop)
