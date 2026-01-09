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
    GraphicsDeviceAcceleration  Acceleration;
    GraphicsDeviceType          Type;
    u32                         Memory;     // In KB
    char                        Name[MAX_GRAPHICS_DEVICE_NAME_LENGTH];
    char                        Driver[MAX_GRAPHICS_DEVICE_NAME_LENGTH];
};

struct CGraphicsSettings {
    GraphicsAPI                     API;
    GraphicsResolution              Resolution;
    GraphicsColorDepth              ColorDepth;
    GraphicsShadowQuality           ShadowQuality;
    GraphicsTextureQuality          TextureQuality;
    GraphicsGenericSetting          Unk14; // TODO
    GraphicsGenericSetting          AnisotropicFiltering;
    GraphicsGenericSetting          AntiAliasing;
    GraphicsGenericSetting          TextureCompression;
    GraphicsGenericSetting          PaletteCompression;
    GraphicsGenericSetting          VertexLighing;
    GraphicsGenericSetting          FullScreen;
};

class CGraphicsPreset {
public:
    CGraphicsPreset();
    ~CGraphicsPreset();
    CGraphicsPreset* GetNext();

public:
    CGraphicsPreset* Next;
    char Name[MAX_GRAPHICS_DEVICE_NAME_LENGTH];
    CGraphicsSettings Settings;
};

#pragma pack(pop)

#if defined(_DEBUG) && !defined(_WIN64)
static_assert(sizeof(CGraphicsDevice) == 0x70,      "CGraphicsDevice size mismatch.");
static_assert(sizeof(CGraphicsPreset) == 0x66,      "CGraphicsPreset size mismatch.");
static_assert(sizeof(CGraphicsSettings) == 0x30,    "CGraphicsSettings size mismatch.");
#endif
