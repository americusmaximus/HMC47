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

#define MAX_RENDER_DEVICE_NAME_LENGTH       40

#pragma pack(push, 1)

struct ZRenderDevice {
    char Name[MAX_RENDER_DEVICE_NAME_LENGTH];                               // 0x00
    LPGUID IDD3D;                                                           // 0x28
    D3DDEVICEDESC7 Desc;                                                    // 0x2C
    u32 Acceleration;                                                       // 0x118
    LPGUID IDDD;                                                            // 0x11C
    DDCAPS HAL;                                                             // 0x120
    DDCAPS HEL;                                                             // 0x29C
    DDSURFACEDESC2 Active;                                                  // 0x418
    u32 Unk0x494;                                                           // 0x494
    u32 Unk0x498;                                                           // 0x498
    GUID IDD3DValue;                                                        // 0x49C
    GUID IDDDValue;                                                         // 0x4AC
    DDSURFACEDESC2* Modes;                                                  // 0x4BC
    u32 ModeCount;                                                          // 0x4C0
    u32 Index;                                                              // 0x4C4
    BOOL Window;                                                            // 0x4C8
    BOOL Stereo;                                                            // 0x4CC
};

#pragma pack(pop)

#if defined(_DEBUG) && !defined(_WIN64)
static_assert(sizeof(ZRenderDevice)     == 0x4D0,   "ZRenderDevice size mismatch.");
#endif
