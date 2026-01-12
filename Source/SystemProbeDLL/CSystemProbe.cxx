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

#include "CSystemProbe.hxx"

#include <stdio.h>

static u32 g_DirectDrawDeviceCount;     // 0x0fbbf310
static u32 g_GraphicsDeviceCount;       // 0x0fbbf330

static HMODULE g_Glide3XModule;         // 0x0fbbf318
static HMODULE g_OpenGLModule;          // 0x0fbbf31c
static HMODULE g_DirectDrawModule;      // 0x0fbbf320

#define MAX_SETTINGS_COUNT          30
#define MAX_SETTINGS_VALUE_LENGTH   10

// 0x0fbbc040
static const char SettingNames[MAX_SETTINGS_COUNT][MAX_SETTINGS_VALUE_LENGTH] = {
    // CPU
    "INVALID",
    "VERY_SLOW",
    "SLOW",
    "MEDIUM",
    "FAST",
    "VERY_FAST",

    // Resolution
    "640x480",
    "800x600",
    "1024x768",
    "1280x1024",
    "1600x1200",

    // TODO
    "Low",
    "Normal",
    "High",

    // Texture Quality
    "Very low",
    "Low",
    "Medium",
    "Full",

    // Shadow Quality
    "None",
    "Hitman",
    "All",

    // API
    "DirectX",
    "OpenGL",
    "Glide3x",

    //
    "Off",
    "On",

    // Bits
    "16bit",
    "32bit"
};

// 0x0fbb1030
CSystemProbe::CSystemProbe() {
    g_DirectDrawModule = NULL;
    g_OpenGLModule = NULL;
    g_Glide3XModule = NULL;

    this->Capabilities = GRAPHICSCAPABILITY_NONE;
    this->ProcessorCounter = 0.0f;
    this->ProcessorType = PROCESSORTYPE_NONE;
    this->SystemMemory = 0;
    this->VideoMemory = 0;
    this->GraphicsResult = EXIT_SUCCESS;
    this->Unk0x1D = 0; // TODO
    this->DeviceCount = 0;

    g_DirectDrawDeviceCount = 0;

    ZeroMemory(this->Devices, sizeof(CGraphicsDevice) * MAX_GRAPHICS_DEVICE_COUNT);

    {
        this->Active.API                    = GRAPHICSAPI_UNKNOWN;
        this->Active.Resolution             = GRAPHICSRESOLUTION_UNKNOWN;
        this->Active.ColorDepth             = GRAPHICSCOLORDEPTH_UNKNOWN;
        this->Active.ShadowQuality          = GRAPHICSSHADOWQUALITY_UNKNOWN;
        this->Active.TextureQuality         = GRAPHICSTEXTUREQUALITY_UNKNOWN;
        this->Active.Unk0x14                = GRAPHICSGENERICSETTING_UNKNOWN;
        this->Active.AnisotropicFiltering   = GRAPHICSGENERICSETTING_UNKNOWN;
        this->Active.AntiAliasing           = GRAPHICSGENERICSETTING_UNKNOWN;
        this->Active.TextureCompression     = GRAPHICSGENERICSETTING_UNKNOWN;
        this->Active.PaletteCompression     = GRAPHICSGENERICSETTING_UNKNOWN;
        this->Active.VertexLighing          = GRAPHICSGENERICSETTING_UNKNOWN;
        this->Active.FullScreen             = GRAPHICSGENERICSETTING_UNKNOWN;
    }

    this->SettingsCount = 0;

    for (u32 i = 0; i < MAX_GRAPHICS_SETTINGS_COUNT; i++) {
        this->Settings[i].API                   = GRAPHICSAPI_UNKNOWN;
        this->Settings[i].Resolution            = GRAPHICSRESOLUTION_UNKNOWN;
        this->Settings[i].ColorDepth            = GRAPHICSCOLORDEPTH_UNKNOWN;
        this->Settings[i].ShadowQuality         = GRAPHICSSHADOWQUALITY_UNKNOWN;
        this->Settings[i].TextureQuality        = GRAPHICSTEXTUREQUALITY_UNKNOWN;
        this->Settings[i].Unk0x14               = GRAPHICSGENERICSETTING_UNKNOWN;
        this->Settings[i].AnisotropicFiltering  = GRAPHICSGENERICSETTING_UNKNOWN;
        this->Settings[i].AntiAliasing          = GRAPHICSGENERICSETTING_UNKNOWN;
        this->Settings[i].TextureCompression    = GRAPHICSGENERICSETTING_UNKNOWN;
        this->Settings[i].PaletteCompression    = GRAPHICSGENERICSETTING_UNKNOWN;
        this->Settings[i].VertexLighing         = GRAPHICSGENERICSETTING_UNKNOWN;
        this->Settings[i].FullScreen            = GRAPHICSGENERICSETTING_UNKNOWN;
    }

    this->Presets = nullptr;

    this->InitializePresets();

    this->MemorySufficiency = MEMORYSUFFICIENCY_UNKNOWN;
    this->Unk0x20C = -1; // TODO
}

// 0x0fbb1130
GraphicsAPI CSystemProbe::GetGraphicsAPI() {
    return this->Active.API;
}

// 0x0fbb1140
GraphicsResolution CSystemProbe::GetResolution() {
    return this->Active.Resolution;
}

