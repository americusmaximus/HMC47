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

#include "ZConsoleCommand.hxx"

#pragma pack(push, 1)

class ZConsole {
public:
    ZConsole();
    ~ZConsole();

public:
    void Method0x0();                                                       // 0x0
    void Method0x4();                                                       // 0x4
    void Method0x8();                                                       // 0x8
    void Method0xC();                                                       // 0xC
    void Method0x10();                                                      // 0x10
    void Method0x14();                                                      // 0x14
    void Method0x18();                                                      // 0x18
    void Method0x1C();                                                      // 0x1C
    void Method0x20();                                                      // 0x20
    void Method0x24();                                                      // 0x24
    void Method0x28();                                                      // 0x28
    void Method0x2C();                                                      // 0x2C
    void Method0x30();                                                      // 0x20
    void Method0x34();                                                      // 0x34

protected:
    // TODO
};

#pragma pack(pop)

#if defined(_DEBUG) && !defined(_WIN64)
static_assert(sizeof(ZConsole)  == 0x10F4,  "ZConsole size mismatch.");
#endif
