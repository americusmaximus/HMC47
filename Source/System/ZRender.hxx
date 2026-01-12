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

#include "LinkSortRefTab.hxx"
#include "ZConsole.hxx"
#include "ZCounter.hxx"

#define ZRENDERCAPS_NONE                0x0
#define ZRENDERCAPS_ANISOTROPY          0x1
#define ZRENDERCAPS_ANTIALIAS           0x2
#define ZRENDERCAPS_DXT                 0x8
#define ZRENDERCAPS_TRILINEARFILTERING  0x10

#define ZRENDERCLEAR_TARGET             0x1
#define ZRENDERCLEAR_ZBUFFER            0x2

#define ZRENDERFEATURE_ANISOTROPY           1
#define ZRENDERFEATURE_ANTIALIAS            2
#define ZRENDERFEATURE_TEXTURECOMPRESSION   4
#define ZRENDERFEATURE_TRILINEARFILTERING   5

class ZRenderConsoleCommand;

#pragma pack(push, 1)

struct ZRenderConfiguration {
    u32 Width;
    u32 Height;
    u32 ColorDepth;
    BOOL FullScreen;
};

class ZRenderBase {
public:
    ZRenderBase(HINSTANCE instance, HWND window);

public:
    virtual ~ZRenderBase();                                                         // 0x0
    virtual void SetModule(const char* path);                                       // 0x4
    virtual void Method0x8(u32 todo);                                               // 0x8
    virtual void SetConfiguration(ZRenderConfiguration* config);                    // 0xC
    virtual const ZRenderConfiguration* GetSupportedConfigurations();               // 0x10
    virtual void GetCaps(u32* caps);                                                // 0x14
    virtual void SetFeature(u32 feature, u32 value);                                // 0x18
    virtual void Method0x1C(u32 todo1, u32 todo2);                                  // 0x1C
    virtual void Clear(u32 color) = 0;                                              // 0x20
    virtual void Method0x24() = 0;                                                  // 0x24
    virtual void EnableDepthBuffer() = 0;                                           // 0x28
    virtual void Method0x2C(u32 todo) = 0;                                          // 0x2C
    virtual void Method0x30(u32 todo1, bool todo2) = 0;                             // 0x30
    virtual void Method0x34(u32 todo1, u32 todo2) = 0;                              // 0x34
    virtual void Method0x38() = 0;                                                  // 0x38
    virtual bool Method0x3C();                                                      // 0x3C
    virtual void Method0x40();                                                      // 0x40
    virtual void Method0x44();                                                      // 0x44
    virtual void Method0x48();                                                      // 0x48
    virtual void Method0x4C(u32 todo1, u32 todo2, u32 todo3);                       // 0x4C
    virtual void Method0x50(u32 todo1, u32 todo2, u32 todo3);                       // 0x50
    virtual void Method0x54();                                                      // 0x54
    virtual void ClipCursorToWindow(bool value);                                    // 0x58
    virtual bool Method0x5C();                                                      // 0x5C
    virtual void Method0x60(u32 todo);                                              // 0x60
    virtual void Method0x64(u32 todo);                                              // 0x64
    virtual void Method0x68();                                                      // 0x68
    virtual LinkSortRefTab* Method0x6C();                                           // 0x6C
    virtual void Method0x70(LinkSortRefTab* todo);                                  // 0x70
    virtual void Method0x74();                                                      // 0x74
    virtual void Method0x78();                                                      // 0x78
    virtual void* Method0x7C(u32 todo);                                             // 0x7C
    virtual void Method0x80();                                                      // 0x80
    virtual void Method0x84();                                                      // 0x84
    virtual bool Method0x88();                                                      // 0x88
    virtual void Method0x8C();                                                      // 0x8C
    virtual bool BeginScene();                                                      // 0x90
    virtual bool EndScene();                                                        // 0x94
    virtual void Method0x98();                                                      // 0x98
    virtual f32 Method0x9C();                                                       // 0x9C
    virtual s32 GetWindowWidth();                                                   // 0xA0
    virtual s32 GetWindowHeight();                                                  // 0xA4
    virtual void SetWindowSize(s32 width, s32 height);                              // 0xA8
    virtual s32 Method0xAC();                                                       // 0xAC
    virtual s32 Method0xB0();                                                       // 0xB0
    virtual f32 Method0xB4();                                                       // 0xB4
    virtual f32 Method0xB8();                                                       // 0xB8
    virtual void SetFontSize(f32 value);                                            // 0xBC
    virtual f32 Method0xC0();                                                       // 0xC0
    virtual bool Method0xC4();                                                      // 0xC4
    virtual void Method0xC8(u32 todo1, u32 todo2, u32 todo3);                       // 0xC8
    virtual ZCounter* GetCounter();                                                 // 0xCC
    virtual void Method0xD0();                                                      // 0xD0
    virtual void Method0xD4();                                                      // 0xD4
    virtual ZRenderBase* GetRender();                                               // 0xD8
    virtual void Method0xDC(f64* value);                                            // 0xDC
    virtual void Method0xE0(f32* value);                                            // 0xE0
    virtual void Method0xE4() = 0;                                                  // 0xE4
    virtual void __cdecl Method0xE8(s32 todo1, s32 todo2, const char* format, ...); // 0xE8
    virtual void Method0xEC(const char* value);                                     // 0xEC
    virtual void SetShowCursorVisibility(u32 visibility);                           // 0xF0
    virtual u32 GetShowCursorVisibility();                                          // 0xF4
    virtual void Method0xF8(u32 todo1, u32 todo2, u32 todo3);                       // 0xF8
    virtual void Method0xFC();                                                      // 0xFC
    virtual void SetViewport(f32 x, f32 y, f32 width, f32 height);                  // 0x100
    virtual void ClearSurface(u32 options, u32 color);                              // 0x104
    virtual void ApplyCurrentState();                                               // 0x108
    virtual void InitializeCurrentState();                                          // 0x10C
    virtual void Method0x110();                                                     // 0x110
    virtual void Method0x114();                                                     // 0x114
    virtual void Method0x118();                                                     // 0x118
    virtual void Method0x11C(u32 todo1, u32 todo2, u32 todo3);                      // 0x11C
    virtual void Method0x120(u32 todo);                                             // 0x120
    virtual void Method0x124(u32 todo);                                             // 0x124
    virtual bool SetFog(u32 value);                                                 // 0x128
    virtual void Method0x12C(u32 todo1, u32 todo2);                                 // 0x12C
    virtual void Method0x130(f64* value);                                           // 0x130
    virtual void Method0x134(u32);                                                  // 0x134
    virtual void Method0x138();                                                     // 0x138
    virtual void Method0x13C(u32);                                                  // 0x13C
    virtual void Method0x140(u32 todo1, u32 todo2, u32 todo3);                      // 0x140
    virtual u32 GetMaximumTextures();                                               // 0x144
    const char* GetRenderName();                                                    // 0x148
    virtual void SetMaximumTextures(u32 count);                                     // 0x14C

public:
    u32 Unk0x4;                                                                     // 0x4
    ZRenderBase* Current;                                                           // 0x8
    ZRenderBase* Previous;                                                          // 0xC
    HINSTANCE Instance;                                                             // 0x10
    HWND Window;                                                                    // 0x14
    HWND ParentWindow;                                                              // 0x18
    f32 NativeDrawVisibility;                                                       // 0x1C
    u32 Unk0x20;                                                                    // 0x20
    u32 Unk0x24;                                                                    // 0x24
    u32 MaximumTextures;                                                            // 0x28
    bool Initialized;                                                               // 0x2C
    u32 Unk0x2D;                                                                    // 0x2D
    u32 Unk0x31;                                                                    // 0x31
    u32 Unk0x35;                                                                    // 0x35
    u32 Unk0x39;                                                                    // 0x39
    bool DisableTripleBuffering;                                                    // 0x3D
    f32 Unk0x3E;                                                                    // 0x3E
    f32 Unk0x42;                                                                    // 0x42
    s32 MouseX;                                                                     // 0x46
    s32 MouseY;                                                                     // 0x4A
    s32 PreviousMouseX;                                                             // 0x4E
    s32 PreviousMouseY;                                                             // 0x52
    bool Unk0x56;                                                                   // 0x56
    f32 ShowCursorVisibility;                                                       // 0x57
    bool Unk0x5B;                                                                   // 0x5B
    void* Unk0x5C;                                                                  // 0x5C
    u32 Unk0x60;                                                                    // 0x60
    u32 Unk0x64;                                                                    // 0x64
    u32 Unk0x68;                                                                    // 0x68
    u32 Unk0x6C;                                                                    // 0x6C