// 0x0fbb1150
GraphicsColorDepth CSystemProbe::GetColorDepth() {
    return this->Active.ColorDepth;
}

// 0x0fbb1160
GraphicsGenericSetting CSystemProbe::Method0x54() {
    return this->Active.Unk0x14;
}

// 0x0fbb1170
GraphicsShadowQuality CSystemProbe::GetShadowQuality() {
    return this->Active.ShadowQuality;
}

// 0x0fbb1180
GraphicsTextureQuality CSystemProbe::GetTextureQuality() {
    return this->Active.TextureQuality;
}

// 0x0fbb1190
GraphicsGenericSetting CSystemProbe::GetAnisotropicFiltering() {
    return this->Active.AnisotropicFiltering;
}

// 0x0fbb11a0
GraphicsGenericSetting CSystemProbe::GetAntiAliasing() {
    return this->Active.AntiAliasing;
}

// 0x0fbb11b0
GraphicsGenericSetting CSystemProbe::GetTextureCompression() {
    return this->Active.TextureCompression;
}

// 0x0fbb11c0
GraphicsGenericSetting CSystemProbe::GetPaletteCompression() {
    return this->Active.PaletteCompression;
}

// 0x0fbb11d0
GraphicsGenericSetting CSystemProbe::GetVertexLighing() {
    return this->Active.VertexLighing;
}

// 0x0fbb11e0
GraphicsGenericSetting CSystemProbe::GetFullScreen() {
    return this->Active.FullScreen;
}

// 0x0fbb11f0
CGraphicsSettings* CSystemProbe::GetSettings() {
    return &this->Active;
}

// 0x0fbb1200
MemorySufficiency* CSystemProbe::GetMemorySufficieny() {
    return &this->MemorySufficiency;
}

// 0x0fbb1210
// 0x0fbb1270
CSystemProbe::~CSystemProbe() {
    if (this->Presets != nullptr) {
        delete this->Presets;
    }
}

// 0x0fbb12e0
u32 CSystemProbe::DetectAllCapabilities() {
    this->DetectProcessor();
    this->InitializeDirectDraw();
    this->InitializeOpenGL();
    this->InitializeGlide3();
    this->DetectSystemMemory();
    this->SetAllSettings();
    this->SetActiveSettings();
    this->SetDefaultActiveSettings();

    return EXIT_SUCCESS;
}

// 0x0fbb1320
u32 CSystemProbe::DetectSystemCapabilities() {
    this->DetectProcessor();
    this->DetectSystemMemory();
    this->SetMemorySufficiency();
    this->SetUnk0x20C();

    return EXIT_SUCCESS;
}

// 0x0fbb1350
void CSystemProbe::LogSystemInfo() {
    for (u32 i = 0; i < this->DeviceCount; i++) {
        CGraphicsDevice* device = &this->Devices[i];

        switch (device->Type) {
        case GRAPHICSDEVICETYPE_DIRECTDRAW: {
            g_pSysCom->Log("Z:\\SystemProbeDll\\CSystemProbeDLL.cpp", 214)
                ->LogMessage("CSystemProbeDLL: Found DirectX graphics device\n- %s\n- %s\n",
                    device->Name, device->Driver);

            break;
        }
        case GRAPHICSDEVICETYPE_OPENGL: {
            g_pSysCom->Log("Z:\\SystemProbeDll\\CSystemProbeDLL.cpp", 216)
                ->LogMessage("CSystemProbeDLL: Found OpenGL graphics device\n- %s\n- Renderer %s\n",
                    device->Name, device->Driver);

            break;
        }
        case GRAPHICSDEVICETYPE_GLIDE3X: {
            g_pSysCom->Log("Z:\\SystemProbeDll\\CSystemProbeDLL.cpp", 218)
                ->LogMessage("CSystemProbeDLL: Found Glide3D graphics device\n- %s\n\n", device->Name);

            break;
        }
        }
    }

    if (this->Capabilities & GRAPHICSCAPABILITY_DIRECTDRAW) {
        g_pSysCom->Log("Z:\\SystemProbeDll\\CSystemProbeDLL.cpp", 223)
            ->LogMessage("CSystemProbeDLL: DirectX detected\n");
    }
    else {
        g_pSysCom->Log("Z:\\SystemProbeDll\\CSystemProbeDLL.cpp", 225)
            ->LogMessage("CSystemProbeDLL: DirectX NOT detected\n");
    }

    if (this->Capabilities & GRAPHICSCAPABILITY_OPENGL) {
        g_pSysCom->Log("Z:\\SystemProbeDll\\CSystemProbeDLL.cpp", 231)
            ->LogMessage("CSystemProbeDLL: OpenGL detected\n");
    }
    else {
        g_pSysCom->Log("Z:\\SystemProbeDll\\CSystemProbeDLL.cpp", 233)
            ->LogMessage("CSystemProbeDLL: OpenGL NOT detected\n");
    }

    if (this->Capabilities & GRAPHICSCAPABILITY_GLIDE3X) {
        g_pSysCom->Log("Z:\\SystemProbeDll\\CSystemProbeDLL.cpp", 238)
            ->LogMessage("CSystemProbeDLL: Glide3D detected\n");
    }
    else {
        g_pSysCom->Log("Z:\\SystemProbeDll\\CSystemProbeDLL.cpp", 240)
            ->LogMessage("CSystemProbeDLL: Glide3D NOT detected\n");
    }

    g_pSysCom->Log("Z:\\SystemProbeDll\\CSystemProbeDLL.cpp", 250)
        ->LogMessage("CSystemProbeDLL: Total System Memory %i(Kb)\n", this->SystemMemory);

    if (this->VideoMemory != 0) {
        g_pSysCom->Log("Z:\\SystemProbeDll\\CSystemProbeDLL.cpp", 253)
            ->LogMessage("CSystemProbeDLL: Total Video Memory %i(Kb)\n", this->VideoMemory);
    }

    g_pSysCom->Log("Z:\\SystemProbeDll\\CSystemProbeDLL.cpp", 256)
        ->LogMessage("CSystemProbeDLL: Cycles per second %f\n", this->ProcessorCounter);
    g_pSysCom->Log("Z:\\SystemProbeDll\\CSystemProbeDLL.cpp", 257)
        ->LogMessage("CSystemProbeDLL: Processor speed ~ %s\n", SettingNames[this->ProcessorType]);
}

