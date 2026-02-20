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

#include "RefTab.hxx"

#include <dmusicc.h>
#include <dmusici.h>

#pragma pack(push, 1)

enum ZMUSICQUALITY {
    ZMUSICQUALITY_NORMAL    = 0,
    ZMUSICQUALITY_LOW       = 1
};

class ZMusic {
public:
    ZMusic();

public:
    virtual bool CreateStreamingBuffer();                                               // 0x0
    virtual ~ZMusic();                                                                  // 0x4
    virtual bool Initialize(LPDIRECTSOUND ds, HWND window);                             // 0x8
    virtual void Method0xC(s32 todo);                                                   // 0xC
    virtual void StopPerformance(u32);                                                  // 0x10
    virtual void Method0x14();                                                          // 0x14
    virtual void CreateSegment(const char* dir, const char* file, u32 todo, u32 todo);  // 0x18
    virtual void Method0x1C(void* todo);                                                // 0x1C
    virtual s32 Method0x20(RefTab* todo);                                              // 0x20
    virtual void Method0x24(RefTab* todo);                                              // 0x24
    virtual void* Method0x28(s32 todo);                                                 // 0x28
    virtual bool IsInit();                                                              // 0x2C
    virtual void PrintStatus();                                                         // 0x30
    virtual void SetQuality(ZMUSICQUALITY value);                                       // 0x34
    virtual void SetVolume(f32 value);                                                  // 0x38

protected:
    IDirectMusicPort* CreateDirectMusicPort();

protected:
    s32 Unk0x4;                                                                         // 0x4
    LPDIRECTSOUND DirectSound;                                                          // 0x8
    LPDIRECTSOUNDBUFFER DirectSoundBuffer;                                              // 0xC
    void* Unk0x10;                                                                      // 0x10
    bool Init;                                                                          // 0x14
    ZMUSICQUALITY Quality;                                                              // 0x15
    IDirectMusicPerformance* DirectMusicPerformance;                                    // 0x16
    IDirectMusicLoader* DirectMusicLoader;                                              // 0x1A
    RefTab* Segments;                                                                   // 0x1E
    LPDIRECTMUSIC DirectMusic;                                                          // 0x22
    IDirectMusicPort* DirectMusicPort;                                                  // 0x26
};

#pragma pack(pop)

#if defined(_DEBUG) && !defined(_WIN64)
static_assert(sizeof(ZMusic)    == 0x2A,    "ZMusic size mismatch.");
#endif
