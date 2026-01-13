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

#include <dplay.h>

#pragma pack(push, 1)

class ZDirectPlay {
public:
    ZDirectPlay();

public:
    virtual ~ZDirectPlay();                                                             // 0x0
    virtual void Initialize();                                                          // 0x4
    virtual void Release();                                                             // 0x8
    virtual void Enumerate();                                                           // 0xC
    virtual bool HasSession();                                                          // 0x10
    virtual void OpenSession(u32 count);                                                // 0x14
    virtual void OpenUntitledSession();                                                 // 0x18

protected:
    bool Active;                                                                        // 0x4
    char* SessionName;                                                                  // 0x5
    char* Unk0x9;                                                                       // 0x9
    char* Unk0xD;                                                                       // 0xD
    u32 Unk0x11;                                                                        // 0x11
    LPDIRECTPLAY2 Instance;                                                             // 0x15
    GUID* ID;                                                                           // 0x19
};

#pragma pack(pop)

#if defined(_DEBUG) && !defined(_WIN64)
static_assert(sizeof(ZDirectPlay)   == 0x1D,    "ZDirectPlay size mismatch.");
#endif