// 0x0fbb15b0
void CSystemProbe::PrintSystemInfo() {
    for (u32 i = 0; i < this->DeviceCount; i++) {
        CGraphicsDevice* device = &this->Devices[i];

        switch (device->Type) {
        case GRAPHICSDEVICETYPE_DIRECTDRAW: {
            printf("CSystemProbeDLL: Found DirectX graphics device\n- %s\n- %s\n", device->Name, device->Driver);
            break;
        }
        case GRAPHICSDEVICETYPE_OPENGL: {
            printf("CSystemProbeDLL: Found OpenGL graphics device\n- %s\n- Renderer %s\n", device->Name, device->Driver);
            break;
        }
        case GRAPHICSDEVICETYPE_GLIDE3X: {
            printf("CSystemProbeDLL: Found Glide3D graphics device\n- %s\n\n", device->Name);
            break;
        }
        }
    }

    printf("CSystemProbeDLL");
    printf((this->Capabilities & GRAPHICSCAPABILITY_DIRECTDRAW)
        ? "DirectX detected," : "DirectX NOT detected,");
    printf((this->Capabilities & GRAPHICSCAPABILITY_OPENGL)
        ? "OpenGL detected," : "OpenGL NOT detected,");
    printf((this->Capabilities & GRAPHICSCAPABILITY_GLIDE3X)
        ? "Glide3D detected\n" : "Glide3D NOT detected\n");

    printf("CSystemProbeDLL: Total System Memory %i(Kb)\n", this->SystemMemory);

    if (this->VideoMemory != 0) {
        printf("CSystemProbeDLL: Total Video Memory %i(Kb)\n", this->VideoMemory);
    }

    printf("CSystemProbeDLL: Cycles per second %f (%s)\n",
        this->ProcessorCounter, SettingNames[this->ProcessorType]);
}

// 0x0fbb16e0
void CSystemProbe::SaveSystemInfo(const char* name) {
    FILE* file = fopen(name, "w");

    for (u32 i = 0; i < this->DeviceCount; i++) {
        CGraphicsDevice* device = &this->Devices[i];

        switch (device->Type) {
        case GRAPHICSDEVICETYPE_DIRECTDRAW: {
            fprintf(file, "CSystemProbeDLL: Found DirectX graphics device\n- %s\n- %s\n", device->Name, device->Driver);
            break;
        }
        case GRAPHICSDEVICETYPE_OPENGL: {
            fprintf(file, "CSystemProbeDLL: Found OpenGL graphics device\n- %s\n- Renderer %s\n", device->Name, device->Driver);
            break;
        }
        case GRAPHICSDEVICETYPE_GLIDE3X: {
            fprintf(file, "CSystemProbeDLL: Found Glide3D graphics device\n- %s\n\n", device->Name);
            break;
        }
        }
    }

    fprintf(file, (this->Capabilities & GRAPHICSCAPABILITY_DIRECTDRAW)
        ? "CSystemProbeDLL: DirectX detected\n" : "CSystemProbeDLL: DirectX NOT detected\n");
    fprintf(file, (this->Capabilities & GRAPHICSCAPABILITY_OPENGL)
        ? "CSystemProbeDLL: OpenGL detected\n" : "CSystemProbeDLL: OpenGL NOT detected\n");
    fprintf(file, (this->Capabilities & GRAPHICSCAPABILITY_GLIDE3X)
        ? "CSystemProbeDLL: Glide3D detected\n" : "CSystemProbeDLL: Glide3D NOT detected\n");

    fprintf(file, "CSystemProbeDLL: Total System Memory %i(Kb)\n", this->SystemMemory);

    if (this->VideoMemory != 0) {
        fprintf(file, "CSystemProbeDLL: Total Video Memory %i(Kb)\n", this->VideoMemory);
    }

    printf("CSystemProbeDLL: Cycles per second %f\n", this->ProcessorCounter);
    printf("CSystemProbeDLL: Processor speed ~ %s\n", SettingNames[this->ProcessorType]);

    fclose(file);
}

