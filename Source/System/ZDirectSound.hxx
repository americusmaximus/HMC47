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

#include "ZSound.hxx"

#pragma pack(push, 1)

class ZDirectSound : public ZSound {
public:
    ZDirectSound();

public:
    virtual ~ZDirectSound();                                                                // 0x2C
    virtual BOOL Init(HWND window);                                                         // 0x34
    virtual BOOL EndInit(const char* path);                                                 // 0x3C
    virtual void AddSound();                                                                // 0x98
    virtual void Method0x12C();                                                             // 0x12C
    virtual void Method0x130();                                                             // 0x130
    virtual void Method0x134();                                                             // 0x134
    virtual void Method0x138();                                                             // 0x138
    virtual void Method0x13C();                                                             // 0x13C
    virtual void Method0x140();                                                             // 0x140
    virtual void Method0x144();                                                             // 0x144
    virtual void Method0x1E4();                                                             // 0x1E4
};

#pragma pack(pop)

#if defined(_DEBUG) && !defined(_WIN64)
static_assert(sizeof(ZDirectSound)  == 0x45F,   "ZDirectSound size mismatch.");
#endif
