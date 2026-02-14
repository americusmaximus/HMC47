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

#include <System/BlockRefTab.hxx>
#include <System/ZString.hxx>

#include <dmusici.h>

#pragma pack(push, 1)

class ZMusicSegment {
public:
    ZMusicSegment();

public:
    virtual void Method0x0();                                                       // 0x0
    virtual void Method0x4();                                                       // 0x4
    virtual void Method0x8();                                                       // 0x8

public:
    BlockRefTab Bands;                                                              // 0x4
    IDirectMusicStyle* Style;                                                       // 0xAC
    u32 Unk0xB0;                                                                    // 0xB0
    u32 Unk0xB4;                                                                    // 0xB4
    ZString File;                                                                   // 0xB8
    ZString Directory;                                                              // 0x138
    IDirectMusicSegment* Segment;                                                   // 0x1B8
};

#pragma pack(pop)

#if defined(_DEBUG) && !defined(_WIN64)
static_assert(sizeof(ZMusicSegment) == 0x1BC,   "ZMusicSegment size mismatch.");
#endif