// 0x0fbb1840
u32 CSystemProbe::InitializeDirectDraw() {
    bool load = false;

    g_DirectDrawModule = GetModuleHandleA("DDRAW");

    if (g_DirectDrawModule == NULL) {
        load = true;
        g_DirectDrawModule = LoadLibraryA("ddraw.dll");

        if (g_DirectDrawModule == NULL) {
            this->GraphicsResult = GRAPHICS_DIRECTDRAW_NOT_FOUND;
            return GRAPHICS_LIBRARY_NOT_FOUND;
        }
    }

    if (EnumerateDirectDrawDevices() == EXIT_SUCCESS) {
        this->Capabilities |= GRAPHICSCAPABILITY_DIRECTDRAW;

        if (load) {
            FreeLibrary(g_DirectDrawModule);
        }

        return EXIT_SUCCESS;
    }

    if (load) {
        FreeLibrary(g_DirectDrawModule);
    }

    return GRAPHICS_DIRECTDRAW_NOT_FOUND;
}

// 0x0fbb18d0
u32 CSystemProbe::EnumerateDirectDrawDevices() {
    LPDIRECTDRAWENUMERATEEX action =
        (LPDIRECTDRAWENUMERATEEX)GetProcAddress(g_DirectDrawModule, "DirectDrawEnumerateExA");

    if (action != nullptr) {
        if (action(DirectDrawEnumerateCallback, this->Devices,
            DDENUM_DETACHEDSECONDARYDEVICES | DDENUM_ATTACHEDSECONDARYDEVICES) == DD_OK) {
            this->DeviceCount = g_GraphicsDeviceCount;
            return EXIT_SUCCESS;
        }
    }

    this->GraphicsResult = GRAPHICS_DIRECTDRAW_ENUMERATE_NOT_FOUND;

    return GRAPHICS_LIBRARY_NOT_FOUND;
}

// 0x0fbb1920
BOOL WINAPI CSystemProbe::DirectDrawEnumerateCallback(LPGUID lpGUID, LPSTR, LPSTR, LPVOID lpContext, HMONITOR) {
    CGraphicsDevice* devices = (CGraphicsDevice*)lpContext;

    LPDIRECTDRAWCREATE action =
        (LPDIRECTDRAWCREATE)GetProcAddress(g_DirectDrawModule, "DirectDrawCreate");

    LPDIRECTDRAW dd = NULL;
    if (action(lpGUID, &dd, NULL) == DD_OK) {
        LPDIRECTDRAW7 dd7 = NULL;
        if (dd->QueryInterface(IID_IDirectDraw7, (LPVOID*)&dd7) == DD_OK) {
            DDCAPS caps;
            ZeroMemory(&caps, sizeof(DDCAPS));
            caps.dwSize = sizeof(DDCAPS);

            dd7->GetCaps(&caps, NULL);

            DDDEVICEIDENTIFIER2 ident;
            ZeroMemory(&ident, sizeof(DDDEVICEIDENTIFIER2));

            dd7->GetDeviceIdentifier(&ident, 0);

            devices[g_GraphicsDeviceCount].Type = GRAPHICSDEVICETYPE_DIRECTDRAW;
            devices[g_GraphicsDeviceCount].Memory = caps.dwVidMemTotal / 1024;

            strncpy(devices[g_GraphicsDeviceCount].Name,
                ident.szDescription, MAX_GRAPHICS_DEVICE_NAME_LENGTH);
            devices[g_GraphicsDeviceCount].Name[MAX_GRAPHICS_DEVICE_NAME_LENGTH - 1] = NULL;

            strncpy(devices[g_GraphicsDeviceCount].Driver,
                ident.szDriver, MAX_GRAPHICS_DEVICE_NAME_LENGTH);
            devices[g_GraphicsDeviceCount].Driver[MAX_GRAPHICS_DEVICE_NAME_LENGTH - 1] = NULL;

            devices[g_GraphicsDeviceCount].Acceleration = (caps.dwCaps & DDCAPS_3D)
                ? GRAPHICSDEVICEACCELERATION_DIRECTDRAW3D : GRAPHICSDEVICEACCELERATION_DIRECTDRAW2D;

            if (dd7 != NULL) {
                dd7->Release();
            }

            g_DirectDrawDeviceCount = g_DirectDrawDeviceCount + 1;

            if (g_GraphicsDeviceCount < MAX_GRAPHICS_DEVICE_COUNT) {
                g_GraphicsDeviceCount = g_GraphicsDeviceCount + 1;
                return TRUE;
            }
        }
    }

    return FALSE;
}

// 0x0fbb1b80
u32 CSystemProbe::InitializeOpenGL() {
    bool load = false;
    g_OpenGLModule = GetModuleHandleA("OPENGL32");

    if (g_OpenGLModule == NULL) {
        load = true;
        g_OpenGLModule = LoadLibraryA("OPENGL32.DLL");

        if (g_OpenGLModule == NULL) {
            this->GraphicsResult = GRAPHICS_OPENGL_NOT_FOUND;
            return GRAPHICS_LIBRARY_NOT_FOUND;
        }
    }

    if (this->EnumerateOpenGLDevices() == EXIT_SUCCESS) {
        this->Capabilities |= GRAPHICSCAPABILITY_OPENGL;

        if (load) {
            FreeLibrary(g_OpenGLModule);
        }

        return EXIT_SUCCESS;
    }

    this->GraphicsResult = GRAPHICS_OPENGL_NOT_FOUND;

    if (load) {
        FreeLibrary(g_OpenGLModule);
    }

    return GRAPHICS_LIBRARY_NOT_FOUND;
}

