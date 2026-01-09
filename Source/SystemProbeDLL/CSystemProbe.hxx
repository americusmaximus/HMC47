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

#include <System/ZDynamicLoader.hxx>

#define GLIDE3
#define __MSC__ _MSC_VER
#include <glide.h>

#include <gl/GL.h>

#define DIRECTDRAW_VERSION                  0x0700
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
class CSystemProbe : public ZDynamicLoader {
private:
    typedef HRESULT(WINAPI* LPDIRECTDRAWCREATE)(LPGUID, LPDIRECTDRAW*, LPUNKNOWN);
    typedef const GLubyte* (APIENTRY* LPGLGETSTRING)(GLenum name);
    typedef FxBool(FX_CALL* LPGRGET)(FxU32 pname, FxU32 plength, FxI32* params);

public:
    CSystemProbe();

public:
    virtual ~CSystemProbe();
    virtual void LogSystemInfo();
    virtual void PrintSystemInfo();
    virtual void SaveSystemInfo(const char* name);
    virtual u32 DetectAllCapabilities();
    virtual GraphicsAPI GetGraphicsAPI();
    virtual GraphicsResolution GetResolution();
    virtual GraphicsColorDepth GetColorDepth();
    virtual GraphicsGenericSetting GetUnk14(); // TODO
    virtual GraphicsShadowQuality GetShadowQuality();
    virtual GraphicsTextureQuality GetTextureQuality();
    virtual GraphicsGenericSetting GetAnisotropicFiltering();
    virtual GraphicsGenericSetting GetAntiAliasing();
    virtual GraphicsGenericSetting GetTextureCompression();
    virtual GraphicsGenericSetting GetPaletteCompression();
    virtual GraphicsGenericSetting GetVertexLighing();
    virtual GraphicsGenericSetting GetFullScreen();
    virtual CGraphicsSettings* GetSettings();
    virtual MemorySufficiency* GetMemorySufficieny();
    virtual u32 DetectSystemCapabilities();

protected:
    u32 Capabilities;
    u32 DeviceCount;
    CGraphicsDevice Devices[MAX_GRAPHICS_DEVICE_COUNT];
    u32 Unk0x1D; // TODO
    CGraphicsSettings Active;
    MemorySufficiency MemorySufficiency;
    u32 Unk0x20C; // TODO
    u32 SettingsCount;
    CGraphicsSettings Settings[MAX_GRAPHICS_SETTINGS_COUNT];
    f32 ProcessorCounter;
    ProcessorType ProcessorType;
    u32 SystemMemory; // In KB
    u32 VideoMemory; // In KB
    u32 GraphicsResult;
    CGraphicsPreset* Presets;

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
    u32 SetUnk0x20C(); // TODO

    static BOOL WINAPI DirectDrawEnumerateCallback(LPGUID, LPSTR, LPSTR, LPVOID, HMONITOR);
};

#pragma pack(pop)

#if defined(_DEBUG) && !defined(_WIN64)
static_assert(sizeof(CSystemProbe) == 0x3DC, "CSystemProbe size mismatch.");
#endif
