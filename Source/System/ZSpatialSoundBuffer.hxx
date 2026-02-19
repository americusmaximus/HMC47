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

#include "ZPropertySet.hxx"

#pragma pack(push, 1)

class ZSpatialSoundBuffer : public ZPropertySet {
public:
    ZSpatialSoundBuffer();

public:
    virtual ~ZSpatialSoundBuffer();                                                     // 0x0
    virtual void Method0x8();                                                           // 0x8
    virtual bool Initialize(LPDIRECTSOUND ds, LPDIRECTSOUNDBUFFER buffer);              // 0x10
    virtual bool SetRoom(DWORD environment);                                            // 0x14
    virtual bool SetRoomSize(f32 size);                                                 // 0x18
    virtual void Method0x1C(u32 todo);                                                  // 0x1C

protected:
    LPDIRECTSOUNDBUFFER Buffer;                                                         // 0x8
    u32 Unk0xC;                                                                         // 0xC
    EAXLISTENERPROPERTIES Properties;                                                   // 0x10
    u32 Unk0x48;                                                                        // 0x48
};

#pragma pack(pop)

#if defined(_DEBUG) && !defined(_WIN64)
static_assert(sizeof(ZSpatialSoundBuffer) == 0x4C, "ZSpatialSoundBuffer size mismatch.");
#endif