// 0x0fbb1c10
u32 CSystemProbe::EnumerateOpenGLDevices() {
    HGLRC glc = NULL;

    if (g_pSysInterface == nullptr) {
        PIXELFORMATDESCRIPTOR desc;
        ZeroMemory(&desc, sizeof(PIXELFORMATDESCRIPTOR));

        desc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
        desc.nVersion = 1;
        desc.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
        desc.cColorBits = 16;
        desc.cDepthBits = 32;

        HDC hdc = GetDC(NULL);
        const s32 format = ChoosePixelFormat(hdc, &desc);
        SetPixelFormat(hdc, format, &desc);
        glc = wglCreateContext(hdc);

        if (glc != NULL && !wglMakeCurrent(hdc, glc)) {
            wglDeleteContext(glc);
            this->GraphicsResult = GRAPHICS_OPENGL_NOT_FOUND;
            return GRAPHICS_LIBRARY_NOT_FOUND;
        }
    }

    LPGLGETSTRING getString =
        (LPGLGETSTRING)GetProcAddress(g_OpenGLModule, "glGetString");

    const char* vendor = (const char*)getString(GL_VENDOR);

    if (vendor != nullptr) {
        strncpy(this->Devices[g_GraphicsDeviceCount].Name, vendor, MAX_GRAPHICS_DEVICE_NAME_LENGTH);
        this->Devices[g_GraphicsDeviceCount].Name[MAX_GRAPHICS_DEVICE_NAME_LENGTH - 1] = NULL;
    }

    const char* renderer = (const char*)getString(GL_RENDERER);

    if (renderer != nullptr) {
        strncpy(this->Devices[g_GraphicsDeviceCount].Driver, renderer, MAX_GRAPHICS_DEVICE_NAME_LENGTH);
        this->Devices[g_GraphicsDeviceCount].Driver[MAX_GRAPHICS_DEVICE_NAME_LENGTH - 1] = NULL;
    }

    this->Devices[g_GraphicsDeviceCount].Type = GRAPHICSDEVICETYPE_OPENGL;
    this->Devices[g_GraphicsDeviceCount].Acceleration = GRAPHICSDEVICEACCELERATION_OPENGL;

    this->DeviceCount = this->DeviceCount + 1;

    if (g_pSysInterface == nullptr) {
        wglDeleteContext(glc);
    }

    return EXIT_SUCCESS;
}

// 0x0fbb1df0
u32 CSystemProbe::InitializeGlide3() {
    if (g_pSysInterface->Render != nullptr) {
        if (_strcmpi("GLIDE3", g_pSysInterface->Render->GetRenderName()) == 0) {
            this->Capabilities |= GRAPHICSCAPABILITY_GLIDE3X;
            return EXIT_SUCCESS;
        }
    }

    bool load = false;

    g_Glide3XModule = GetModuleHandleA("GLIDE3X");

    if (g_Glide3XModule == NULL) {
        load = true;
        g_Glide3XModule = LoadLibraryA("GLIDE3X.DLL");

        if (g_Glide3XModule == NULL) {
            return GRAPHICS_GLIDE3X_NOT_FOUND;
        }
    }

    LPGRGET action =
        (LPGRGET)GetProcAddress(g_Glide3XModule, "_grGet@12");

    if (action == nullptr) {
        if (load) {
            FreeLibrary(g_Glide3XModule);
        }

        return GRAPHICS_LIBRARY_NOT_FOUND;
    }

    u32 count = 0;
    action(GR_NUM_FB, sizeof(u32), (FxI32*)&count);

    if (count != 0) {
        this->Capabilities |= GRAPHICSCAPABILITY_GLIDE3X;

        if (load) {
            FreeLibrary(g_Glide3XModule);
        }

        return GRAPHICS_LIBRARY_NOT_FOUND;
    }

    if (load) {
        FreeLibrary(g_Glide3XModule);
    }

    return EXIT_SUCCESS;
}

// 0x0fbb1ee0
void CSystemProbe::DetectProcessor() {
    this->ProcessorCounter = GetProcessorCycles();

    const u32 count = (u32)(this->ProcessorCounter / 1000000);

    if (count < PROCESSORCOUNTER_VERY_SLOW) {
        this->ProcessorType = PROCESSORTYPE_VERY_SLOW;
    }
    else if (count < PROCESSORCOUNTER_SLOW) {
        this->ProcessorType = PROCESSORTYPE_SLOW;
    }
    else if (count < PROCESSORCOUNTER_MEDIUM) {
        this->ProcessorType = PROCESSORTYPE_MEDIUM;
    }
    else if (count < PROCESSORCOUNTER_FAST) {
        this->ProcessorType = PROCESSORTYPE_FAST;
    }
    else {
        this->ProcessorType = PROCESSORTYPE_VERY_FAST;
    }
}

// 0x0fbb1f70
f32 CSystemProbe::GetProcessorCycles() {
    if (g_pSysInterface) {
        return (f32)g_pSysInterface->ProcessorCounter;
    }

    const u64 cycles = __rdtsc();

    Sleep(1000);

    return (f32)(__rdtsc() - cycles);
}

// 0x0fbb1fe0
void CSystemProbe::DetectSystemMemory() {
    MEMORYSTATUS mem;
    GlobalMemoryStatus(&mem);

    this->SystemMemory = mem.dwTotalPhys / 1024U;
}

