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

#include "ZSpatialSoundBuffer.hxx"

class ZSound;

#pragma pack(push, 1)

class ZSoundListenerBaseBase { // TODO NAME
public:
    ZSoundListenerBaseBase();

public:
    virtual ~ZSoundListenerBaseBase();                                                      // 0x0
    virtual bool Method0x4();                                                               // 0x4
    virtual void Method0x8(u32 todo);                                                       // 0x8
    virtual u32 Method0xC();                                                                // 0xC

protected:
    u32 Unk0x4;                                                                             // 0x4
    u32 Unk0x8;                                                                             // 0x8
    u32 Unk0xC;                                                                             // 0xC
    u32 Unk0x10;                                                                            // 0x10
    u32 Unk0x14;                                                                            // 0x14
    u32 Unk0x18;                                                                            // 0x18
    u32 Unk0x1C;                                                                            // 0x1C
};

class ZSoundListenerBase : public ZSoundListenerBaseBase {
public:
    ZSoundListenerBase();

public:
    virtual ~ZSoundListenerBase();                                                          // 0x0
    virtual bool Method0x4();                                                               // 0x4
    virtual bool Initialize() = 0;                                                          // 0x10
    virtual void Release() = 0;                                                             // 0x14
    virtual bool SetRoomSize(f32 size);                                                     // 0x18
    virtual void SetRoom(DWORD environment);                                                // 0x1C
    virtual void Method0x20(u32 todo);                                                      // 0x20
    virtual u32 Method0x24();                                                               // 0x24

public:
    inline ZSound* GetSound() {
        return this->Sound;
    }

    inline void SetSound(ZSound* value) {
        this->Sound = value;
    }

protected:
    ZSound* Sound;                                                                          // 0x20
    u32 Unk0x24;                                                                            // 0x24
};

class ZSoundListener: public ZSoundListenerBase {
public:
    ZSoundListener();

public:
    virtual ~ZSoundListener();                                                              // 0x0
    virtual bool Method0x4();                                                               // 0x4
    virtual bool Initialize();                                                              // 0x10
    virtual void Release();                                                                 // 0x14
    virtual bool SetRoomSize(f32 size);                                                     // 0x18
    virtual void SetRoom(DWORD environment);                                                // 0x1C
    virtual void Method0x20(u32 todo);                                                      // 0x20
    virtual void SetBuffer(LPDIRECTSOUNDBUFFER buffer);                                     // 0x28

protected:
    LPDIRECTSOUND3DLISTENER Listener;                                                       // 0x28
    LPDIRECTSOUNDBUFFER Buffer;                                                             // 0x2C
    ZSpatialSoundBuffer* SpatialBuffer;                                                     // 0x30
};

#pragma pack(pop)

#if defined(_DEBUG) && !defined(_WIN64)
static_assert(sizeof(ZSoundListener)            == 0x34, "ZSoundListener size mismatch.");
static_assert(sizeof(ZSoundListenerBase)        == 0x28, "ZSoundListenerBase size mismatch.");
static_assert(sizeof(ZSoundListenerBaseBase)    == 0x20, "ZSoundListenerBaseBase size mismatch."); // TODO
#endif
