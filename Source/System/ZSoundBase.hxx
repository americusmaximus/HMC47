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

#include "BlockRefTab.hxx"
#include "LinkRefTab.hxx"
#include "ZMusic.hxx"

#define DIRECTSOUND_VERSION 0x0600 
#include <dsound.h>

#pragma pack(push, 1)

class ZSoundBase {
public:
    ZSoundBase();

public:
    virtual void Method0x0();                                                               // 0x0
    virtual void Method0x4();                                                               // 0x4
    virtual void Method0x8();                                                               // 0x8
    virtual bool Method0xC() = 0;                                                           // 0xC
    virtual void Method0x10();                                                              // 0x10
    virtual void Method0x14();                                                              // 0x14
    virtual void Method0x18();                                                              // 0x18
    virtual void Method0x1C();                                                              // 0x1C
    virtual void Method0x20();                                                              // 0x20
    virtual void Method0x24();                                                              // 0x24
    virtual void Method0x28();                                                              // 0x28
    virtual ~ZSoundBase();                                                                  // 0x2C
    virtual bool Method0x30();                                                              // 0x30
    virtual BOOL Init(HWND) = 0;                                                            // 0x34
    virtual bool Method0x38();                                                              // 0x38
    virtual BOOL EndInit(const char*);                                                      // 0x3C
    virtual void Method0x40();                                                              // 0x40
    virtual BOOL RenderFrame() = 0;                                                         // 0x44
    virtual void Method0x48();                                                              // 0x48
    virtual void Method0x4C();                                                              // 0x4C
    virtual void Method0x50();                                                              // 0x50
    virtual BOOL Method0x54(u32);                                                           // 0x54
    virtual void Method0x58();                                                              // 0x58
    virtual void SetMusicVolume(f32 volume);                                                // 0x5C
    virtual void Method0x60();                                                              // 0x60
    virtual void SetSpeechVolume(f32 volume, bool todo);                                    // 0x64
    virtual void SetMusicQuality(s32 value);                                                // 0x68
    virtual void SetSfxVolume(f32 volume);                                                  // 0x6C
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
    virtual void AddSound();                                                                // 0x98
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
    virtual void SetStreaming(bool value);                                                  // 0x120
    virtual void Method0x124();                                                             // 0x124
    virtual void Method0x128();                                                             // 0x128
    virtual void Method0x12C();                                                             // 0x12C
    virtual void Method0x130();                                                             // 0x130
    virtual void Method0x134();                                                             // 0x134
    virtual void Method0x138();                                                             // 0x138
    virtual void Method0x13C();                                                             // 0x13C
    virtual void Method0x140();                                                             // 0x140
    virtual void Method0x144();                                                             // 0x144
    virtual void Method0x148();                                                             // 0x148
    virtual void Method0x14C();                                                             // 0x14C
    virtual void Method0x150();                                                             // 0x150
    virtual void Method0x154();                                                             // 0x154
    virtual void Method0x158();                                                             // 0x158
    virtual void Method0x15C();                                                             // 0x15C
    virtual void Method0x160();                                                             // 0x160
    virtual void Method0x164();                                                             // 0x164
    virtual void Method0x168();                                                             // 0x168
    virtual void Method0x16C();                                                             // 0x16C
    virtual void Method0x170();                                                             // 0x170
    virtual void Method0x174();                                                             // 0x174
    virtual void Method0x178();                                                             // 0x178
    virtual void Method0x17C();                                                             // 0x17C
    virtual void Method0x180();                                                             // 0x180
    virtual void Method0x184();                                                             // 0x184
    virtual void Method0x188();                                                             // 0x188
    virtual void Method0x18C();                                                             // 0x18C
    virtual void Method0x190();                                                             // 0x190
    virtual void Method0x194();                                                             // 0x194
    virtual void Method0x198();                                                             // 0x198
    virtual void Method0x19C();                                                             // 0x19C
    virtual void Method0x1A0();                                                             // 0x1A0
    virtual void Method0x1A4();                                                             // 0x1A4
    virtual void Method0x1A8();                                                             // 0x1A8
    virtual s32 Method0x1AC();                                                              // 0x1AC
    virtual bool Method0x1B0();                                                             // 0x1B0
    virtual bool HasMusic();                                                                // 0x1B4
    virtual void Method0x1B8();                                                             // 0x1B8
    virtual bool Method0x1BC();                                                             // 0x1BC
    virtual void Method0x1C0();                                                             // 0x1C0
    virtual void Method0x1C4();                                                             // 0x1C4
    virtual void Method0x1C8();                                                             // 0x1C8

protected:
    u32 Unk0x4;                                                                             // 0x4
    bool Unk0x8;                                                                            // 0x8
    u32 Unk0x9;                                                                             // 0x9
    f32 Unk0xD;                                                                             // 0xD
    u32 Unk0x11;                                                                            // 0x11
    u32 Unk0x15;                                                                            // 0x15
    u32 Unk0x19;                                                                            // 0x19
    BlockRefTab Unk0x1D;                                                                    // 0x1D
    u32 SpatialStreamingBuffers;                                                            // 0xC5
    bool Unk0xC9;                                                                           // 0xC9
    LinkRefTab* Unk0xCA;                                                                    // 0xCA
    BlockRefTab* Unk0xCE;                                                                   // 0xCE
    u32 Unk0xD2;                                                                            // 0xD2
    u32 Unk0xD6;                                                                            // 0xD6
    bool Unk0xDA;                                                                           // 0xDA
    u32 Unk0xDB;                                                                            // 0xDB
    bool Unk0xDF;                                                                           // 0xDF
    u32 Unk0xE0;                                                                            // 0xE0
    f32 Unk0xE4;                                                                            // 0xE4
    f32 MusicVolume;                                                                        // 0xE8
    u32 Unk0xEC;                                                                            // 0xEC
    f32 SfxVolume;                                                                          // 0xF0
    f32 SpeechVolume;                                                                       // 0xF4
    bool Unk0xF8;                                                                           // 0xF8
    bool UseStreaming;                                                                      // 0xF9
    u32 Unk0xFA;                                                                            // 0xFA
    u32 Unk0xFE;                                                                            // 0xFE
    u32 Unk0x102;                                                                           // 0x102
    u32 Unk0x106;                                                                           // 0x106
    bool Unk0x10A;                                                                          // 0x10A
    bool Unk0x10B;                                                                          // 0x10B
    u32 Unk0x10C;                                                                           // 0x10C
    u32 Unk0x110;                                                                           // 0x110
    u32 Unk0x114;                                                                           // 0x114
    u32 Unk0x118;                                                                           // 0x118
    u32 Unk0x11C;                                                                           // 0x11C
    u32 Unk0x120;                                                                           // 0x120
    u32 Unk0x124;                                                                           // 0x124
    LinkRefTab* Unk0x128;                                                                   // 0x128
    u32 Unk0x12C;                                                                           // 0x12C
    u32 Unk0x130;                                                                           // 0x130
    u32 Unk0x134;                                                                           // 0x134
    u32 Unk0x138;                                                                           // 0x138
    u32 Unk0x13C;                                                                           // 0x13C
    bool Unk0x140;                                                                          // 0x140
    ZMusic* Music;                                                                          // 0x141
    u32 Unk0x145;                                                                           // 0x145
    u32 Unk0x149;                                                                           // 0x149
    u32 Unk0x14D;                                                                           // 0x14D
    BlockRefTab Unk0x151;                                                                   // 0x151
    BlockRefTab Unk0x1F9;                                                                   // 0x1F9
    BlockRefTab Unk0x2A1;                                                                   // 0x2A1
    BlockRefTab Unk0x349;                                                                   // 0x349
};

#pragma pack(pop)

#if defined(_DEBUG) && !defined(_WIN64)
static_assert(sizeof(ZSoundBase)    == 0x3F1,   "ZSoundBase size mismatch.");
#endif