    // TODO

    u32 Unk0x78;                                                                    // 0x78
    u32 DisplayFrequency;                                                           // 0x7C
    bool Unk0x80;                                                                   // 0x80
    bool Unk0x81;                                                                   // 0x81
    bool Unk0x82;                                                                   // 0x82
    s32 W;                                                                          // 0x83
    s32 H;                                                                          // 0x87
    LinkSortRefTab* Unk0x8B;                                                        // 0x8B
    u32 Unk0x8F;                                                                    // 0x8F
    u32 Unk0x93;                                                                    // 0x93

    u32 Unk0x9F;                                                                    // 0x9F
    ZRenderBase* Render;                                                            // 0xA3
    s32 LockCount;                                                                  // 0xA7

    // TODO
    ZCounter* Counter;                                                              // 0xDB
    ZUnk0xC* Unk0xDF;                                                               // 0xDF
    RefTab* Unk0xE3;                                                                // 0xE3
    RefTab* Unk0xE7;                                                                // 0xE7
    RefTab* Unk0xEB;                                                                // 0xEB
    RefTab* Unk0xEF;                                                                // 0xEF
    f32 Unk0xF3;                                                                    // 0xF3
    ZVisualConsoleCommand* DebugCommand;                                            // 0xF7
    ZVisualConsoleCommand* TimersCommand;                                           // 0xFB
    ZVisualConsoleCommand* TimeMultiplierCommand;                                   // 0xFF
    ZVisualConsoleCommand* FogCommand;                                              // 0x103
    ZVisualConsoleCommand* NativeDrawCommand;                                       // 0x107
    ZVisualConsoleCommand* ShowCursorCommand;                                       // 0x10B
    ZVisualConsoleCommand* GammaCommand;                                            // 0x10F
    ZVisualConsoleCommand* TextureStatCommand;                                      // 0x113
    ZVisualConsoleCommand* ShowDrawModesCommand;                                    // 0x117
    ZRenderConsoleCommand* FontSizeCommand;                                         // 0x11B
    ZRenderConsoleCommand* AntialiasCommand;                                        // 0x11F
    f32 Unk0x123;                                                                   // 0x123
    f32 Unk0x127;                                                                   // 0x127
};