// 0x0fbb2010
u32 CSystemProbe::SetDefaultActiveSettings() {
    if (this->Active.Resolution == GRAPHICSRESOLUTION_UNKNOWN) {
        this->Active.Resolution = GRAPHICSRESOLUTION_1024x768;
    }

    if (this->Active.ColorDepth == GRAPHICSCOLORDEPTH_UNKNOWN) {
        this->Active.ColorDepth = GRAPHICSCOLORDEPTH_32BIT;
    }

    if (this->Active.ShadowQuality == GRAPHICSSHADOWQUALITY_UNKNOWN) {
        this->Active.ShadowQuality = GRAPHICSSHADOWQUALITY_HITMAN;
    }

    if (this->Active.TextureQuality == GRAPHICSTEXTUREQUALITY_UNKNOWN) {
        this->Active.TextureQuality = GRAPHICSTEXTUREQUALITY_FULL;
    }

    if (this->Active.Unk0x14 == GRAPHICSGENERICSETTING_UNKNOWN) {
        this->Active.Unk0x14 = GRAPHICSGENERICSETTING_ON;
    }

    if (this->Active.AnisotropicFiltering == GRAPHICSGENERICSETTING_UNKNOWN) {
        this->Active.AnisotropicFiltering = GRAPHICSGENERICSETTING_OFF;
    }

    if (this->Active.AntiAliasing == GRAPHICSGENERICSETTING_UNKNOWN) {
        this->Active.AntiAliasing = GRAPHICSGENERICSETTING_OFF;
    }

    if (this->Active.TextureCompression == GRAPHICSGENERICSETTING_UNKNOWN) {
        this->Active.TextureCompression = GRAPHICSGENERICSETTING_ON;
    }

    if (this->Active.PaletteCompression == GRAPHICSGENERICSETTING_UNKNOWN) {
        this->Active.PaletteCompression = GRAPHICSGENERICSETTING_ON;
    }

    if (this->Active.VertexLighing == GRAPHICSGENERICSETTING_UNKNOWN) {
        this->Active.VertexLighing = GRAPHICSGENERICSETTING_OFF;
    }

    if (this->Active.FullScreen == GRAPHICSGENERICSETTING_UNKNOWN) {
        this->Active.FullScreen = GRAPHICSGENERICSETTING_ON;
    }

    return EXIT_SUCCESS;
}

// 0x0fbb20d0
u32 CSystemProbe::SetAllSettings() {
    this->SetVideoMemorySettings();
    this->SetSystemMemorySettings();
    this->SetProcessorSettings();

    return EXIT_SUCCESS;
}

// 0x0fbb20f0
u32 CSystemProbe::SetActiveSettings() {
    this->Active.API = (this->Capabilities & GRAPHICSCAPABILITY_GLIDE3X)
        ? GRAPHICSAPI_GLIDE3D : GRAPHICSAPI_DIRECTX;

    // Resolution
    {
        u32 count = 0;
        f32 value = 0.0f;

        for (u32 i = 0; i < this->SettingsCount; i++) {
            if (this->Active.Resolution != GRAPHICSRESOLUTION_UNKNOWN) {
                value += (f32)this->Active.Resolution;
                count = count + 1;
            }
        }

        this->Active.Resolution =
            (GraphicsResolution)this->AverageSettingValue(value, count);
    }

    // Color Depth
    {
        u32 count = 0;
        f32 value = 0.0f;

        for (u32 i = 0; i < this->SettingsCount; i++) {
            if (this->Active.ColorDepth != GRAPHICSCOLORDEPTH_UNKNOWN) {
                value += (f32)this->Active.ColorDepth;
                count = count + 1;
            }
        }

        this->Active.ColorDepth =
            (GraphicsColorDepth)this->AverageSettingValue(value, count);
    }

    // Texture Quality
    {
        u32 count = 0;
        f32 value = 0.0f;

        for (u32 i = 0; i < this->SettingsCount; i++) {
            if (this->Active.TextureQuality != GRAPHICSTEXTUREQUALITY_UNKNOWN) {
                value += (f32)this->Active.TextureQuality;
                count = count + 1;
            }
        }

        this->Active.TextureQuality =
            (GraphicsTextureQuality)this->AverageSettingValue(value, count);
    }

    // TODO
    {
        u32 count = 0;
        f32 value = 0.0f;

        for (u32 i = 0; i < this->SettingsCount; i++) {
            if (this->Active.Unk0x14 != GRAPHICSGENERICSETTING_UNKNOWN) {
                value += (f32)this->Active.Unk0x14;
                count = count + 1;
            }
        }

        this->Active.Unk0x14 =
            (GraphicsGenericSetting)this->AverageSettingValue(value, count);
    }

    // Shadow Quality
    {
        u32 count = 0;
        f32 value = 0.0f;

        for (u32 i = 0; i < this->SettingsCount; i++) {
            if (this->Active.ShadowQuality != GRAPHICSSHADOWQUALITY_UNKNOWN) {
                value += (f32)this->Active.ShadowQuality;
                count = count + 1;
            }
        }

        this->Active.ShadowQuality =
            (GraphicsShadowQuality)this->AverageSettingValue(value, count);
    }

    // Vertex Lighing
    {
        u32 count = 0;

        for (u32 i = 0; i < this->SettingsCount; i++) {
            if (this->Active.VertexLighing != GRAPHICSGENERICSETTING_UNKNOWN) {
                count = count + 1;
            }
        }

        if (count != 0) {
            this->Active.VertexLighing = GRAPHICSGENERICSETTING_ON;
        }
    }

    return EXIT_SUCCESS;
}

