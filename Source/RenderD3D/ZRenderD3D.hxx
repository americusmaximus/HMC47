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

#include "ZVertexBufferManager.hxx"

#include <System/ZRender.hxx>

// TODO
#define ZRENDEROPTION_FULL_SCREEN           0x1
#define ZRENDEROPTION_STEREO                0x2
#define ZRENDEROPTION_DEPTH_BUFFER          0x4
#define ZRENDEROPTION_PRESERVE_FPU          0x8

#define ZERR_82000001                       0x82000001
#define ZERR_82000002                       0x82000002
#define ZERR_82000003                       0x82000003
#define ZERR_82000004                       0x82000004
#define ZERR_82000005                       0x82000005
#define ZERR_82000007                       0x82000007
#define ZERR_82000008                       0x82000008
#define ZERR_82000009                       0x82000009
#define ZERR_8200000A                       0x8200000A
#define ZERR_8200000B                       0x8200000B
#define ZERR_8200000C                       0x8200000C
#define ZERR_8200000E                       0x8200000E
#define ZERR_8200000F                       0x8200000F

#pragma pack(push, 1)

#define MAX_RENDER_TEXTURE_STAGE_COUNT  8
#define MAX_RENDER_TEXTURE_STATE_COUNT  11

struct ZRenderState {
    LPDIRECTDRAWSURFACE7 Texture[MAX_RENDER_TEXTURE_STAGE_COUNT];                   // 0x0
    D3DTEXTUREOP ColorOperation[MAX_RENDER_TEXTURE_STAGE_COUNT];                    // 0x20
    DWORD ColorArg1[MAX_RENDER_TEXTURE_STAGE_COUNT];                                // 0x40
    DWORD ColorArg2[MAX_RENDER_TEXTURE_STAGE_COUNT];                                // 0x60
    D3DTEXTUREOP AlphaOperation[MAX_RENDER_TEXTURE_STAGE_COUNT];                    // 0x80
    DWORD AlphaArg1[MAX_RENDER_TEXTURE_STAGE_COUNT];                                // 0xA0
    DWORD AlphaArg2[MAX_RENDER_TEXTURE_STAGE_COUNT];                                // 0xC0
    D3DBLEND SrcBlend;                                                              // 0xE0
    D3DBLEND DestBlend;                                                             // 0xE4
    u32 FogEnable;                                                                  // 0xE8
    BOOL AlphaBlendEnable;                                                          // 0xEC
    BOOL ZWriteEnable;                                                              // 0xF0
    u32 Unk0xF4;                                                                    // 0xF4
    u32 Unk0xF8;                                                                    // 0xF8
    D3DCOLOR FogColor;                                                              // 0xFC
    u32 Unk0x100;                                                                   // 0x100
    u32 Unk0x104;                                                                   // 0x104
    u32 Unk0x108;                                                                   // 0x108
    DWORD FogStart;                                                                 // 0x10C
    DWORD FogEnd;                                                                   // 0x110
    D3DCMPFUNC ZFunc;                                                               // 0x114
    D3DCMPFUNC AlphaFunc;                                                           // 0x118
};

class ZRenderD3D : public ZRender {
public:
    ZRenderD3D(HINSTANCE instance, HWND window, void* todo);

public:
    virtual ~ZRenderD3D();                                                          // 0x0
    virtual void GetCaps(u32* caps);                                                // 0x14
    virtual void SetFeature(u32 feature, u32 value);                                // 0x18
    virtual void Clear(u32 color);                                                  // 0x20
    virtual void EnableDepthBuffer();                                               // 0x28
    virtual void Method0x2C(u32 todo);                                              // 0x2C
    virtual void Method0x4C(u32 todo1, u32 todo2, u32 todo3);                       // 0x4C
    virtual void Method0x54();                                                      // 0x54
    virtual void Method0x64(u32 todo);                                              // 0x64
    virtual bool BeginScene();                                                      // 0x90
    virtual bool EndScene();                                                        // 0x94
    virtual void Method0xD4();                                                      // 0xD4
    virtual void Method0xE4();                                                      // 0xE4
    virtual void SetViewport(f32 x, f32 y, f32 width, f32 height);                  // 0x100
    virtual void ClearSurface(u32 options, u32 color);                              // 0x104
    virtual void ApplyCurrentState();                                               // 0x108
    virtual void InitializeCurrentState();                                          // 0x10C

    // TODO

    const char* GetRenderName();                                                    // 0x148

    // TODO

    virtual void Initialize();                                                      // 0x1B8

    // TODO


    virtual void Method0x1C8(HWND window);                                          // 0x1C8

public:
    // TODO

    LPDIRECTDRAW7 DirectDraw;                                                       // 0x959
    LPDIRECTDRAWSURFACE7 MainSurface;                                               // 0x95D
    LPDIRECTDRAWSURFACE7 BackSurface;                                               // 0x961
    LPDIRECTDRAWSURFACE7 StereoSurface;                                             // 0x965
    LPDIRECTDRAWSURFACE7 DepthSurface;                                              // 0x969
    LPDIRECT3D7 D3D;                                                                // 0x96D
    LPDIRECT3DDEVICE7 Device;                                                       // 0x971
    DDCAPS Caps;                                                                    // 0x975
    D3DDEVICEDESC7 DeviceCaps;                                                      // 0xAF1
    RECT WindowRectangle;                                                           // 0xBDD
    s32 X;                                                                          // 0xBED
    s32 Y;                                                                          // 0xBF1
    s32 Width;                                                                      // 0xBF5
    s32 Height;                                                                     // 0xBF9
    BOOL FullScreen;                                                                // 0xBFD
    BOOL StereoView;                                                                // 0xC01
    s32 WindowWidth;                                                                // 0xC05
    s32 WindowHeight;                                                               // 0xC09
    u32 Capabilities;                                                               // 0xC0D
    void* Unk0xC11;                                                                 // 0xC11
    void* Unk0xC15;                                                                 // 0xC15
    ZVertexBufferManager* VBM;                                                      // 0xC19
    void* Unk0xC1D;                                                                 // 0xC1D

protected:
    void InitializeRendererValues();
    HRESULT CreateWindowSurfaces();
    HRESULT InitializeDirectDraw(LPGUID dd, LPCGUID d3d, LPDDSURFACEDESC2 settings, u32 options);
    HRESULT InitializeD3D(LPGUID dd, LPCGUID d3d, LPDDSURFACEDESC2 settings, u32 options);
    HRESULT CreateDirectDraw(LPGUID device, u32 options);
    HRESULT CreateFullScreenSurfaces(LPDDSURFACEDESC2 settings);
    HRESULT CreateD3D(LPCGUID device);
    HRESULT CreateDepthBuffer(LPCGUID device);
    HRESULT ReleaseDirectDraw();
    void Release();
    HRESULT RestoreAllSurfaces();
    HRESULT Blt(bool blt);
};

#pragma pack(pop)

#if defined(_DEBUG) && !defined(_WIN64)
static_assert(sizeof(ZRenderTextureStage)   == 0x11C,   "ZRenderTextureStage size mismatch.");
static_assert(sizeof(ZRenderD3D)            == 0xC21,   "ZRenderD3D size mismatch.");
#endif
