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

#include "ZConsole.hxx"
#include "ZDirectPlayModule.hxx"
#include "ZEngineData.hxx"
#include "ZMasterControl.hxx"
#include "ZRender.hxx"
#include "ZRenderModule.hxx"
#include "ZSettings.hxx"
#include "ZSoundModule.hxx"
#include "ZEngineDataModule.hxx"

typedef void (*NOTIFYDESTROYCALL)();

#pragma pack(push, 1)

struct Unk0xF00 {
    bool Unk0x0;                                                                // 0x0
    bool Unk0x1;                                                                // 0x1
    bool Unk0x2;                                                                // 0x2
    bool Unk0x3[256 /* TODO */];                                                // 0x3

    u8 Unk0xTODO[3581]; // TODO

};

#if defined(_DEBUG) && !defined(_WIN64)

#endif

struct Unk0x1E00 {
    bool Unk0x0;                                                                // 0x0
    bool Unk0x1;                                                                // 0x1
    bool Unk0x2;                                                                // 0x2
    bool Unk0x3[512 /* TODO */];                                                // 0x3

    u8 Unk0xTODO[7165]; // TODO
};

// 0x0ffd3494
class ZSysInterfaceBase {
public:
    ZSysInterfaceBase();

public:
    virtual bool Method0x0();                                                   // 0x0
    virtual ~ZSysInterfaceBase();                                               // 0x4
    virtual void Initialize() = 0;                                              // 0x8
    virtual bool HandleWindowMessages(HWND) = 0;                                // 0xC
    virtual void Method0x10() = 0;                                              // 0x10
    virtual void Method0x14(WPARAM, void*, u32) = 0;                            // 0x14
    virtual bool Method0x18(const char*, u32) = 0;                              // 0x18
    virtual void RestoreDisplaySettings() = 0;                                  // 0x1C
    virtual void Method0x20(bool) = 0;                                          // 0x20
    virtual void Method0x24() = 0;                                              // 0x24
    virtual void Method0x28() = 0;                                              // 0x28
    virtual void Method0x2C() = 0;                                              // 0x2C
    virtual void Method0x30() = 0;                                              // 0x30
    virtual void Method0x34() = 0;                                              // 0x34
    virtual void Method0x38() = 0;                                              // 0x38
    virtual void Method0x3C() = 0;                                              // 0x3C
    virtual void Method0x40() = 0;                                              // 0x40
    virtual void Method0x44() = 0;                                              // 0x44
    virtual bool IsKeyPressed(s32) = 0;                                         // 0x48
    virtual void Method0x4C() = 0;                                              // 0x4C
    virtual void Method0x50() = 0;                                              // 0x50
    virtual void Method0x54() = 0;                                              // 0x54
    virtual void Method0x58() = 0;                                              // 0x58
    virtual void SetWindowTitle(const char*) = 0;                               // 0x5C
    virtual void Method0x60() = 0;                                              // 0x60
    virtual void Method0x64() = 0;                                              // 0x64
    virtual void Method0x68() = 0;                                              // 0x68
    virtual void Method0x6C() = 0;                                              // 0x6C
    virtual void Method0x70() = 0;                                              // 0x70
    virtual void Method0x74() = 0;                                              // 0x74
    virtual void Method0x78() = 0;                                              // 0x78
    virtual void Method0x7C() = 0;                                              // 0x7C
    virtual void Method0x80() = 0;                                              // 0x80
    virtual void ExecuteWithArgs(const char*) = 0;                              // 0x84
    virtual bool Execute(u32) = 0;                                              // 0x88
    virtual void Method0x8C() = 0;                                              // 0x8C
    virtual void Method0x90() = 0;                                              // 0x90
    virtual void Method0x94() = 0;                                              // 0x94
    virtual void Method0x98() = 0;                                              // 0x98
    virtual void Method0x9C() = 0;                                              // 0x9C
    virtual void Method0xA0() = 0;                                              // 0xA0
    virtual void Sleep(f32) = 0;                                                // 0xA4
    virtual void Method0xA8() = 0;                                              // 0xA8
    virtual void Method0xAC() = 0;                                              // 0xAC
    virtual void SetRenderNameCounterValue(const char*, u64) = 0;               // 0xB0
    virtual void Method0xB4() = 0;                                              // 0xB4
    virtual void SetRenderCounterValue(u64) = 0;                                // 0xB8
    virtual ZModule* LoadModule(const char*) = 0;                               // 0xBC
    virtual bool ReleaseModule(ZModule*) = 0;                                   // 0xC0
    virtual void Method0xC4() = 0;                                              // 0xC4
    virtual void __cdecl Method0xC8(u32, u32, const char*, ...) = 0;            // 0xC8
    virtual u32 Method0xCC(u32, u32) = 0;                                       // 0xCC
    virtual s32 __cdecl FormatString(char*, const char* format, ...);           // 0xD0
    virtual void __cdecl Method0xD4() = 0;                                      // 0xD4
    virtual void Method0xD8() = 0;                                              // 0xD8
    virtual void __cdecl Method0xDC(const char* format, ...) = 0;               // 0xDC
    virtual void RegisterConsoleCommand(ZConsoleCommand* command);              // 0xE0
    virtual void UnregisterConsoleCommand(ZConsoleCommand* command);            // 0xE4
    virtual void RunConsoleCommand(const char* command);                        // 0xE8
    virtual ZConsole* GetConsole();                                             // 0xEC
    virtual void Method0xF0(u64 value, const char* path, u32 line);             // 0xF0
    virtual void Method0xF4();                                                  // 0xF4
    virtual void Method0xF8();                                                  // 0xF8
    virtual void Method0xFC();                                                  // 0xFC
    virtual void Method0x100();                                                 // 0x100
    virtual void Method0x104();                                                 // 0x104
    virtual void Method0x108();                                                 // 0x108
    virtual void Method0x10C(u32, u32) = 0;                                     // 0x10C
    virtual void Method0x110();                                                 // 0x110
    virtual void Method0x114();                                                 // 0x114
    virtual void Method0x118();                                                 // 0x118
    virtual void Method0x11C();                                                 // 0x11C
    virtual void Method0x120();                                                 // 0x120
    virtual void Method0x124();                                                 // 0x124
    virtual u64 GetProcessorTicks(const char*, u32) = 0;                        // 0x128

public:
    bool Unk0x4;                                                                // 0x4
    bool Unk0x5;                                                                // 0x5
    bool Unk0x6;                                                                // 0x6
    bool Unk0x7;                                                                // 0x7
    bool EnablePack;                                                            // 0x8
    bool StereoView;                                                            // 0x9
    bool Antialias;                                                             // 0xA
    bool DisableDXT;                                                            // 0xB
    bool DisablePAL;                                                            // 0xC
    bool EnableZBuffer;                                                         // 0xD
    bool DisableMultiTexture;                                                   // 0xE
    bool Unk0xF;                                                                // 0xF
    bool EnableLight;                                                           // 0x10
    bool TrilinearFiltering;                                                    // 0x11
    bool FullScreen;                                                            // 0x12
    bool PreviousFullScreen;                                                    // 0x13
    bool DisableHWTnL;                                                          // 0x14
    bool EnableEMBM;                                                            // 0x15
    bool EnableTripleBuffering;                                                 // 0x16
    bool DebugVideo;                                                            // 0x17
    bool Unk0x18;                                                               // 0x18
    s32 WindowWidth;                                                            // 0x19
    s32 WindowHeight;                                                           // 0x1D
    s32 PreviousWindowWidth;                                                    // 0x21
    s32 PreviousWindowHeight;                                                   // 0x25
    s32 ColorDepth;                                                             // 0x29
    s32 PreviousColorDepth;                                                     // 0x2D
    u32 RefreshRate;                                                            // 0x31
    u32 Unk0x35;                                                                // 0x35
    s32 WindowX;                                                                // 0x39
    s32 WindowY;                                                                // 0x3D
    u32 Unk0x41;                                                                // 0x41
    GraphicsAnisotropy Anisotropy;                                              // 0x45
    GraphicsShadowQuality ShadowDetail;                                         // 0x49
    bool Blood;                                                                 // 0x4D
    bool PathfinderInfo;                                                        // 0x4E
    bool EnableScriptRTC;                                                       // 0x4F
    bool DisableOptions;                                                        // 0x50
    GraphicsTextureQuality TextureResolution;                                   // 0x51
    GraphicsLevelOfDetail LevelOfDetail;                                        // 0x55
    ZEngineData* EngineData;                                                    // 0x59
    u32 Unk0x5D;                                                                // 0x5D
    ZString ModulePath;                                                         // 0x61
    ZString BasePath;                                                           // 0xE1
    ZString Unk0x161;                                                           // 0x161
    ZString Unk0x1E1;                                                           // 0x1E1
    ZString Unk0x261;                                                           // 0x261
    ZString ProjectFile;                                                        // 0x2E1
    ZString Unk0x361;                                                           // 0x361
    ZString DrawDll;                                                            // 0x3E1
    ZString ScriptDll;                                                          // 0x461
    ZString DirectPlayDll;                                                      // 0x4E1
    ZString SoundDll;                                                           // 0x561
    ZString LocaleDll;                                                          // 0x5E1
    ZString AnimFile;                                                           // 0x661
    ZString Unk0x6E1;                                                           // 0x6E1
    ZString Unk0x761;                                                           // 0x761
    ZString Unk0x7E1;                                                           // 0x7E1
    ZString Unk0x861;                                                           // 0x861
    ZString RecordFile;                                                         // 0x8E1
    ZString PlayFile;                                                           // 0x961
    ZString PlayVideo;                                                          // 0x9E1
    f64 ProcessorCounter;                                                       // 0xA61
    u32 Unk0xA69;                                                               // 0xA69
    u32 FreeVideoMemory;                                                        // 0xA6D
    f32 TimersVisibility;                                                       // 0xA71
    f32 DebugOptionsVisibility;                                                 // 0xA75
    u32 Unk0xA79;                                                               // 0xA79
    HMODULE Module;                                                             // 0xA7D
    u32 Unk0xA81;                                                               // 0xA81
    HWND Unk0xA85;                                                              // 0xA85
    HWND MainWindow;                                                            // 0xA89
    u32 Unk0xA8D;                                                               // 0xA8D
    u32 Unk0xA91;                                                               // 0xA91
    s32 Unk0xA95;                                                               // 0xA95
    s32 Unk0xA99;                                                               // 0xA99
    f32 MouseWheel;                                                             // 0xA9D
    u32 Unk0xAA1;                                                               // 0xAA1
    u32 Unk0xAA5;                                                               // 0xAA5
    u32 Unk0xAA9;                                                               // 0xAA9
    f32 Unk0xAAD;                                                               // 0xAAD
    Unk0xF00 Unk0xAB1;                                                          // 0xAB1
    Unk0x1E00 Unk0x19B1;                                                        // 0x19B1
    void* Unk0x37B1;                                                            // 0x37B1
    u32 Unk0x37B5;                                                              // 0x37B5
    u32 Unk0x37B9;                                                              // 0x37B9
    u32 Unk0x37BD;                                                              // 0x37BD
    u32 Unk0x37C1;                                                              // 0x37C1
    f32 Unk0x37C5;                                                              // 0x37C5
    u32 Unk0x37C9;                                                              // 0x37C9
    u32 Unk0x37CD;                                                              // 0x37CD
    u32 Unk0x37D1;                                                              // 0x37D1
    u32 Unk0x37D5;                                                              // 0x37D5
    u32 Unk0x37D9;                                                              // 0x37D9
    u32 Unk0x37DD;                                                              // 0x37DD
    f32 Unk0x37E1;                                                              // 0x37E1
    ZRender* Render;                                                            // 0x37E5
    ZMasterControl* MasterControl;                                              // 0x37E9
    ZString MasterProject;                                                      // 0x37ED
    ZString CommandLine;                                                        // 0x386D
    bool Unk0x38ED;                                                             // 0x38ED
    bool EnableTextures;                                                        // 0x38EE
    bool Unk0x38EF;                                                             // 0x38EF
    bool IsEngineRunning;                                                       // 0x38F0
    bool Unk0x38F1;                                                             // 0x38F1
    bool DisableConfig;                                                         // 0x38F2
    bool Unk0x38F3;                                                             // 0x38F3
    bool Unk0x38F4;                                                             // 0x38F4
    ZSoundModule* SoundModule;                                                  // 0x38F5
    ZRenderModule* RenderModule;                                                // 0x38F9
    ZModule* ScriptModule;                                                      // 0x38FD
    ZDirectPlayModule* DirectPlayModule;                                        // 0x3901
    ZModule* LocaleModule;                                                      // 0x3905
    ZString Unk0x3909;                                                          // 0x3909
    bool ScriptDebug;                                                           // 0x3989
    bool ScriptDebugPrint;                                                      // 0x398A
    bool Unk0x398B;                                                             // 0x398B
    bool Unk0x398C;                                                             // 0x398C
    f32 TimeMultiplierVisibility;                                               // 0x398D
    BOOL WindowHasFocus;    // TODO NAME AND TYPE                               // 0x3991
    bool Unk0x3995;                                                             // 0x3995
    char Unk0x3996[128 /* TODO */];                                             // 0x3996
    void* Unk0x3A16;                                                            // 0x3A16
    void* Unk0x3A1A;                                                            // 0x3A1A
    u32 Unk0x3A1E;                                                              // 0x3A1E
    u32 Unk0x3A22;                                                              // 0x3A22
    bool SaveRecordRuntime;                                                     // 0x3A26
    u32 Unk0x3A27;                                                              // 0x3A27
    u32 Unk0x3A2B;                                                              // 0x3A2B
    u32 Unk0x3A2F;                                                              // 0x3A2F
    void* Unk0x3A33;                                                            // 0x3A33
    u32 Unk0x3A37;                                                              // 0x3A37
    ZConsole* Console;                                                          // 0x3A3B
    bool Unk0x3A3F;                                                             // 0x3A3F
    ZEngineDataModule* EngineModule;                                            // 0x3A40
    bool ProcessingWindowMessages;                                              // 0x3A44
};

