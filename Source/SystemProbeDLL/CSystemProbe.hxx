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

#include "CGraphics.hxx"

#include <System/ZModule.hxx>

#define GLIDE3
#define __MSC__ _MSC_VER
#include <glide.h>

#include <gl/GL.h>

#define DIRECTDRAW_VERSION                      0x0700
#include <ddraw.h>

#define MAX_GRAPHICS_DEVICE_COUNT               4
#define MAX_GRAPHICS_SETTINGS_COUNT             9

#define GRAPHICS_LIBRARY_NOT_FOUND              1000
#define GRAPHICS_DIRECTDRAW_NOT_FOUND           1002
#define GRAPHICS_OPENGL_NOT_FOUND               1003
#define GRAPHICS_GLIDE3X_NOT_FOUND              1004
#define GRAPHICS_DIRECTDRAW_ENUMERATE_NOT_FOUND 2000

#define GRAPHICSCAPABILITY_NONE                 0x0
#define GRAPHICSCAPABILITY_DIRECTDRAW           0x1
#define GRAPHICSCAPABILITY_OPENGL               0x2
#define GRAPHICSCAPABILITY_GLIDE3X              0x4

#define PROCESSORCOUNTER_VERY_SLOW              400
#define PROCESSORCOUNTER_SLOW                   500
#define PROCESSORCOUNTER_MEDIUM                 650
#define PROCESSORCOUNTER_FAST                   750

enum ProcessorType {
    PROCESSORTYPE_NONE          = 0,
    PROCESSORTYPE_VERY_SLOW     = 1,
    PROCESSORTYPE_SLOW          = 2,
    PROCESSORTYPE_MEDIUM        = 3,
    PROCESSORTYPE_FAST          = 4,
    PROCESSORTYPE_VERY_FAST     = 5,
    PROCESSORTYPE_FORCE_DWORD   = 0x7FFFFFFF
};

#define SUFFICIENT_MEMORY_SIZE                  97000

enum MemorySufficiency {
    MEMORYSUFFICIENCY_UNKNOWN           = -1,
    MEMORYSUFFICIENCY_INSUFFICIENT      = 0,
    MEMORYSUFFICIENCY_SUFFICIENT        = 1,
    MEMORYSUFFICIENCY_FORCE_DWORD       = 0x7FFFFFFF
};

#pragma pack(push, 1)

// 0x0fbbb110
class CSystemProbe : public ZModule {
private:
    typedef HRESULT(WINAPI* LPDIRECTDRAWCREATE)(LPGUID, LPDIRECTDRAW*, LPUNKNOWN);
    typedef const GLubyte* (APIENTRY* LPGLGETSTRING)(GLenum name);
    typedef FxBool(FX_CALL* LPGRGET)(FxU32 pname, FxU32 plength, FxI32* params);

public:
    CSystemProbe();

public:
    virtual ~CSystemProbe();                                                            // 0x0
    virtual void LogSystemInfo();                                                       // 0x38
    virtual void PrintSystemInfo();                                                     // 0x3C
    virtual void SaveSystemInfo(const char* name);                                      // 0x40
    virtual u32 DetectAllCapabilities();                                                // 0x44
    virtual GraphicsAPI GetGraphicsAPI();                                               // 0x48
    virtual GraphicsResolution GetResolution();                                         // 0x4C
    virtual GraphicsColorDepth GetColorDepth();                                         // 0x50
    virtual GraphicsGenericSetting Method0x54();                                        // 0x54
    virtual GraphicsShadowQuality GetShadowQuality();                                   // 0x58
    virtual GraphicsTextureQuality GetTextureQuality();                                 // 0x5C
    virtual GraphicsGenericSetting GetAnisotropicFiltering();                           // 0x60
    virtual GraphicsGenericSetting GetAntiAliasing();                                   // 0x64
    virtual GraphicsGenericSetting GetTextureCompression();                             // 0x68
    virtual GraphicsGenericSetting GetPaletteCompression();                             // 0x6C
    virtual GraphicsGenericSetting GetVertexLighing();                                  // 0x70
    virtual GraphicsGenericSetting GetFullScreen();                                     // 0x74
    virtual CGraphicsSettings* GetSettings();                                           // 0x78
    virtual MemorySufficiency* GetMemorySufficieny();                                   // 0x7C
    virtual u32 DetectSystemCapabilities();                                             // 0x80

protected:
    u32 Capabilities;                                                                   // 0xC
    u32 DeviceCount;                                                                    // 0x10
    CGraphicsDevice Devices[MAX_GRAPHICS_DEVICE_COUNT];                                 // 0x14
    u32 Unk0x1D;                                                                        // 0x1D4
    CGraphicsSettings Active;                                                           // 0x1D8
    MemorySufficiency MemorySufficiency;                                                // 0x208
    u32 Unk0x20C;                                                                       // 0x20C
    u32 SettingsCount;                                                                  // 0x210
    CGraphicsSettings Settings[MAX_GRAPHICS_SETTINGS_COUNT];                            // 0x214
    f32 ProcessorCounter;                                                               // 0x3C4
    ProcessorType ProcessorType;                                                        // 0x3C8
    u32 SystemMemory; // In KB                                                          // 0x3CC
    u32 VideoMemory; // In KB                                                           // 0x3D0
    u32 GraphicsResult;                                                                 // 0x3D4
    CGraphicsPreset* Presets;                                                           // 0x3D8

protected:
    u32 InitializeDirectDraw();
    u32 EnumerateDirectDrawDevices();
    u32 InitializeOpenGL();
    u32 EnumerateOpenGLDevices();
    u32 InitializeGlide3();
    void DetectProcessor();
    f32 GetProcessorCycles();
    void DetectSystemMemory();
    u32 SetDefaultActiveSettings();
    u32 SetAllSettings();
    u32 SetActiveSettings();
    u32 AverageSettingValue(f32 value, u32 count);
    u32 SetVideoMemorySettings();
    u32 SelectVideoSettings(u32 size);
    u32 SetSystemMemorySettings();
    u32 SetProcessorSettings();
    u32 InitializePresets();
    u32 SetMemorySufficiency();
    u32 SetUnk0x20C();

    static BOOL WINAPI DirectDrawEnumerateCallback(LPGUID, LPSTR, LPSTR, LPVOID, HMONITOR);
};

#pragma pack(pop)

#if defined(_DEBUG) && !defined(_WIN64)
static_assert(sizeof(CSystemProbe) == 0x3DC, "CSystemProbe size mismatch.");
#endif