// 0x0fbb22e0
u32 CSystemProbe::AverageSettingValue(f32 value, u32 count) {
    return count == 0 ? ((u32)-1) : (u32)(value / count);
}

// 0x0fbb2300
u32 CSystemProbe::SetVideoMemorySettings() {
    if (this->Capabilities & GRAPHICSCAPABILITY_DIRECTDRAW) {
        for (u32 i = 0; i < g_DirectDrawDeviceCount; i++) {
            CGraphicsDevice* device = &this->Devices[i];

            if (device->Acceleration & GRAPHICSDEVICEACCELERATION_DIRECTDRAW3D) {
                this->SelectVideoSettings(device->Memory);
                return EXIT_SUCCESS;
            }
        }

        u32 indx = 0;
        for (u32 i = 1; i < g_DirectDrawDeviceCount; i++) {
            if (this->Devices[indx].Memory < this->Devices[i].Memory) {
                indx = i;
            }
        }

        this->SelectVideoSettings(this->Devices[indx].Memory);
    }

    return EXIT_SUCCESS;
}

// 0x0fbb23a0
u32 CSystemProbe::SelectVideoSettings(u32 size) {
    this->VideoMemory = size;

    if (size <= 8200) {
        this->Settings[this->SettingsCount].Resolution = GRAPHICSRESOLUTION_640x480;
        this->Settings[this->SettingsCount].TextureQuality = GRAPHICSTEXTUREQUALITY_LOW;
    }
    else if (size < 16000) {
        this->Settings[this->SettingsCount].Resolution = GRAPHICSRESOLUTION_800x600;
        this->Settings[this->SettingsCount].TextureQuality = GRAPHICSTEXTUREQUALITY_MEDIUM;
    }
    else if (size < 32000) {
        this->Settings[this->SettingsCount].Resolution = GRAPHICSRESOLUTION_1024x768;
        this->Settings[this->SettingsCount].TextureQuality = GRAPHICSTEXTUREQUALITY_FULL;
    }
    else {
        this->Settings[this->SettingsCount].Resolution = GRAPHICSRESOLUTION_1280x1024;
        this->Settings[this->SettingsCount].TextureQuality = GRAPHICSTEXTUREQUALITY_FULL;
    }

    this->SettingsCount = this->SettingsCount + 1;

    return EXIT_SUCCESS;
}

// 0x0fbb24a0
u32 CSystemProbe::SetSystemMemorySettings() {
    if (this->SystemMemory != 0) {
        if (this->SystemMemory < 65000) {
            this->Settings[this->SettingsCount].Resolution = GRAPHICSRESOLUTION_640x480;
            this->Settings[this->SettingsCount].ColorDepth = GRAPHICSCOLORDEPTH_16BIT;
            this->Settings[this->SettingsCount].ShadowQuality = GRAPHICSSHADOWQUALITY_NONE;
            this->Settings[this->SettingsCount].TextureQuality = GRAPHICSTEXTUREQUALITY_VERY_LOW;
            this->Settings[this->SettingsCount].Unk0x14 = GRAPHICSGENERICSETTING_OFF;
        }
        else if (this->SystemMemory < 128000) {
            this->Settings[this->SettingsCount].TextureQuality = GRAPHICSTEXTUREQUALITY_MEDIUM;
            this->Settings[this->SettingsCount].Unk0x14 = GRAPHICSGENERICSETTING_ON;
        }
        else {
            this->Settings[this->SettingsCount].TextureQuality = GRAPHICSTEXTUREQUALITY_FULL;
            this->Settings[this->SettingsCount].Unk0x14 = GRAPHICSGENERICSETTING_ON;
        }

        this->SettingsCount = this->SettingsCount + 1;
    }

    return EXIT_SUCCESS;
}

// 0x0fbb25a0
u32 CSystemProbe::SetProcessorSettings() {
    if (this->ProcessorType == PROCESSORTYPE_NONE) {
        return EXIT_SUCCESS;
    }

    switch (this->ProcessorType) {
    case PROCESSORTYPE_VERY_SLOW: {
        this->Settings[this->SettingsCount].Resolution = GRAPHICSRESOLUTION_640x480;
        this->Settings[this->SettingsCount].ColorDepth = GRAPHICSCOLORDEPTH_16BIT;
        this->Settings[this->SettingsCount].ShadowQuality = GRAPHICSSHADOWQUALITY_NONE;
        this->Settings[this->SettingsCount].TextureQuality = GRAPHICSTEXTUREQUALITY_VERY_LOW;
        this->Settings[this->SettingsCount].Unk0x14 = GRAPHICSGENERICSETTING_OFF;

        break;
    }
    case PROCESSORTYPE_SLOW: {
        this->Settings[this->SettingsCount].Resolution = GRAPHICSRESOLUTION_800x600;
        this->Settings[this->SettingsCount].ShadowQuality = GRAPHICSSHADOWQUALITY_NONE;
        this->Settings[this->SettingsCount].Unk0x14 = GRAPHICSGENERICSETTING_OFF;

        break;
    }
    case PROCESSORTYPE_MEDIUM: {
        this->Settings[this->SettingsCount].Resolution = GRAPHICSRESOLUTION_1024x768;
        this->Settings[this->SettingsCount].ShadowQuality = GRAPHICSSHADOWQUALITY_HITMAN;
        this->Settings[this->SettingsCount].Unk0x14 = GRAPHICSGENERICSETTING_ON;

        break;
    }
    case PROCESSORTYPE_FAST: {
        this->Settings[this->SettingsCount].Resolution = GRAPHICSRESOLUTION_1024x768;
        this->Settings[this->SettingsCount].ShadowQuality = GRAPHICSSHADOWQUALITY_HITMAN;
        this->Settings[this->SettingsCount].Unk0x14 = GRAPHICSGENERICSETTING_ON;

        break;
    }
    case PROCESSORTYPE_VERY_FAST: {
        this->Settings[this->SettingsCount].Resolution = GRAPHICSRESOLUTION_1024x768;
        this->Settings[this->SettingsCount].ShadowQuality = GRAPHICSSHADOWQUALITY_ALL;
        this->Settings[this->SettingsCount].Unk0x14 = GRAPHICSGENERICSETTING_ON;

        break;
    }
    }

    this->SettingsCount = this->SettingsCount + 1;

    return EXIT_SUCCESS;
}