// 0x0ffd35d8
class ZSysInterface : public ZSysInterfaceBase {
public:
    ZSysInterface(HMODULE module);

public:
    typedef void (ZSysInterface::* LPFNRUNACTION)(u32 code); // TODO

public:
    virtual ~ZSysInterface();                                                           // 0x4
    virtual void Initialize();                                                          // 0x8
    virtual bool HandleWindowMessages(HWND hwnd);                                       // 0xC
    virtual void Method0x10();                                                          // 0x10
    virtual void Method0x14(WPARAM wParam, void* value, u32 size);                      // 0x14
    virtual bool Method0x18(const char*, u32 /* TODO */);                               // 0x18
    virtual void RestoreDisplaySettings();                                              // 0x1C
    virtual void Method0x20(bool);                                                      // 0x20
    virtual void ReleaseRender();                                                       // 0x24
    virtual void ReleaseScriptModule();                                                 // 0x28
    virtual void Method0x2C();                                                          // 0x2C
    virtual void Method0x30();                                                          // 0x30
    virtual void Method0x34();                                                          // 0x34
    virtual void Method0x38();                                                          // 0x38
    virtual void Method0x3C();                                                          // 0x3C
    virtual void Method0x40();                                                          // 0x40
    virtual void Method0x44();                                                          // 0x44
    virtual bool IsKeyPressed(s32 key);                                                 // 0x48
    virtual void Method0x4C();                                                          // 0x4C
    virtual void Method0x50();                                                          // 0x50
    virtual void Method0x54();                                                          // 0x54
    virtual void Method0x58();                                                          // 0x58
    virtual void SetWindowTitle(const char* title);                                     // 0x5C
    virtual void SetEngineData(ZEngineData* value);                                     // 0x60
    virtual void Method0x64();                                                          // 0x64
    virtual void Method0x68();                                                          // 0x68
    virtual void Method0x6C();                                                          // 0x6C
    virtual void Method0x70();                                                          // 0x70
    virtual void Method0x74();                                                          // 0x74
    virtual void Method0x78();                                                          // 0x78
    virtual void Method0x7C();                                                          // 0x7C
    virtual void Method0x80();                                                          // 0x80
    virtual void ExecuteWithArgs(const char* ini);                                      // 0x84
    virtual bool Execute(u32 code /* TODO */);                                          // 0x88
    virtual void Method0x8C();                                                          // 0x8C
    virtual void Method0x90();                                                          // 0x90
    virtual void Method0x94();                                                          // 0x94
    virtual void Method0x98();                                                          // 0x98
    virtual void Method0x9C();                                                          // 0x9C
    virtual void Method0xA0();                                                          // 0xA0
    virtual void Sleep(f32 time);                                                       // 0xA4
    virtual void Method0xA8();                                                          // 0xA8
    virtual void Method0xAC();                                                          // 0xAC
    virtual void SetRenderNameCounterValue(const char* name, u64 value);                // 0xB0
    virtual void Method0xB4();                                                          // 0xB4
    virtual void SetRenderCounterValue(u64 value);                                      // 0xB8
    virtual ZModule* LoadModule(const char* path);                                      // 0xBC
    virtual bool ReleaseModule(ZModule* module);                                        // 0xC0
    virtual void Method0xC4();                                                          // 0xC4
    virtual void Method0xC8(u32, u32);                                                  // 0xC8
    virtual u32 Method0xCC(u32, u32);                                                   // 0xCC
    virtual void __cdecl Method0xD4();                                                  // 0xD4
    virtual void __cdecl DisplayDebugString(s32 x, s32 y, const char* format, ...);     // 0xD8
    virtual void __cdecl Method0xDC(const char* format, ...);                           // 0xDC
    virtual void Method0x10C(const char* path, u32 line);                               // 0x10C
    virtual u64 GetProcessorTicks(const char* path, u32 line);                          // 0x128
    virtual void NotifyDestroy(ZModule* module);                                        // 0x12C

public:
    void InitializeModule(ZModule** result, const char* name);
    void ExecuteEngineWrapper(u32 code);   // TODO

protected:
    bool CreateMainWindow();
    void SetCommandLine(const char* ini);
    void ExecuteEngine(u32 code);   // TODO

public:
    ZString LogPath;                                                            // 0x3A45
    bool UseTryCatchMainLoop;                                                   // 0x3AC5
    u32 ExceptionCount;                                                         // 0x3AC6
    ZString WindowTitle;                                                        // 0x3ACA
    ZString Unk0x3B4A;                                                          // 0x3B4A
    bool Continue;                                                              // 0x3BCA
    u64 Unk0x3BCD;                                                              // 0x3BCD
    u32 Unk0x3BD3;                                                              // 0x3BD3
    u32 Unk0x3BD7;                                                              // 0x3BD7
    LPFNRUNACTION RunAction;                                                    // 0x3BDB
};

#pragma pack(pop)

#if defined(_DEBUG) && !defined(_WIN64)
static_assert(sizeof(Unk0xF00)          == 0xF00,   "Unk0xF00 size mismatch."); // TODO
static_assert(sizeof(Unk0x1E00)         == 0x1E00,  "Unk0x1E00 size mismatch."); // TODO
static_assert(sizeof(ZSysInterface)     == 0x3BDF,  "ZSysInterface size mismatch.");
static_assert(sizeof(ZSysInterfaceBase) == 0x3A45,  "ZSysInterfaceBase size mismatch.");
#endif