class ZRender : public ZRenderBase {
public:
    ZRender(HINSTANCE instance, HWND window);

public:
    virtual ~ZRender();                                                                     // 0x0
    virtual void SetModule(const char* path);                                               // 0x4
    virtual void SetConfiguration(ZRenderConfiguration* config);                            // 0xC
    virtual const ZRenderConfiguration* GetSupportedConfigurations();                       // 0x10
    virtual void Method0x30(u32 todo1, bool todo2);                                         // 0x30
    virtual void Method0x34(u32 todo1, u32 todo2);                                          // 0x34
    virtual void Method0x38();                                                              // 0x38
    virtual void ClipCursorToWindow(bool value);                                            // 0x58
    virtual bool Method0x5C();                                                              // 0x5C
    virtual void Method0xF8(u32 todo1, u32 todo2, u32 todo3);                               // 0xF8
    virtual void Method0x150(const char* path);                                             // 0x150
    virtual void Method0x154(u32 todo);                                                     // 0x154
    virtual void Method0x158();                                                             // 0x158
    virtual bool CreateRenderWindow(const char* cls,
        const char* name, DWORD style, DWORD estyle, LPRECT size, HWND window);             // 0x15C
    virtual void ShowRenderWindow(int cmd);                                                 // 0x160
    virtual LRESULT HandleRenderWindowMessages(UINT uMsg, WPARAM wparam, LPARAM lParam);    // 0x164
    virtual void InitializeRenderWindow(LPARAM lParam);                                     // 0x168
    virtual void ReleaseRenderWindow();                                                     // 0x16C
    virtual void HandleKeyDown(WPARAM wParam, LPARAM lParam);                               // 0x170
    virtual void HandleKeyUp(WPARAM wParam, LPARAM lParam);                                 // 0x174
    virtual void HandleLeftButtonDown(WPARAM wParam, s32 x, s32 y);                         // 0x178
    virtual void HandleLeftButtonUp(WPARAM wParam, s32 x, s32 y);                           // 0x17C
    virtual void HandleLeftButtonDoubleClick(WPARAM wParam, s32 x, s32 y);                  // 0x180
    virtual void HandleRightButtonDown(WPARAM wParam, s32 x, s32 y);                        // 0x184
    virtual void HandleRightButtonUp(WPARAM wParam, s32 x, s32 y);                          // 0x188
    virtual void HandleMiddleButtonDown(WPARAM wParam, s32 x, s32 y);                       // 0x18C
    virtual void HandleMiddleButtonUp(WPARAM wParam, s32 x, s32 y);                         // 0x190
    virtual void HandleMouseMove(WPARAM wParam, s32 x, s32 y);                              // 0x194
    virtual void HandleMouseWheel(s16 value);                                               // 0x198
    virtual bool HandleSetCursor();                                                         // 0x19C
    virtual void HandleSetFocus();                                                          // 0x1A0
    virtual void HandleKillFocus();                                                         // 0x1A4
    virtual void HandleUpdate() = 0;                                                        // 0x1A8
    virtual void HandleSize(s32 width, s32 height) = 0;                                     // 0x1AC
    virtual void Method0x1B0();                                                             // 0x1B0
    virtual void Method0x1B4() = 0;                                                         // 0x1B4
    virtual void Method0x1B8() = 0;                                                         // 0x1B8
    virtual void CreateRenderPalette(HDC hdc);                                              // 0x1BC
    virtual void GetRenderWindowDimensions(LPRECT rect);                                    // 0x1C0
    virtual void Initialize();                                                              // 0x1C4

protected:
    void DisplayRenderWindow(const char* title);

protected:
    bool Unk0x12B;                                                                          // 0x12B
    bool Unk0x12C;                                                                          // 0x12C
    u32 Unk0x12D;                                                                           // 0x12D
    HPALETTE Palette;                                                                       // 0x131
    HPALETTE ActivePalette;                                                                 // 0x135
    u32 Unk0x139;                                                                           // 0x139

    // TODO

    RefTab* Unk0x14D[512 /* TODO */];                                                       // 0x14D
    ZVisualConsoleCommand* FogOverrideCommand;                                              // 0x94D
    ZVisualConsoleCommand* FogNearCommand;                                                  // 0x951
    ZVisualConsoleCommand* FogFarCommand;                                                   // 0x955
};

class ZRenderConsoleCommand : public ZConsoleCommand {
public:
    ZRenderConsoleCommand(const char* command, ZRenderBase* instance);

public:
    virtual ~ZRenderConsoleCommand();
    virtual void Execute(const char* value);                                                // 0x8

protected:
    ZRenderBase* Instance;                                                                  // 0x8
};

#pragma pack(pop)

#if defined(_DEBUG) && !defined(_WIN64)
static_assert(sizeof(ZRender)                       == 0x959,   "ZRender size mismatch.");
static_assert(sizeof(ZRenderBase)                   == 0x12B,   "ZRenderBase size mismatch.");
static_assert(sizeof(ZRenderConsoleCommand)         == 0xC,     "ZRenderConsoleCommand size mismatch.");
#endif