// 0x0fbb2730
u32 CSystemProbe::InitializePresets() {
    if (this->Presets == nullptr) {
        this->Presets = new CGraphicsPreset();
    }

    CGraphicsPreset* card = this->Presets;
    strcpy(card->Name, "Diamond FIRE GL 1000");

    card = card->GetNext();
    strcpy(card->Name, "Diamond Stealth III S540 for Windows 98");
    card->Settings.API = GRAPHICSAPI_OPENGL;
    card->Settings.Resolution = GRAPHICSRESOLUTION_640x480;
    card->Settings.FullScreen = GRAPHICSGENERICSETTING_OFF;

    card = card->GetNext();
    strcpy(card->Name, "Matrox Millennium G200 AGP - English");
    card->Settings.API = GRAPHICSAPI_OPENGL;
    card->Settings.Resolution = GRAPHICSRESOLUTION_640x480;
    card->Settings.FullScreen = GRAPHICSGENERICSETTING_ON;

    card = card->GetNext();
    strcpy(card->Name, "Matrox Millennium G400 DualHead - English");
    card->Settings.API = GRAPHICSAPI_GLIDE3D;
    card->Settings.Resolution = GRAPHICSRESOLUTION_800x600;
    card->Settings.FullScreen = GRAPHICSGENERICSETTING_ON;

    card = card->GetNext();
    strcpy(card->Name, "NVIDIA GeForce2 GTS/GeForce2 Pro");
    card->Settings.API = GRAPHICSAPI_GLIDE3D;
    card->Settings.Resolution = GRAPHICSRESOLUTION_800x600;
    card->Settings.FullScreen = GRAPHICSGENERICSETTING_ON;

    card = card->GetNext();
    strcpy(card->Name, "NVIDIA GeForce2 MX");
    card->Settings.API = GRAPHICSAPI_GLIDE3D;
    card->Settings.Resolution = GRAPHICSRESOLUTION_800x600;
    card->Settings.FullScreen = GRAPHICSGENERICSETTING_ON;

    card = card->GetNext();
    strcpy(card->Name, "NVIDIA GeForce 256");
    card->Settings.API = GRAPHICSAPI_GLIDE3D;
    card->Settings.FullScreen = GRAPHICSGENERICSETTING_ON;

    card = card->GetNext();
    strcpy(card->Name, "NVIDIA RIVA TNT");
    card->Settings.API = GRAPHICSAPI_GLIDE3D;
    card->Settings.FullScreen = GRAPHICSGENERICSETTING_ON;

    card = card->GetNext();
    strcpy(card->Name, "Voodoo3 AGP");
    card->Settings.Resolution = GRAPHICSRESOLUTION_640x480;
    card->Settings.FullScreen = GRAPHICSGENERICSETTING_OFF;

    card = card->GetNext();
    strcpy(card->Name, "Voodoo5 5500 AGP");
    card->Settings.Resolution = GRAPHICSRESOLUTION_800x600;
    card->Settings.FullScreen = GRAPHICSGENERICSETTING_OFF;

    card = card->GetNext();
    strcpy(card->Name, "All-In-Wonder 128 Pro AGP (English)");
    card->Settings.API = GRAPHICSAPI_GLIDE3D;
    card->Settings.Resolution = GRAPHICSRESOLUTION_800x600;
    card->Settings.FullScreen = GRAPHICSGENERICSETTING_ON;

    card = card->GetNext();
    strcpy(card->Name, "Radeon DDR");
    card->Settings.API = GRAPHICSAPI_GLIDE3D;
    card->Settings.Resolution = GRAPHICSRESOLUTION_800x600;
    card->Settings.FullScreen = GRAPHICSGENERICSETTING_ON;

    return EXIT_SUCCESS;
}

// 0x0fbb2ab0
u32 CSystemProbe::SetMemorySufficiency() {
    this->SystemMemory =
        this->SystemMemory < SUFFICIENT_MEMORY_SIZE ? FALSE : TRUE;

    return EXIT_SUCCESS;
}

// 0x0fbb2ae0
u32 CSystemProbe::SetUnk0x20C() {
    this->Unk0x20C = 8; // TODO

    return EXIT_SUCCESS;
}
