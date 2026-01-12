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

#include "Globals.hxx"

#define MAX_GRAPHICS_DEVICE_NAME_LENGTH     50

enum GraphicsDeviceAcceleration {
    GRAPHICSDEVICEACCELERATION_OPENGL           = 0,
    GRAPHICSDEVICEACCELERATION_DIRECTDRAW3D     = 1,
    GRAPHICSDEVICEACCELERATION_DIRECTDRAW2D     = 2,
    GRAPHICSDEVICEACCELERATION_FORCE_DWORD      = 0x7FFFFFFF
};

enum GraphicsDeviceType {
    GRAPHICSDEVICETYPE_DIRECTDRAW               = 0,
    GRAPHICSDEVICETYPE_OPENGL                   = 1,
    GRAPHICSDEVICETYPE_GLIDE3X                  = 2,
    GRAPHICSDEVICETYPE_FORCE_DWORD              = 0x7FFFFFFF
};

#pragma pack(push, 1)

struct CGraphicsDevice {
    GraphicsDeviceAcceleration  Acceleration;                                           // 0x0
    GraphicsDeviceType          Type;                                                   // 0x4
    u32                         Memory;     // In KB                                    // 0x8
    char                        Name[MAX_GRAPHICS_DEVICE_NAME_LENGTH];                  // 0xC
    char                        Driver[MAX_GRAPHICS_DEVICE_NAME_LENGTH];                // 0x3E
};

struct CGraphicsSettings {
    GraphicsAPI                     API;                                                // 0x0
    GraphicsResolution              Resolution;                                         // 0x4
    GraphicsColorDepth              ColorDepth;                                         // 0x8
    GraphicsShadowQuality           ShadowQuality;                                      // 0xC
    GraphicsTextureQuality          TextureQuality;                                     // 0x10
    GraphicsGenericSetting          Unk0x14;                                            // 0x14
    GraphicsGenericSetting          AnisotropicFiltering;                               // 0x18
    GraphicsGenericSetting          AntiAliasing;                                       // 0x1C
    GraphicsGenericSetting          TextureCompression;                                 // 0x20
    GraphicsGenericSetting          PaletteCompression;                                 // 0x24
    GraphicsGenericSetting          VertexLighing;                                      // 0x28
    GraphicsGenericSetting          FullScreen;                                         // 0x2C
};

class CGraphicsPreset {
public:
    CGraphicsPreset();
    ~CGraphicsPreset();
    CGraphicsPreset* GetNext();

public:
    CGraphicsPreset* Next;                                                              // 0x0
    char Name[MAX_GRAPHICS_DEVICE_NAME_LENGTH];                                         // 0x4
    CGraphicsSettings Settings;                                                         // 0x36
};

#pragma pack(pop)

#if defined(_DEBUG) && !defined(_WIN64)
static_assert(sizeof(CGraphicsDevice)   == 0x70,    "CGraphicsDevice size mismatch.");
static_assert(sizeof(CGraphicsPreset)   == 0x66,    "CGraphicsPreset size mismatch.");
static_assert(sizeof(CGraphicsSettings) == 0x30,    "CGraphicsSettings size mismatch.");
#endif
