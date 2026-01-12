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

#include "ZExceptionRenderD3D.hxx"
#include "ZRenderD3D.hxx"
#include "ZRenderModuleD3D.hxx"

#define MAX_RENDER_DEVICE_COUNT         20

typedef HRESULT(CALLBACK* LPFNACCEPTDEVICECALLBACK)(LPDDCAPS, LPD3DDEVICEDESC7);

HRESULT CALLBACK EnumerateDepthBufferFormatsCallback(LPDDPIXELFORMAT, LPVOID);
HRESULT WINAPI EnumerateDirectDrawDisplayModes(LPDDSURFACEDESC2, LPVOID);
HRESULT CALLBACK EnumerateDirect3DDevices(LPSTR, LPSTR, LPD3DDEVICEDESC7, LPVOID);

s32 EnumerateDirectDrawDevices(LPFNACCEPTDEVICECALLBACK);
s32 __cdecl CompareDirectDrawDisplayMode(void const*, void const*);

static D3DTEXTUREMAGFILTER g_TextureStageMagFilter = D3DTFG_LINEAR;             // 0x0fbb80a4
static D3DTEXTUREMINFILTER g_TextureStageMinFilter = D3DTFN_LINEAR;             // 0x0fbb80a8
static D3DTEXTUREMIPFILTER g_TextureStageMipFilter = D3DTFP_POINT;              // 0x0fbb80ac

static BOOL g_DestBlendSrcAlpha = TRUE;                                         // 0x0fbb84c4

static ZRenderState g_RenderStates[MAX_RENDER_TEXTURE_STATE_COUNT];             // 0x0fbbe1ac
static ZRenderState g_CurrentRenderState;                                       // 0x0fbbede0
static LPDIRECT3DDEVICE7 g_Device;                                              // 0x0fbbeefc
static u32 g_MaxSimTextures;                                                    // 0x0fbbef00

static LPDIRECTDRAWSURFACE7 DAT_0fbbef0c;                                       // 0x0fbbef0c

static D3DZBUFFERTYPE g_DepthBufferType;                                        // 0x0fbbef10

static ZRenderDevice g_Devices[MAX_RENDER_DEVICE_COUNT];                        // 0x0fbbef20
static LPFNACCEPTDEVICECALLBACK g_DeviceCallback;                               // 0x0fbc4f60
static u32 g_DeviceCount;                                                       // 0x0fbc4f64
static u32 g_AcceptedDeviceCount;                                               // 0x0fbc4f68

// 0x0fb81300
void LogDirectDrawError(HRESULT hr) {
    const char* err = "<Unknown>";

    switch (hr) {
    case DDERR_SURFACELOST: { err = "DDERR_SURFACELOST"; break; }
    case DDERR_NOFLIPHW: { err = "DDERR_NOFLIPHW"; break; }
    case DDERR_NOOVERLAYHW: { err = "DDERR_NOOVERLAYHW"; break; }
    case DDERR_NOZBUFFERHW: { err = "DDERR_NOZBUFFERHW"; break; }
    case DDERR_OUTOFVIDEOMEMORY: { err = "DDERR_OUTOFVIDEOMEMORY"; break; }
    case DDERR_SURFACEALREADYATTACHED: { err = "DDERR_SURFACEALREADYATTACHED"; break; }
    case DDERR_SURFACEBUSY: { err = "DDERR_SURFACEBUSY"; break; }
    case DDERR_WASSTILLDRAWING: { err = "DDERR_WASSTILLDRAWING"; break; }
    case DDERR_NODIRECTDRAWHW: { err = "DDERR_NODIRECTDRAWHW"; break; }
    case DDERR_PRIMARYSURFACEALREADYEXISTS: { err = "DDERR_PRIMARYSURFACEALREADYEXISTS"; break; }
    case DDERR_NOEMULATION: { err = "DDERR_NOEMULATION"; break; }
    case DDERR_UNSUPPORTEDMODE: { err = "DDERR_UNSUPPORTEDMODE"; break; }
    case DDERR_NOMIPMAPHW: { err = "DDERR_NOMIPMAPHW"; break; }
    case DDERR_NOEXCLUSIVEMODE: { err = "DDERR_NOEXCLUSIVEMODE"; break; }
    case DDERR_INVALIDCAPS: { err = "DDERR_INVALIDCAPS"; break; }
    case DDERR_INVALIDPARAMS: { err = "DDERR_INVALIDPARAMS"; break; }
    case DDERR_UNSUPPORTED: { err = "DDERR_UNSUPPORTED"; break; }
    case DDERR_GENERIC: { err = "DDERR_GENERIC"; break; }
    case DDERR_OUTOFMEMORY: { err = "DDERR_OUTOFMEMORY"; break; }
    case DDERR_CANNOTATTACHSURFACE: { err = "DDERR_CANNOTATTACHSURFACE"; break; }
    case DDERR_INCOMPATIBLEPRIMARY: { err = "DDERR_INCOMPATIBLEPRIMARY"; break; }
    case DDERR_INVALIDMODE: { err = "DDERR_INVALIDMODE"; break; }
    case DDERR_INVALIDOBJECT: { err = "DDERR_INVALIDOBJECT"; break; }
    case DDERR_INVALIDPIXELFORMAT: { err = "DDERR_INVALIDPIXELFORMAT"; break; }
    case DDERR_LOCKEDSURFACES: { err = "DDERR_LOCKEDSURFACES"; break; }
    case DDERR_NOALPHAHW: { err = "DDERR_NOALPHAHW"; break; }
    case DDERR_NOCOOPERATIVELEVELSET: { err = "DDERR_NOCOOPERATIVELEVELSET"; break; }
    }

    g_pSysCom->Log("Z:\\Engine\\Drawing\\_Wintel\\RenderD3D\\Source\\RenderWintelD3DDraw.cpp", 157)
        ->LogMessage("DD Error: (%08x) %s", hr, err);

    g_pSysCom->Log("Z:\\Engine\\Drawing\\_Wintel\\RenderD3D\\Source\\RenderWintelD3DDraw.cpp", 158)
        ->LogMessage("INT3 in %s at line %d",
            "Z:\\Engine\\Drawing\\_Wintel\\RenderD3D\\Source\\RenderWintelD3DDraw.cpp", 158);

    __asm { int 3 }
}

// 0x0fb816c0
void LogDirectDrawSceneError(HRESULT hr) {
    const char* err = "Unknown error";

    switch (hr) {
    case D3D_OK: { err = "D3D_OK"; break; }
    case D3DERR_UNSUPPORTEDCOLOROPERATION: { err = "D3DERR_UNSUPPORTEDCOLOROPERATION"; break; }
    case D3DERR_INVALIDVERTEXTYPE: { err = "D3DERR_INVALIDVERTEXTYPE"; break; }
    case DDERR_WASSTILLDRAWING: { err = "DDERR_WASSTILLDRAWING"; break; }
    case DDERR_INVALIDPARAMS: { err = "DDERR_INVALIDPARAMS"; break; }
    case DDERR_INVALIDOBJECT: { err = "DDERR_INVALIDOBJECT"; break; }
    case D3DERR_INVALIDPRIMITIVETYPE: { err = "D3DERR_INVALIDPRIMITIVETYPE"; break; }
    case D3DERR_INVALIDRAMPTEXTURE: { err = "D3DERR_INVALIDRAMPTEXTURE"; break; }
    case D3DERR_VERTEXBUFFERLOCKED: { err = "D3DERR_VERTEXBUFFERLOCKED"; break; }
    case D3DERR_WRONGTEXTUREFORMAT: { err = "D3DERR_WRONGTEXTUREFORMAT"; break; }
    case D3DERR_CONFLICTINGTEXTUREPALETTE: { err = "D3DERR_CONFLICTINGTEXTUREPALETTE"; break; }
    case D3DERR_UNSUPPORTEDCOLORARG: { err = "D3DERR_UNSUPPORTEDCOLORARG"; break; }
    case D3DERR_UNSUPPORTEDALPHAOPERATION: { err = "D3DERR_UNSUPPORTEDALPHAOPERATION"; break; }
    case D3DERR_UNSUPPORTEDALPHAARG: { err = "D3DERR_UNSUPPORTEDALPHAARG"; break; }
    case D3DERR_TOOMANYOPERATIONS: { err = "D3DERR_TOOMANYOPERATIONS"; break; }
    case D3DERR_CONFLICTINGTEXTUREFILTER: { err = "D3DERR_CONFLICTINGTEXTUREFILTER"; break; }
    case D3DERR_UNSUPPORTEDFACTORVALUE: { err = "D3DERR_UNSUPPORTEDFACTORVALUE"; break; }
    }

    g_pSysCom->Log("Z:\\Engine\\Drawing\\_Wintel\\RenderD3D\\Source\\RenderWintelD3DDraw.cpp", 226)
        ->LogMessage("D3D Error: (%08x) %s", hr, err);

    g_pSysCom->Log("Z:\\Engine\\Drawing\\_Wintel\\RenderD3D\\Source\\RenderWintelD3DDraw.cpp", 227)
        ->LogMessage("INT3 in %s at line %d",
            "Z:\\Engine\\Drawing\\_Wintel\\RenderD3D\\Source\\RenderWintelD3DDraw.cpp", 227);

    __asm {int 3}
}

// 0x0fb81920
ZRenderD3D::ZRenderD3D(HINSTANCE instance, HWND window, ZTextureManagerD3D* manager)
    : ZRender(instance, window) {
    this->TextureManager = manager;

    this->DirectDraw = nullptr;
    this->MainSurface = nullptr;
    this->BackSurface = nullptr;
    this->StereoSurface = nullptr;
    this->DepthSurface = nullptr;
    this->D3D = nullptr;
    this->Device = nullptr;

    this->FullScreen = FALSE;
    this->StereoView = FALSE;

    this->WindowWidth = 0;
    this->WindowHeight = 0;
    this->Capabilities = 0;

    this->Unk0xC11 = 0;
    this->Unk0xC15 = 0;

    this->VertexBufferManager = nullptr;

    ZeroMemory(&ZRenderUnk0x18Array, 8 * sizeof(ZRenderUnk0x18)); // TODO
}

// 0x0fb81a20
const char* ZRenderD3D::GetRenderName() {
    return "D3D7";
}

// 0x0fb81a30
// 0x0fb81a50
ZRenderD3D::~ZRenderD3D() {
    if (this->Initialized) {
        this->Release();
    }
}

// 0x0fb81ab0
void ZRenderD3D::Initialize() {
    if (g_pSysInterface->DebugVideo) {
        g_pSysCom->Log("Z:\\Engine\\Drawing\\_Wintel\\RenderD3D\\Source\\RenderWintelD3DDraw.cpp", 318)
            ->LogMessage("Initializing D3D environment");
    }

    this->InitializeRendererValues();
    EnumerateDirectDrawDevices(nullptr);

    if (g_AcceptedDeviceCount == 0) {
        throw ZExceptionRenderD3D("Direct3D", "No suitable hardware found");
    }

    ZRenderDevice* tnl = nullptr;
    ZRenderDevice* hal = nullptr;
    ZRenderDevice* rgb = nullptr;
    ZRenderDevice* reference = nullptr;

    for (u32 i = 0; i < g_AcceptedDeviceCount; i++) {
        ZRenderDevice* current = &g_Devices[i];

        if (g_pSysInterface->DebugVideo) {
            g_pSysCom->Log("Z:\\Engine\\Drawing\\_Wintel\\RenderD3D\\Source\\RenderWintelD3DDraw.cpp", 335)
                ->LogMessage("[%d] %s", i, current->Name);
        }

        if (memcmp(&current->IDD3D, &IID_IDirect3DTnLHalDevice, sizeof(GUID)) == 0
            && (!g_pSysInterface->StereoView || current->Unk0x498) && tnl == nullptr) {
            tnl = current;
        }

        if (memcmp(&current->IDD3D, &IID_IDirect3DHALDevice, sizeof(GUID)) == 0
            && (!g_pSysInterface->StereoView || current->Unk0x498) && hal == nullptr) {
            hal = current;
        }

        if (memcmp(&current->IDD3D, &IID_IDirect3DRGBDevice, sizeof(GUID)) == 0
            && (!g_pSysInterface->StereoView || current->Unk0x498) && rgb == nullptr) {
            rgb = current;
        }

        if (_strcmpi(current->Name, "Reference Rasterizer") == 0 && reference == nullptr) {
            reference = current;
        }
    }

    ZRenderDevice* selected = nullptr;

    if (tnl == nullptr) {
        selected = hal;

        if (hal == nullptr) {
            throw ZExceptionRenderD3D("Direct3D", "No suitable hardware found");
        }
    }
    else {
        selected = tnl;

        if (g_pSysInterface->DisableHWTnL) {
            g_pSysCom->Log("Z:\\Engine\\Drawing\\_Wintel\\RenderD3D\\Source\\RenderWintelD3DDraw.cpp", 360)
                ->LogMessage("Disabling HW Transform and Lighting (TnL)");

            selected = hal;

            if (hal == nullptr) {
                throw ZExceptionRenderD3D("Direct3D", "No suitable hardware found");
            }
        }
    }

    if (selected == nullptr) {
        throw ZExceptionRenderD3D("Direct3D", "No suitable hardware found");
    }

    LPDDSURFACEDESC2 settings = nullptr;
    LPDDSURFACEDESC2 settings16 = nullptr;
    LPDDSURFACEDESC2 settings32 = nullptr;

    for (u32 i = 0; i < selected->ModeCount; i++) {
        LPDDSURFACEDESC2 mode = &selected->Modes[i];

        if (!g_pSysInterface->FullScreen
            || mode->dwWidth == g_pSysInterface->WindowWidth
            || mode->dwHeight == g_pSysInterface->WindowHeight) {
            if (g_pSysInterface->DebugVideo) {
                g_pSysCom->Log("Z:\\Engine\\Drawing\\_Wintel\\RenderD3D\\Source\\RenderWintelD3DDraw.cpp", 381)
                    ->LogMessage("Mode %4dx%4d %2d - %2dHz", mode->dwWidth, mode->dwHeight,
                        mode->ddpfPixelFormat.dwRGBBitCount, mode->dwRefreshRate);

                settings32 = settings;
            }

            if (mode->ddpfPixelFormat.dwRGBBitCount == 32) {
                if (settings32 == nullptr || !g_pSysInterface->FullScreen
                    || g_pSysInterface->RefreshRate == 0 || g_pSysInterface->RefreshRate <= mode->dwRefreshRate) {
                    settings = mode;
                    settings32 = mode;
                }
            }
            else if (mode->ddpfPixelFormat.dwRGBBitCount == 16) {
                if (settings16 == nullptr || !g_pSysInterface->FullScreen
                    || g_pSysInterface->RefreshRate == 0 || g_pSysInterface->RefreshRate <= mode->dwRefreshRate) {
                    settings16 = mode;
                }
            }
        }
    }

    if (g_pSysInterface->ColorDepth == 32) {
        if (settings32 == nullptr) {
            throw ZExceptionRenderD3D("Direct3D",
                "Unable to find a suitable display mode for true color. Try changing to 16bit colors.");
        }
    }
    else if (g_pSysInterface->ColorDepth == 16) {
        settings32 = settings16;

        if (settings16 == nullptr) {
            throw ZExceptionRenderD3D("Direct3D",
                "Unable to find a suitable display mode for high color. Try changing to 32bit colors.");
        }
    }
    else if (settings32 == nullptr) {
        settings32 = settings16;

        if (settings16 == nullptr) {
            throw ZExceptionRenderD3D("Direct3D", "Unable to find a suitable display mode");
        }
    }

    if (g_pSysInterface->DebugVideo) {
        if (g_pSysInterface->FullScreen) {
            g_pSysCom->Log("Z:\\Engine\\Drawing\\_Wintel\\RenderD3D\\Source\\RenderWintelD3DDraw.cpp", 415)
                ->LogMessage("%s mode %dx%dx%d - %dHz", settings32->dwWidth, settings32->dwHeight,
                    settings32->ddpfPixelFormat.dwRGBBitCount, settings32->dwRefreshRate);
        }
        else {
            g_pSysCom->Log("Z:\\Engine\\Drawing\\_Wintel\\RenderD3D\\Source\\RenderWintelD3DDraw.cpp", 417)
                ->LogMessage("%s", selected->Name);
        }
    }

    if (this->InitializeD3D(selected->IDDD, selected->IDD3D, settings32,
        g_pSysInterface->FullScreen ? (ZRENDEROPTION_DEPTH_BUFFER | ZRENDEROPTION_FULL_SCREEN) : ZRENDEROPTION_DEPTH_BUFFER) != DD_OK) {
        throw ZExceptionRenderD3D("Direct3D", "Unable to initialize Direct3D");
    }

    g_Device = this->Device;

    if (g_pSysInterface->DebugVideo) {
        DDDEVICEIDENTIFIER2 ident;
        this->DirectDraw->GetDeviceIdentifier(&ident, 0);

        g_pSysCom->Log("Z:\\Engine\\Drawing\\_Wintel\\RenderD3D\\Source\\RenderWintelD3DDraw.cpp", 436)
            ->LogMessage("%s - %s", ident.szDescription, ident.szDriver);
    }

    this->TextureManager->Method0x3C();

    if (!this->TextureManager->SupportsCompression()) {
        g_pSysInterface->DisableDXT = true;
    }

    if (!this->TextureManager->SupportsEMBM()) {
        g_pSysInterface->EnableEMBM = false;
    }
    else if (g_pSysInterface->DebugVideo) {
        g_pSysCom->Log("Z:\\Engine\\Drawing\\_Wintel\\RenderD3D\\Source\\RenderWintelD3DDraw.cpp", 447)
            ->LogMessage("Environment bumpmapping supported");

        if (g_pSysInterface->EnableEMBM) {
            g_pSysCom->Log("Z:\\Engine\\Drawing\\_Wintel\\RenderD3D\\Source\\RenderWintelD3DDraw.cpp", 449)
                ->LogMessage("Environment bumpmapping enabled");
        }
    }

    this->X = this->WindowRectangle.left;
    this->Y = this->WindowRectangle.top;
    this->Width = this->WindowRectangle.right;
    this->Height = this->WindowRectangle.bottom;

    this->Device->GetCaps(&this->DeviceCaps);
    g_MaxSimTextures = this->DeviceCaps.wMaxSimultaneousTextures;

    u32 simTex = 0;
    for (; simTex < g_MaxSimTextures; simTex++) {
        if (g_CurrentRenderState.ColorOperation[simTex] != D3DTOP_MODULATE) {
            g_CurrentRenderState.ColorOperation[simTex] = D3DTOP_MODULATE;
            g_Device->SetTextureStageState(simTex, D3DTSS_COLOROP, D3DTOP_MODULATE);
        }

        if (g_CurrentRenderState.ColorArg1[simTex] != D3DTOP_SELECTARG1) {
            g_CurrentRenderState.ColorArg1[simTex] = D3DTOP_SELECTARG1;
            g_Device->SetTextureStageState(simTex, D3DTSS_COLORARG1, D3DTOP_SELECTARG1);
        }

        if (g_CurrentRenderState.ColorArg2[simTex] != D3DTOP_DISABLE) {
            g_CurrentRenderState.ColorArg2[simTex] = D3DTOP_DISABLE;
            g_Device->SetTextureStageState(simTex, D3DTSS_COLORARG2, D3DTOP_DISABLE);
        }

        g_Device->SetTextureStageState(simTex, D3DTSS_TEXCOORDINDEX, simTex);

        DWORD valid = DD_OK;
        if (FAILED(g_Device->ValidateDevice(&valid)) || valid != DD_OK) {
            break;
        }
    }

    for (u32 i = 0; i < g_MaxSimTextures; i++) {
        if (g_CurrentRenderState.ColorOperation[i] != D3DTOP_DISABLE) {
            g_CurrentRenderState.ColorOperation[i] = D3DTOP_DISABLE;
            g_Device->SetTextureStageState(i, D3DTSS_COLOROP, D3DTOP_DISABLE);
        }

        if (g_CurrentRenderState.ColorArg1[i] != D3DTOP_SELECTARG1) {
            g_CurrentRenderState.ColorArg1[i] = D3DTOP_SELECTARG1;
            g_Device->SetTextureStageState(i, D3DTSS_COLORARG1, D3DTOP_SELECTARG1);
        }

        if (g_CurrentRenderState.ColorArg2[i] != D3DTOP_DISABLE) {
            g_CurrentRenderState.ColorArg2[i] = D3DTOP_DISABLE;
            g_Device->SetTextureStageState(i, D3DTSS_COLORARG2, D3DTOP_DISABLE);
        }

        g_Device->SetTextureStageState(i, D3DTSS_TEXCOORDINDEX, i);
    }

    g_DestBlendSrcAlpha = this->DeviceCaps.dpcTriCaps.dwDestBlendCaps & D3DPBLENDCAPS_SRCALPHA;

    if (g_pSysInterface->DebugVideo) {
        g_pSysCom->Log("Z:\\Engine\\Drawing\\_Wintel\\RenderD3D\\Source\\RenderWintelD3DDraw.cpp", 477)
            ->LogMessage("Number of texturing units: %d", g_MaxSimTextures);

        g_pSysCom->Log("Z:\\Engine\\Drawing\\_Wintel\\RenderD3D\\Source\\RenderWintelD3DDraw.cpp", 478)
            ->LogMessage("Number of simultaneous stages: %d", simTex);


        g_pSysCom->Log("Z:\\Engine\\Drawing\\_Wintel\\RenderD3D\\Source\\RenderWintelD3DDraw.cpp", 479)
            ->LogMessage("BlendDst SrcAlpha: %s", g_DestBlendSrcAlpha ? "Yes" : "No");
    }

    this->Caps.dwSize = sizeof(DDCAPS);
    this->DirectDraw->GetCaps(&this->Caps, nullptr);

    if (g_pSysInterface->DebugVideo) {
        g_pSysCom->Log("Z:\\Engine\\Drawing\\_Wintel\\RenderD3D\\Source\\RenderWintelD3DDraw.cpp", 486)
            ->LogMessage("Render depth: %d", g_pSysInterface->ColorDepth);

        g_pSysCom->Log("Z:\\Engine\\Drawing\\_Wintel\\RenderD3D\\Source\\RenderWintelD3DDraw.cpp", 487)
            ->LogMessage("Memory Total %dMb - free %dMb",
                this->Caps.dwVidMemTotal / 1048576, this->Caps.dwVidMemFree / 1048576);
    }

    void* piVar7 = nullptr; // TODO

    if (!g_pSysInterface->DisableMultiTexture) {
        if (g_MaxSimTextures == 1) {
            piVar7 = this->Unk0xC15;
            this->Unk0xC11 = nullptr;
        }
        else {
            piVar7 = this->Unk0xC11;
            this->Unk0xC15 = nullptr;
        }
    }
    else {
        g_pSysCom->Log("Z:\\Engine\\Drawing\\_Wintel\\RenderD3D\\Source\\RenderWintelD3DDraw.cpp", 492)
            ->LogMessage("MultiTexturing is disabled!");

        g_MaxSimTextures = 1;
        piVar7 = this->Unk0xC15;
        this->Unk0xC11 = nullptr;
    }

    this->VertexBufferManager = nullptr;

    if (tnl == nullptr) {
        // TODO
        uStack_4 = 1;
        this->VertexBufferManager
            = new ZVertexBufferManager(selected->IDD3D, this->Device, 1000, piVar7, g_Module->Unk0xC, 0);
    }
    else {
        // TODO
        uStack_4 = 0;
        if (rgb != nullptr) {
            this->VertexBufferManager
                = new ZVertexBufferManager(selected->IDD3D, this->Device, 1000, piVar7, g_Module->Unk0xC, 0);
        }
    }

    g_pSysInterface->FreeVideoMemory = this->Caps.dwVidMemFree;

    this->Device->SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_NONE);
    this->Device->SetRenderState(D3DRENDERSTATE_LIGHTING, FALSE);
    this->Device->SetRenderState(D3DRENDERSTATE_SPECULARENABLE, FALSE);
    this->Device->SetRenderState(D3DRENDERSTATE_AMBIENT, 0xFFFFFFFF);

    if (!(this->DeviceCaps.dpcTriCaps.dwRasterCaps & D3DPRASTERCAPS_WBUFFER)
        || g_pSysInterface->EnableZBuffer) {
        if (g_pSysInterface->DebugVideo) {
            g_pSysCom->Log("Z:\\Engine\\Drawing\\_Wintel\\RenderD3D\\Source\\RenderWintelD3DDraw.cpp", 531)
                ->LogMessage("Using Z-Buffering");
        }

        g_DepthBufferType = D3DZB_TRUE;
    }
    else {
        if (g_pSysInterface->DebugVideo) {
            g_pSysCom->Log("Z:\\Engine\\Drawing\\_Wintel\\RenderD3D\\Source\\RenderWintelD3DDraw.cpp", 526)
                ->LogMessage("Using W-Buffering");
        }

        g_DepthBufferType = D3DZB_USEW;
    }

    this->Device->SetRenderState(D3DRENDERSTATE_ZENABLE, g_DepthBufferType);

    for (u32 i = 0; i < g_MaxSimTextures; i++) {
        this->Device->SetTextureStageState(i, D3DTSS_MINFILTER, g_TextureStageMinFilter);
        this->Device->SetTextureStageState(i, D3DTSS_MAGFILTER, g_TextureStageMagFilter);
        this->Device->SetTextureStageState(i, D3DTSS_MIPFILTER, g_TextureStageMipFilter);
        this->Device->SetTextureStageState(i, D3DTSS_TEXCOORDINDEX, i);

    }

    if (g_pSysInterface->Antialias) {
        this->SetFeature(ZRENDERFEATURE_ANTIALIAS, TRUE);
    }

    if (g_pSysInterface->Anisotropy != ANISOTROPY_0) {
        this->SetFeature(ZRENDERFEATURE_ANISOTROPY, g_pSysInterface->Anisotropy);
    }

    if (g_pSysInterface->TrilinearFiltering) {
        this->SetFeature(ZRENDERFEATURE_TRILINEARFILTERING, TRUE);
    }

    if (g_pSysInterface->ColorDepth == 16
        && (this->DeviceCaps.dpcTriCaps.dwRasterCaps & D3DPRASTERCAPS_DITHER)) {
        if (g_pSysInterface->DebugVideo) {
            g_pSysCom->Log("Z:\\Engine\\Drawing\\_Wintel\\RenderD3D\\Source\\RenderWintelD3DDraw.cpp", 558)
                ->LogMessage("Enabling dither in 16bit mode");
        }

        this->Device->SetRenderState(D3DRENDERSTATE_DITHERENABLE, TRUE);
    }

    puVar9 = (uint*)operator_new(0x10000);


    this->Clear(0x00000000);
    this->EnableDepthBuffer();

    this->Clear(0x00000000);
    this->EnableDepthBuffer();

    this->Initialized = true;

    if (g_pSysInterface->DebugVideo) {
        g_pSysCom->Log("Z:\\Engine\\Drawing\\_Wintel\\RenderD3D\\Source\\RenderWintelD3DDraw.cpp", 607)
            ->LogMessage("Initialized D3D environment.");
    }

    FUN_0fb86850();

    this->SetMaximumTextures(g_MaxSimTextures);
}

// 0x0fb827c0
void ZRenderD3D::Release() {
    // TODO NOT IMPLEMENTED
}

// 0x0fb82860
void ZRenderD3D::GetCaps(u32* caps) {
    u32 result = ZRENDERCAPS_NONE;

    if (this->DeviceCaps.dpcTriCaps.dwRasterCaps & D3DPRASTERCAPS_ANTIALIASSORTINDEPENDENT) {
        if (g_pSysInterface->DebugVideo) {
            g_pSysCom->Log("Z:\\Engine\\Drawing\\_Wintel\\RenderD3D\\Source\\RenderWintelD3DDraw.cpp", 649)
                ->LogMessage("Supports antialias");
        }

        result = ZRENDERCAPS_ANTIALIAS;
    }

    if (this->DeviceCaps.dpcTriCaps.dwRasterCaps & D3DPRASTERCAPS_ANISOTROPY) {
        if (g_pSysInterface->DebugVideo) {
            g_pSysCom->Log("Z:\\Engine\\Drawing\\_Wintel\\RenderD3D\\Source\\RenderWintelD3DDraw.cpp", 654)
                ->LogMessage("Supports anisotropy");
        }

        result |= ZRENDERCAPS_ANISOTROPY;
    }

    if (this->TextureManager->SupportsCompression()) {
        if (g_pSysInterface->DebugVideo) {
            g_pSysCom->Log("Z:\\Engine\\Drawing\\_Wintel\\RenderD3D\\Source\\RenderWintelD3DDraw.cpp", 659)
                ->LogMessage("Supports dxt");
        }

        result |= ZRENDERCAPS_DXT;
    }

    if (this->DeviceCaps.dpcTriCaps.dwTextureFilterCaps & D3DPTFILTERCAPS_LINEARMIPLINEAR) {
        if (g_pSysInterface->DebugVideo) {
            g_pSysCom->Log("Z:\\Engine\\Drawing\\_Wintel\\RenderD3D\\Source\\RenderWintelD3DDraw.cpp", 664)
                ->LogMessage("Supports trilinear filtering");
        }

        result |= ZRENDERCAPS_TRILINEARFILTERING;
    }

    *caps = result;
}

// 0x0fb82990
void ZRenderD3D::SetFeature(u32 feature, u32 value) {
    switch (feature) {
    case ZRENDERFEATURE_ANISOTROPY: {
        if (value) {
            for (u32 i = 0; i < g_MaxSimTextures; i++) {
                this->Device->SetTextureStageState(i, D3DTSS_MINFILTER, D3DTFN_ANISOTROPIC);
                this->Device->SetTextureStageState(i, D3DTSS_MAGFILTER, D3DTFG_ANISOTROPIC);
                this->Device->SetTextureStageState(i, D3DTSS_MAXANISOTROPY, this->DeviceCaps.dwMaxAnisotropy);
            }

            g_TextureStageMinFilter = D3DTFN_ANISOTROPIC;
            g_TextureStageMagFilter = D3DTFG_ANISOTROPIC;
        }
        else {
            for (u32 i = 0; i < g_MaxSimTextures; i++) {
                this->Device->SetTextureStageState(i, D3DTSS_MINFILTER, D3DTFN_LINEAR);
                this->Device->SetTextureStageState(i, D3DTSS_MAGFILTER, 2);
                this->Device->SetTextureStageState(i, D3DTSS_MAXANISOTROPY, 1);
            }

            g_TextureStageMinFilter = D3DTFN_LINEAR;
            g_TextureStageMagFilter = D3DTFG_LINEAR;
        }

        g_pSysInterface->Anisotropy = (GraphicsAnisotropy)value;

        return;
    }
    case ZRENDERFEATURE_ANTIALIAS: {
        if (!(this->DeviceCaps.dpcTriCaps.dwRasterCaps & D3DPRASTERCAPS_ANTIALIASSORTINDEPENDENT)) {
            g_pSysCom->Log("Z:\\Engine\\Drawing\\_Wintel\\RenderD3D\\Source\\RenderWintelD3DDraw.cpp", 709)
                ->LogMessage("WARNING: Antialiasing not supported");
            return;
        }

        this->Device->SetRenderState(D3DRENDERSTATE_ANTIALIAS,
            value ? D3DANTIALIAS_SORTINDEPENDENT : D3DANTIALIAS_NONE);

        g_pSysInterface->Antialias = value;

        return;
    }
    case ZRENDERFEATURE_TEXTURECOMPRESSION: {
        if (!this->TextureManager->SupportsCompression()) {
            g_pSysCom->Log("Z:\\Engine\\Drawing\\_Wintel\\RenderD3D\\Source\\RenderWintelD3DDraw.cpp", 743)
                ->LogMessage("WARNING: Texture compression not supported");
            return;
        }

        g_pSysInterface->DisableDXT = value ? false : true;

        return;
    }
    case ZRENDERFEATURE_TRILINEARFILTERING: {
        if (!(this->DeviceCaps.dpcTriCaps.dwTextureFilterCaps & D3DPTFILTERCAPS_LINEARMIPLINEAR)) {
            g_pSysCom->Log("Z:\\Engine\\Drawing\\_Wintel\\RenderD3D\\Source\\RenderWintelD3DDraw.cpp", 731)
                ->LogMessage("WARNING: Trilinear filtering not supported");

            return;
        }

        if (value) {
            for (u32 i = 0; i < g_MaxSimTextures; i++) {
                this->Device->SetTextureStageState(i, D3DTSS_MIPFILTER, D3DTFP_LINEAR);
            }

            g_TextureStageMipFilter = D3DTFP_LINEAR;
        }
        else {
            for (u32 i = 0; i < g_MaxSimTextures; i++) {
                this->Device->SetTextureStageState(i, D3DTSS_MIPFILTER, D3DTFP_POINT);
            }

            g_TextureStageMipFilter = D3DTFP_POINT;
        }

        g_pSysInterface->TrilinearFiltering = value;

        return;
    }
    }
}

// 0x0fb82ca0
void ZRenderD3D::Clear(u32 color) {
    this->Device->Clear(0, NULL, D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET, color, 1.0f, 0);
}

// 0x0fb82cd0
bool ZRenderD3D::BeginScene() {
    HRESULT hr = DD_OK;

    if (FAILED(hr = this->DirectDraw->TestCooperativeLevel())) {
        if (hr == DDERR_EXCLUSIVEMODEALREADYSET) {
            g_pSysCom->Log("Z:\\Engine\\Drawing\\_Wintel\\RenderD3D\\Source\\RenderWintelD3DDraw.cpp", 766)
                ->LogMessage("Exclusive already set - %.2f", g_pSysInterface->Unk0x37C5);

            return false;
        }

        if (hr == DDERR_NOEXCLUSIVEMODE) {
            g_pSysCom->Log("Z:\\Engine\\Drawing\\_Wintel\\RenderD3D\\Source\\RenderWintelD3DDraw.cpp", 770)
                ->LogMessage("No Exclusive - %.2f", g_pSysInterface->Unk0x37C5);

            return false;
        }

        if (hr == DDERR_WRONGMODE) {
            g_pSysCom->Log("Z:\\Engine\\Drawing\\_Wintel\\RenderD3D\\Source\\RenderWintelD3DDraw.cpp", 775)
                ->LogMessage("Wrong mode");

            return false;
        }
    }

    if (FAILED(this->Device->BeginScene())) {
        return false;
    }

    g_Device = this->Device;

    this->VertexBufferManager->FUN_0fb9ace0();

    return true;
}

// 0x0fb82de0
bool ZRenderD3D::EndScene() {
    this->VertexBufferManager->FUN_0fb9ad40();

    HRESULT hr = this->Device->EndScene();

    if (FAILED(hr) || hr != DDERR_SURFACELOST) {
        LogDirectDrawSceneError(hr);
    }

    return true;
}

// 0x0fb82e10
void ZRenderD3D::ApplyCurrentState() {
    this->VertexBufferManager->FUN_0fb9adb0();

    this->Device->SetRenderState(D3DRENDERSTATE_SRCBLEND, g_CurrentRenderState.SrcBlend);
    this->Device->SetRenderState(D3DRENDERSTATE_DESTBLEND, g_CurrentRenderState.DestBlend);
    this->Device->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, g_CurrentRenderState.AlphaBlendEnable);
    this->Device->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, g_CurrentRenderState.ZWriteEnable);
    this->Device->SetRenderState(D3DRENDERSTATE_ZFUNC, g_CurrentRenderState.ZFunc);
    this->Device->SetRenderState(D3DRENDERSTATE_ALPHAFUNC, g_CurrentRenderState.AlphaFunc);
    this->Device->SetRenderState(D3DRENDERSTATE_FOGENABLE, g_CurrentRenderState.FogEnable);
    this->Device->SetRenderState(D3DRENDERSTATE_FOGCOLOR, g_CurrentRenderState.FogColor);
    this->Device->SetRenderState(D3DRENDERSTATE_FOGSTART, g_CurrentRenderState.FogStart);
    this->Device->SetRenderState(D3DRENDERSTATE_FOGEND, g_CurrentRenderState.FogEnd);

    for (u32 i = 0; i < g_MaxSimTextures; i++) {
        this->Device->SetTexture(i, g_CurrentRenderState.Texture[i]);

        this->Device->SetTextureStageState(i, D3DTSS_COLOROP, g_CurrentRenderState.ColorOperation[i]);
        this->Device->SetTextureStageState(i, D3DTSS_COLORARG1, g_CurrentRenderState.ColorArg1[i]);
        this->Device->SetTextureStageState(i, D3DTSS_COLORARG2, g_CurrentRenderState.ColorArg2[i]);
        this->Device->SetTextureStageState(i, D3DTSS_ALPHAOP, g_CurrentRenderState.AlphaOperation[i]);
        this->Device->SetTextureStageState(i, D3DTSS_ALPHAARG1, g_CurrentRenderState.AlphaArg1[i]);
        this->Device->SetTextureStageState(i, D3DTSS_ALPHAARG2, g_CurrentRenderState.AlphaArg2[i]);
        this->Device->SetTextureStageState(i, D3DTSS_ADDRESS, 1);
    }
}

// 0x0fb82fd0
void ZRenderD3D::InitializeCurrentState() {
    g_CurrentRenderState.SrcBlend = D3DBLEND_ONE;
    g_CurrentRenderState.DestBlend = D3DBLEND_ZERO;
    g_CurrentRenderState.AlphaBlendEnable = FALSE;
    g_CurrentRenderState.ZWriteEnable = TRUE;
    g_CurrentRenderState.ZFunc = D3DCMP_LESSEQUAL;
    g_CurrentRenderState.Unk0xF8 = 0; // TODO
    g_CurrentRenderState.AlphaFunc = D3DCMP_ALWAYS;
    g_CurrentRenderState.FogEnable = TRUE;
    g_CurrentRenderState.FogColor = 0x00000000;
    g_CurrentRenderState.Unk0x100 = 0; // TODO
    g_CurrentRenderState.Unk0x104 = 0xFF000000;
    g_CurrentRenderState.Unk0x108 = 0xFFFFFFFF;

    const f32 start = 0.7f;
    const f32 end = 1.0f;

    g_CurrentRenderState.FogStart = *(DWORD*)(&start);
    g_CurrentRenderState.FogEnd = *(DWORD*)(&end);
    g_CurrentRenderState.Unk0xF4 = 0;   // TODO

    for (u32 i = 0; i < g_MaxSimTextures; i++) {
        g_CurrentRenderState.Texture[i] = nullptr;
        g_CurrentRenderState.ColorOperation[i] = D3DTOP_DISABLE;
        g_CurrentRenderState.ColorArg1[i] = D3DTA_TEXTURE;
        g_CurrentRenderState.ColorArg2[i] = D3DTA_CURRENT;
        g_CurrentRenderState.AlphaOperation[i] = D3DTOP_DISABLE;
        g_CurrentRenderState.AlphaArg1[i] = D3DTA_TEXTURE;
        g_CurrentRenderState.AlphaArg2[i] = D3DTA_CURRENT;
    }

    this->ApplyCurrentState();
}

// 0x0fb83130
void ZRenderD3D::SetViewport(f32 x, f32 y, f32 width, f32 height) {
    D3DVIEWPORT7 vp;

    vp.dwX = (u32)x;
    vp.dwY = (u32)y;
    vp.dwWidth = (u32)width;
    vp.dwHeight = (u32)height;
    vp.dvMinZ = 0.0f;
    vp.dvMaxZ = 1.0f;

    this->Device->SetViewport(&vp);
}

// 0x0fb831b0
void ZRenderD3D::ClearSurface(u32 options, u32 color) {
    DWORD flags = (options & ZRENDERCLEAR_TARGET) ? D3DCLEAR_TARGET : 0;

    if (options & ZRENDERCLEAR_ZBUFFER) {
        flags |= D3DCLEAR_ZBUFFER;
    }

    this->Device->Clear(0, NULL, flags, color, 1.0f, 0);
}

// 0x0fb83420
void ZRenderD3D::Method0x2C(u32 todo) {
    // TODO NOT IMPLEMENTED
}

// 0x0fb83d20
void ZRenderD3D::EnableDepthBuffer() {
    HRESULT hr = DD_OK;

    if (FAILED(hr = this->Blt(false))) {
        if (hr == DDERR_SURFACELOST) {
            this->RestoreAllSurfaces();
        }
        else {
            LogDirectDrawError(hr);
        }
    }

    g_DepthBufferType = !(this->DeviceCaps.dpcTriCaps.dwRasterCaps & D3DPRASTERCAPS_WBUFFER)
        || g_pSysInterface->EnableZBuffer ? D3DZB_TRUE : D3DZB_USEW;

    this->Device->SetRenderState(D3DRENDERSTATE_ZENABLE, g_DepthBufferType);
}

// 0x0fb83d80
void ZRenderD3D::Method0x64(u32 todo) {
    // TODO NOT IMPLEMENTED
}

// 0x0fb83c50
void ZRenderD3D::Method0x1C8(HWND window) {
    if (!g_pSysInterface->Unk0x38F1) {
        this->Window = window;
        this->DisplayRenderWindow("Direct3D");
        this->Initialize();
    }
    else {
        RECT rect;
        GetClientRect(window, &rect);

        this->CreateRenderWindow(nullptr, "Direct3D",
            WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0, &rect, window);

        this->ShowRenderWindow(SW_SHOW);
    }

    DEVMODEA mode;
    ZeroMemory(&mode, sizeof(DEVMODEA));

    mode.dmSize = sizeof(DEVMODEA);

    EnumDisplaySettingsA(nullptr, ENUM_CURRENT_SETTINGS, &mode);

    this->DisplayFrequency = mode.dmDisplayFrequency;
}

// 0x0fb840e0
void ZRenderD3D::Method0xE4() {
    D3DMATRIX matrix;

    matrix._11 = 1.0f;
    matrix._12 = 0.0f;
    matrix._13 = 0.0f;
    matrix._14 = 0.0f;

    matrix._21 = 0.0f;
    matrix._22 = 1.0f;
    matrix._23 = 0.0f;
    matrix._24 = 0.0f;

    matrix._31 = 0.0f;
    matrix._32 = 0.0f;
    matrix._33 = 1.0f;
    matrix._34 = 0.0f;

    matrix._41 = 0.0f;
    matrix._42 = 0.0f;
    matrix._43 = 0.0f;
    matrix._44 = 1.0f;

    this->Device->SetTransform(D3DTRANSFORMSTATE_PROJECTION, &matrix);

    if (g_CurrentRenderState.ColorOperation[0] != D3DTOP_SELECTARG1) {
        g_CurrentRenderState.ColorOperation[0] = D3DTOP_SELECTARG1;
        g_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    }

    if (g_CurrentRenderState.ColorArg1[0] != D3DTA_TEXTURE) {
        g_CurrentRenderState.ColorArg1[0] = D3DTA_TEXTURE;
        g_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    }

    if (g_CurrentRenderState.ColorArg2[0] != D3DTA_CURRENT) {
        g_CurrentRenderState.ColorArg2[0] = D3DTA_CURRENT;
        g_Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
    }

    g_Device->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);

    if (1 < g_MaxSimTextures) {
        if (g_CurrentRenderState.ColorOperation[1] != D3DTOP_DISABLE) {
            g_CurrentRenderState.ColorOperation[1] = D3DTOP_DISABLE;
            g_Device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
        }

        if (g_CurrentRenderState.ColorArg1[1] != D3DTA_TEXTURE) {
            g_CurrentRenderState.ColorArg1[1] = D3DTA_TEXTURE;
            g_Device->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
        }

        if (g_CurrentRenderState.ColorArg2[1] != D3DTA_CURRENT) {
            g_CurrentRenderState.ColorArg2[1] = D3DTA_CURRENT;
            g_Device->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
        }

        g_Device->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
    }

    if (g_CurrentRenderState.AlphaOperation[0] != D3DTOP_SELECTARG1) {
        g_CurrentRenderState.AlphaOperation[0] = D3DTOP_SELECTARG1;
        g_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    }

    if (g_CurrentRenderState.AlphaArg1[0] != D3DTA_TEXTURE) {
        g_CurrentRenderState.AlphaArg1[0] = D3DTA_TEXTURE;
        g_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    }

    if (g_CurrentRenderState.AlphaArg2[0] != D3DTA_CURRENT) {
        g_CurrentRenderState.AlphaArg2[0] = D3DTA_CURRENT;
        g_Device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
    }

    if (1 < g_MaxSimTextures) {
        if (g_CurrentRenderState.AlphaOperation[1] != D3DTOP_DISABLE) {
            g_CurrentRenderState.AlphaOperation[1] = D3DTOP_DISABLE;
            g_Device->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
        }

        if (g_CurrentRenderState.AlphaArg1[1] != D3DTA_TEXTURE) {
            g_CurrentRenderState.AlphaArg1[1] = D3DTA_TEXTURE;
            g_Device->SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
        }

        if (g_CurrentRenderState.AlphaArg2[1] != D3DTA_CURRENT) {
            g_CurrentRenderState.AlphaArg2[1] = D3DTA_CURRENT;
            g_Device->SetTextureStageState(1, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
        }
    }

    if (g_CurrentRenderState.SrcBlend != D3DBLEND_SRCALPHA) {
        g_CurrentRenderState.SrcBlend = D3DBLEND_SRCALPHA;
        g_Device->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
    }

    if (g_CurrentRenderState.DestBlend != D3DBLEND_INVSRCALPHA) {
        g_CurrentRenderState.DestBlend = D3DBLEND_INVSRCALPHA;
        g_Device->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
    }

    if (!g_CurrentRenderState.AlphaBlendEnable) {
        g_CurrentRenderState.AlphaBlendEnable = TRUE;
        g_Device->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
    }

    if (g_CurrentRenderState.ZWriteEnable) {
        g_CurrentRenderState.ZWriteEnable = FALSE;
        g_Device->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, FALSE);
    }

    g_Device->SetRenderState(D3DRENDERSTATE_ZFUNC, D3DCMP_ALWAYS);

    if (g_CurrentRenderState.Texture[0] != DAT_0fbbef0c) {
        g_CurrentRenderState.Texture[0] = DAT_0fbbef0c;
        g_Device->SetTexture(0, DAT_0fbbef0c);
    }

    // TODO TYDY

    const f32 a = 32.0f / this->GetWindowWidth();
    const f32 b = (32.0f / this->GetWindowHeight()) * 0.5f;

    matrix._32 = this->Unk0x3E + this->Unk0x3E - a * 0.5f;
    matrix._33 = -2.0f * this->Unk0x42 - b;
    matrix._34 = 0.0f;

    matrix._41 = a * 0.5f;
    matrix._42 = -b;
    matrix._43 = matrix._32 + a;
    matrix._44 = matrix._33;

    g_Device->DrawPrimitive(D3DPT_TRIANGLEFAN, D3DFVF_TEX1 | D3DFVF_XYZ, &matrix._32, 4, 0);
    g_Device->SetRenderState(D3DRENDERSTATE_ZFUNC, D3DCMP_LESSEQUAL);
}

// 0x0fb84570
void ZRenderD3D::Method0x4C(u32 todo1, u32 todo2, u32 todo3) {
    // TODO NOT IMPLEMENTED
}

// 0x0fb84b80
void ZRenderD3D::Method0x54() {
    // TODO NOT IMPLEMENTED
}

// 0x0fb85620
void ZRenderD3D::Method0xD4() {
    // TODO NOT IMPLEMENTED
}

// 0x0fb862a0
s32 EnumerateDirectDrawDevices(LPFNACCEPTDEVICECALLBACK callback) {
    g_DeviceCallback = callback;

    DirectDrawEnumerateExA(EnumerateDirectDrawDevicesCallback, nullptr,
        DDENUM_NONDISPLAYDEVICES | DDENUM_DETACHEDSECONDARYDEVICES | DDENUM_ATTACHEDSECONDARYDEVICES);

    if (g_DeviceCount == 0) {
        g_pSysCom->Log("Z:\\Engine\\Drawing\\_Wintel\\RenderD3D\\Source\\RenderWintelD3DHelper.cpp", 267)
            ->LogMessage("No devices and/or modes were enumerated!");

        return -1;
    }

    if (g_AcceptedDeviceCount == 0) {
        g_pSysCom->Log("Z:\\Engine\\Drawing\\_Wintel\\RenderD3D\\Source\\RenderWintelD3DHelper.cpp", 272)
            ->LogMessage("No enumerated devices were accepted!");

        g_pSysCom->Log("Z:\\Engine\\Drawing\\_Wintel\\RenderD3D\\Source\\RenderWintelD3DHelper.cpp", 273)
            ->LogMessage("Try enabling the D3D Reference Rasterizer.");

        return -1;
    }

    return 0;
}

// 0x0fb86340
BOOL WINAPI EnumerateDirectDrawDevicesCallback(LPGUID lpGUID, LPSTR lpDriverDescription, LPSTR lpDriverName, LPVOID lpContext, HMONITOR hm) {
    LPDIRECTDRAW7 dd = nullptr;
    if (FAILED(DirectDrawCreateEx(lpGUID, (LPVOID*)&dd, IID_IDirectDraw7, nullptr))) {
        g_pSysCom->Log("Z:\\Engine\\Drawing\\_Wintel\\RenderD3D\\Source\\RenderWintelD3DHelper.cpp", 194)
            ->LogMessage("Can't create DDraw during enumeration!");

        return TRUE;
    }

    LPDIRECT3D7 d3d = nullptr;
    if (FAILED(dd->QueryInterface(IID_IDirect3D7, (LPVOID*)&d3d))) {
        dd->Release();

        g_pSysCom->Log("Z:\\Engine\\Drawing\\_Wintel\\RenderD3D\\Source\\RenderWintelD3DHelper.cpp", 203)
            ->LogMessage("Can't query IDirect3D7 during enumeration!");

        return TRUE;
    }

    ZRenderDevice device;
    ZeroMemory(&device, sizeof(ZRenderDevice));

    lstrcpynA(device.Name, lpDriverDescription, MAX_RENDER_DEVICE_NAME_LENGTH - 1);

    device.HAL.dwSize = sizeof(DDCAPS);
    device.HEL.dwSize = sizeof(DDCAPS);

    dd->GetCaps(&device.HAL, &device.HEL);

    if (lpGUID != nullptr) {
        CopyMemory(&device.IDDDValue, lpGUID, sizeof(GUID));
        device.IDDD = &device.IDDDValue;
    }

    if ((device.HAL.dwCaps2 & DDCAPS2_CANRENDERWINDOWED) && device.IDDD == nullptr) {
        device.Window = TRUE;
    }

    dd->EnumDisplayModes(g_pSysInterface->RefreshRate != 0 /* DDEDM_REFRESHRATES  */,
        nullptr, &device, EnumerateDirectDrawDisplayModes);

    qsort(device.Modes, device.ModeCount, sizeof(DDSURFACEDESC2), CompareDirectDrawDisplayMode);

    d3d->EnumDevices(EnumerateDirect3DDevices, &device);

    if (device.Modes != nullptr) {
        delete[] device.Modes;
    }

    d3d->Release();
    dd->Release();

    return TRUE;
}

// 0x0fb86230
s32 __cdecl CompareDirectDrawDisplayMode(void const* a, void const* b) {
    LPDDSURFACEDESC2 l = (LPDDSURFACEDESC2)a;
    LPDDSURFACEDESC2 r = (LPDDSURFACEDESC2)b;

    if (l->ddpfPixelFormat.dwRGBBitCount < r->ddpfPixelFormat.dwRGBBitCount) { return -1; }
    if (r->ddpfPixelFormat.dwRGBBitCount < l->ddpfPixelFormat.dwRGBBitCount) { return 1; }

    if (l->dwWidth < r->dwWidth) { return -1; }
    if (r->dwWidth < l->dwWidth) { return 1; }

    if (l->dwHeight < r->dwHeight) { return -1; }
    if (r->dwHeight < l->dwHeight) { return 1; }
    if (r->dwMipMapCount < l->dwMipMapCount) { return -1; }

    return l->dwMipMapCount < r->dwMipMapCount;
}

// 0x0fb86530
HRESULT WINAPI EnumerateDirectDrawDisplayModes(LPDDSURFACEDESC2 lpDDSurfaceDesc, LPVOID lpContext) {
    ZRenderDevice* device = (ZRenderDevice*)lpContext;

    LPDDSURFACEDESC2 modes = new DDSURFACEDESC2[device->ModeCount + 1];

    CopyMemory(modes, device->Modes, device->ModeCount * sizeof(DDSURFACEDESC2));

    if (device->Modes != nullptr) {
        delete[] device->Modes;
    }

    device->Modes = modes;

    CopyMemory(&device->Modes[device->ModeCount], lpDDSurfaceDesc, sizeof(DDSURFACEDESC2));

    device->ModeCount++;

    return DDENUMRET_OK;
}

// 0x0fb865d0
HRESULT CALLBACK EnumerateDirect3DDevices(LPSTR lpDeviceDescription, LPSTR lpDeviceName, LPD3DDEVICEDESC7 lpD3DDeviceDesc, LPVOID lpContext) {
    ZRenderDevice* device = (ZRenderDevice*)lpContext;

    g_DeviceCount++;

    ZeroMemory(&g_Devices[g_AcceptedDeviceCount], sizeof(ZRenderDevice));

    g_Devices[g_AcceptedDeviceCount].Acceleration
        = lpD3DDeviceDesc->dwDevCaps & D3DDEVCAPS_HWRASTERIZATION;

    CopyMemory(&g_Devices[g_AcceptedDeviceCount], lpD3DDeviceDesc, sizeof(D3DDEVICEDESC7));

    g_Devices[g_AcceptedDeviceCount].Window = device->Window;

    CopyMemory(&g_Devices[g_AcceptedDeviceCount].HAL, &device->HAL, sizeof(DDCAPS));
    CopyMemory(&g_Devices[g_AcceptedDeviceCount].HEL, &device->HEL, sizeof(DDCAPS));

    CopyMemory(&g_Devices[g_AcceptedDeviceCount].IDD3DValue, &lpD3DDeviceDesc->deviceGUID, sizeof(GUID));

    g_Devices[g_AcceptedDeviceCount].IDD3D = &g_Devices[g_AcceptedDeviceCount].IDD3DValue;

    g_Devices[g_AcceptedDeviceCount].Modes = new DDSURFACEDESC2[device->ModeCount];

    if (device->IDDD == nullptr) {
        g_Devices[g_AcceptedDeviceCount].IDDD = nullptr;
        lstrcpynA(g_Devices[g_AcceptedDeviceCount].Name, lpDeviceName, MAX_RENDER_DEVICE_NAME_LENGTH - 1);
    }
    else {
        CopyMemory(&g_Devices[g_AcceptedDeviceCount].IDDDValue, &device->IDDDValue, sizeof(GUID));

        g_Devices[g_AcceptedDeviceCount].IDDD = &g_Devices[g_AcceptedDeviceCount].IDDDValue;
        lstrcpynA(g_Devices[g_AcceptedDeviceCount].Name, device->Name, MAX_RENDER_DEVICE_NAME_LENGTH - 1);
    }

    if ((g_Devices[g_AcceptedDeviceCount].IDDD == nullptr || g_Devices[g_AcceptedDeviceCount].Acceleration)
        && (g_DeviceCallback == nullptr
            || SUCCEEDED(g_DeviceCallback(&g_Devices[g_AcceptedDeviceCount].HAL, &g_Devices[g_AcceptedDeviceCount].Desc)))) {
        for (u32 i = 0; i < device->ModeCount; i++) {
            LPDDSURFACEDESC2 mode = &device->Modes[i];

            u32 options = g_Devices[g_AcceptedDeviceCount].Desc.dwDeviceRenderBitDepth;

            if (mode->ddpfPixelFormat.dwRGBBitCount == 32) {
                options &= 0x100; // TODO
            }
            else if (mode->ddpfPixelFormat.dwRGBBitCount == 24) {
                options &= 0x200; // TODO
            }
            else if (mode->ddpfPixelFormat.dwRGBBitCount == 16) {
                options &= 0x400; // TODO
            }

            if (options != 0 /* TODO */) {
                CopyMemory(&g_Devices[g_AcceptedDeviceCount].Modes[g_Devices[g_AcceptedDeviceCount].ModeCount],
                    mode, sizeof(DDSURFACEDESC2));

                g_Devices[g_AcceptedDeviceCount].ModeCount++;

                if (mode->ddsCaps.dwCaps2 & DDSCAPS2_STEREOSURFACELEFT) {
                    g_Devices[g_AcceptedDeviceCount].Stereo = TRUE;
                }
            }
        }

        if (g_Devices[g_AcceptedDeviceCount].ModeCount != 0) {
            for (u32 i = 0; i < g_Devices[g_AcceptedDeviceCount].ModeCount; i++) {
                LPDDSURFACEDESC2 mode = &g_Devices[g_AcceptedDeviceCount].Modes[i];

                if (mode->dwWidth == 640 && mode->dwHeight == 480
                    && mode->ddpfPixelFormat.dwRGBBitCount == 16) {
                    CopyMemory(&g_Devices[g_AcceptedDeviceCount].Active, mode, sizeof(DDSURFACEDESC2));
                    g_Devices[g_AcceptedDeviceCount].Index = i;
                }
            }

            g_Devices[g_AcceptedDeviceCount].Unk0x494 = g_Devices[g_AcceptedDeviceCount].Window;

            g_AcceptedDeviceCount++;
        }
    }

    return DDENUMRET_OK;
}

// 0x0fb868a0
void ZRenderD3D::InitializeRendererValues() {
    this->FullScreen = FALSE;
    this->StereoView = FALSE;

    this->WindowWidth = 0;
    this->WindowHeight = 0;

    this->MainSurface = nullptr;
    this->BackSurface = nullptr;
    this->StereoSurface = nullptr;
    this->DepthSurface = nullptr;
    this->Device = nullptr;
    this->DirectDraw = nullptr;
    this->D3D = nullptr;

    this->Capabilities = 0;
}

// 0x0fb868f0
HRESULT ZRenderD3D::ReleaseDirectDraw() {
    ULONG rcd = 0, rcdd = 0;

    if (this->DirectDraw != nullptr) {
        this->DirectDraw->SetCooperativeLevel(this->Window, DDSCL_NORMAL);
    }

    if (this->Device != nullptr) {
        if ((rcd = this->Device->Release()) != 0) {
            g_pSysCom->Log("Z:\\Engine\\Drawing\\_Wintel\\RenderD3D\\Source\\RenderWintelD3DInit.cpp", 53)
                ->LogMessage("Error: D3DDevice object is still referenced %d times!");
        }
    }

    this->Device = nullptr;

    if (this->BackSurface != nullptr) {
        this->BackSurface->Release();
        this->BackSurface = nullptr;
    }

    if (this->StereoSurface != nullptr) {
        this->StereoSurface->Release();
        this->StereoSurface = nullptr;
    }

    if (this->DepthSurface != nullptr) {
        this->DepthSurface->Release();
        this->DepthSurface = nullptr;
    }

    if (this->MainSurface != nullptr) {
        this->MainSurface->Release();
        this->MainSurface = nullptr;
    }

    if (this->D3D != nullptr) {
        this->D3D->Release();
        this->D3D = nullptr;
    }

    if (this->DirectDraw != nullptr) {
        if ((rcdd = this->DirectDraw->Release()) != 0) {
            g_pSysCom->Log("Z:\\Engine\\Drawing\\_Wintel\\RenderD3D\\Source\\RenderWintelD3DInit.cpp", 66)
                ->LogMessage("Error: DDraw object is still referenced %d times!");
        }
    }

    this->DirectDraw = nullptr;

    return (rcd == 0 && rcdd == 0) ? DD_OK : ZERR_8200000C;
}

// 0x0fb86a20
HRESULT ZRenderD3D::InitializeD3D(LPGUID dd, LPCGUID d3d, LPDDSURFACEDESC2 settings, u32 options) {
    if (d3d != nullptr) {
        if (settings != nullptr || !(options & ZRENDEROPTION_FULL_SCREEN)) {
            this->StereoView = FALSE;
            this->FullScreen = options & ZRENDEROPTION_FULL_SCREEN;

            if ((options & ZRENDEROPTION_STEREO_VIEW)
                && !(options & ZRENDEROPTION_FULL_SCREEN)
                && (settings->ddsCaps.dwCaps2 & DDSCAPS2_STEREOSURFACELEFT)) {
                this->StereoView = TRUE;
            }

            HRESULT hr = DD_OK;
            if (FAILED(hr = this->InitializeDirectDraw(dd, d3d, settings, options))) {
                this->ReleaseDirectDraw();
                return hr;
            }

            return DD_OK;
        }
    }

    return DDERR_INVALIDPARAMS;
}

// 0x0fb86ab0
HRESULT ZRenderD3D::InitializeDirectDraw(LPGUID dd, LPCGUID d3d, LPDDSURFACEDESC2 settings, u32 options) {
    if (memcmp(&IID_IDirect3DHALDevice, d3d, sizeof(GUID))) {
        this->Capabilities = DDSCAPS_VIDEOMEMORY;
    }
    else if (memcmp(&IID_IDirect3DTnLHalDevice, d3d, sizeof(GUID))) {
        this->Capabilities = DDSCAPS_VIDEOMEMORY;
    }
    else {
        this->Capabilities = DDSCAPS_SYSTEMMEMORY;
    }

    HRESULT hr = DD_OK;
    if (SUCCEEDED(hr = this->CreateDirectDraw(dd, options))) {
        if (!(options & ZRENDEROPTION_FULL_SCREEN)) {
            hr = this->CreateWindowSurfaces();
        }
        else {
            hr = this->CreateFullScreenSurfaces(settings);
        }

        if (SUCCEEDED(hr)) {
            if (SUCCEEDED(hr = this->CreateD3D(d3d))) {
                if (options & ZRENDEROPTION_DEPTH_BUFFER) {
                    hr = this->CreateDepthBuffer(d3d);
                }
            }
        }
    }

    return hr;
}

// 0x0fb86b70
HRESULT ZRenderD3D::CreateDirectDraw(LPGUID device, u32 options) {
    if (FAILED(DirectDrawCreateEx(device, (LPVOID*)&this->DirectDraw, IID_IDirectDraw7, nullptr))) {
        g_pSysCom->Log("Z:\\Engine\\Drawing\\_Wintel\\RenderD3D\\Source\\RenderWintelD3DInit.cpp", 186)
            ->LogMessage("Could not create DirectDraw");

        return ZERR_82000001;
    }

    DWORD flags = DDSCL_NORMAL;
    if (this->FullScreen) {
        flags = DDSCL_EXCLUSIVE | DDSCL_NOWINDOWCHANGES | DDSCL_ALLOWREBOOT | DDSCL_FULLSCREEN;
    }

    if (!(options & ZRENDEROPTION_PRESERVE_FPU)) {
        flags |= DDSCL_FPUSETUP;
    }

    if (FAILED(this->DirectDraw->SetCooperativeLevel(this->Window, flags))) {
        g_pSysCom->Log("Z:\\Engine\\Drawing\\_Wintel\\RenderD3D\\Source\\RenderWintelD3DInit.cpp", 201)
            ->LogMessage("Couldn't set coop level");

        return ZERR_82000002;
    }

    DDSURFACEDESC2 desc;
    desc.dwSize = sizeof(desc);

    this->DirectDraw->GetDisplayMode(&desc);

    return desc.ddpfPixelFormat.dwRGBBitCount > 8 ? DD_OK : ZERR_8200000E;
}

// 0x0fb86c70
HRESULT ZRenderD3D::CreateFullScreenSurfaces(LPDDSURFACEDESC2 settings) {
    HRESULT hr = DD_OK;

    SetRect(&this->WindowRectangle, 0, 0, settings->dwWidth, settings->dwHeight);

    this->WindowWidth = this->WindowRectangle.right - this->WindowRectangle.left;
    this->WindowHeight = this->WindowRectangle.bottom - this->WindowRectangle.top;

    const u32 flags =
        (this->WindowWidth == 320 && this->WindowHeight == 200 && settings->ddpfPixelFormat.dwRGBBitCount == 8)
        ? DDSDM_STANDARDVGAMODE : 0;

    if (FAILED(this->DirectDraw->SetDisplayMode(this->WindowWidth,
        this->WindowHeight, settings->ddpfPixelFormat.dwRGBBitCount, settings->dwRefreshRate, flags))) {
        g_pSysCom->Log("Z:\\Engine\\Drawing\\_Wintel\\RenderD3D\\Source\\RenderWintelD3DInit.cpp", 249)
            ->LogMessage("Can't set display mode");

        return ZERR_8200000A;
    }

    DDSURFACEDESC2 desc;
    ZeroMemory(&desc, sizeof(DDSURFACEDESC2));

    desc.dwSize = sizeof(DDSURFACEDESC2);
    desc.dwFlags = DDSD_BACKBUFFERCOUNT | DDSD_CAPS;
    desc.ddsCaps.dwCaps = DDSCAPS_3DDEVICE | DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;

    if (this->StereoView) {
        desc.ddsCaps.dwCaps =
            DDSCAPS_VIDEOMEMORY | DDSCAPS_3DDEVICE | DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;
        desc.ddsCaps.dwCaps2 = desc.ddsCaps.dwCaps2 | DDSCAPS2_STEREOSURFACELEFT;
    }

    if (g_pSysInterface->EnableTripleBuffering) {
        desc.dwBackBufferCount = 2;

        if (FAILED(hr = this->DirectDraw->CreateSurface(&desc, &this->MainSurface, nullptr))) {
            if (g_pSysInterface->DebugVideo) {
                g_pSysCom->Log("Z:\\Engine\\Drawing\\_Wintel\\RenderD3D\\Source\\RenderWintelD3DInit.cpp", 275)
                    ->LogMessage("Double buffering enabled");
            }

            desc.dwBackBufferCount = 1;

            if (FAILED(hr = this->DirectDraw->CreateSurface(&desc, &this->MainSurface, nullptr))) {
                g_pSysCom->Log("Z:\\Engine\\Drawing\\_Wintel\\RenderD3D\\Source\\RenderWintelD3DInit.cpp", 279)
                    ->LogMessage("Error: Can't create primary surface");

                if (hr != DDERR_OUTOFVIDEOMEMORY) {
                    return ZERR_82000008;
                }

                g_pSysCom->Log("Z:\\Engine\\Drawing\\_Wintel\\RenderD3D\\Source\\RenderWintelD3DInit.cpp", 282)
                    ->LogMessage("Error: Out of video memory");

                return DDERR_OUTOFVIDEOMEMORY;
            }
        }

        if (g_pSysInterface->DebugVideo) {
            g_pSysCom->Log("Z:\\Engine\\Drawing\\_Wintel\\RenderD3D\\Source\\RenderWintelD3DInit.cpp", 286)
                ->LogMessage("Triple buffering enabled");
        }
    }
    else {
        if (g_pSysInterface->DebugVideo) {
            g_pSysCom->Log("Z:\\Engine\\Drawing\\_Wintel\\RenderD3D\\Source\\RenderWintelD3DInit.cpp", 289)
                ->LogMessage("Double buffering enabled");
        }

        desc.dwBackBufferCount = 1;

        if (FAILED(hr = this->DirectDraw->CreateSurface(&desc, &this->MainSurface, nullptr))) {
            g_pSysCom->Log("Z:\\Engine\\Drawing\\_Wintel\\RenderD3D\\Source\\RenderWintelD3DInit.cpp", 293)
                ->LogMessage("Error: Can't create primary surface");

            if (hr != DDERR_OUTOFVIDEOMEMORY) {
                return ZERR_82000008;
            }

            g_pSysCom->Log("Z:\\Engine\\Drawing\\_Wintel\\RenderD3D\\Source\\RenderWintelD3DInit.cpp", 296)
                ->LogMessage("Error: Out of video memory");

            return DDERR_OUTOFVIDEOMEMORY;
        }
    }

    DDSCAPS2 caps;
    ZeroMemory(&caps, sizeof(caps));

    caps.dwCaps = DDSCAPS_BACKBUFFER;

    if (FAILED(hr = this->MainSurface->GetAttachedSurface(&caps, &this->BackSurface))) {
        g_pSysCom->Log("Z:\\Engine\\Drawing\\_Wintel\\RenderD3D\\Source\\RenderWintelD3DInit.cpp", 306)
            ->LogMessage("Error: Can't get the backbuffer");

        return ZERR_8200000B;
    }

    this->BackSurface->AddRef();

    if (this->StereoView) {
        DDSCAPS2 stereo;
        ZeroMemory(&stereo, sizeof(caps));

        stereo.dwCaps2 = DDSCAPS2_STEREOSURFACELEFT;

        if (FAILED(hr = this->BackSurface->GetAttachedSurface(&stereo, &this->StereoSurface))) {
            g_pSysCom->Log("Z:\\Engine\\Drawing\\_Wintel\\RenderD3D\\Source\\RenderWintelD3DInit.cpp", 321)
                ->LogMessage("Error: Can't get the left backbuffer");

            return ZERR_8200000B;
        }

        this->StereoSurface->AddRef();
    }

    return DD_OK;
}

// 0x0fb87060
HRESULT ZRenderD3D::CreateWindowSurfaces() {
    GetClientRect(this->Window, &this->WindowRectangle);
    ClientToScreen(this->Window, (LPPOINT)&this->WindowRectangle.left);
    ClientToScreen(this->Window, (LPPOINT)&this->WindowRectangle.right);

    this->WindowHeight = this->WindowRectangle.bottom - this->WindowRectangle.top;
    this->WindowWidth = this->WindowRectangle.right - this->WindowRectangle.left;

    DDSURFACEDESC2 desc;
    ZeroMemory(&desc, sizeof(DDSURFACEDESC2));

    desc.dwSize = sizeof(DDSURFACEDESC2);
    desc.dwFlags = DDSD_CAPS;
    desc.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

    HRESULT hr = DD_OK;
    if (FAILED(hr = this->DirectDraw->CreateSurface(&desc, &this->MainSurface, nullptr))) {
        g_pSysCom->Log("Z:\\Engine\\Drawing\\_Wintel\\RenderD3D\\Source\\RenderWintelD3DInit.cpp", 358)
            ->LogMessage("Error: Can't create primary surface");

        if (hr != DDERR_OUTOFVIDEOMEMORY) {
            return ZERR_82000008;
        }

        g_pSysCom->Log("Z:\\Engine\\Drawing\\_Wintel\\RenderD3D\\Source\\RenderWintelD3DInit.cpp", 361)
            ->LogMessage("Error: Out of video memory");

        return DDERR_OUTOFVIDEOMEMORY;
    }

    LPDIRECTDRAWCLIPPER clipper;
    if (FAILED(hr = this->DirectDraw->CreateClipper(0, &clipper, nullptr))) {
        g_pSysCom->Log("Z:\\Engine\\Drawing\\_Wintel\\RenderD3D\\Source\\RenderWintelD3DInit.cpp", 369)
            ->LogMessage("Error: Couldn't create clipper");

        return ZERR_82000009;
    }

    clipper->SetHWnd(0, this->Window);
    this->MainSurface->SetClipper(clipper);

    if (clipper != nullptr) {
        clipper->Release();
    }

    desc.dwHeight = this->WindowHeight;
    desc.dwWidth = this->WindowWidth;
    desc.dwFlags = DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS;
    desc.ddsCaps.dwCaps = DDSCAPS_3DDEVICE | DDSCAPS_OFFSCREENPLAIN;

    if (FAILED(hr = this->DirectDraw->CreateSurface(&desc, &this->BackSurface, nullptr))) {
        g_pSysCom->Log("Z:\\Engine\\Drawing\\_Wintel\\RenderD3D\\Source\\RenderWintelD3DInit.cpp", 386)
            ->LogMessage("Error: Couldn't create the backbuffer");

        if (hr != DDERR_OUTOFVIDEOMEMORY) {
            return ZERR_8200000B;
        }

        g_pSysCom->Log("Z:\\Engine\\Drawing\\_Wintel\\RenderD3D\\Source\\RenderWintelD3DInit.cpp", 389)
            ->LogMessage("Error: Out of video memory");

        return DDERR_OUTOFVIDEOMEMORY;
    }

    return DD_OK;
}

// 0x0fb872a0
HRESULT ZRenderD3D::CreateD3D(LPCGUID device) {
    if (FAILED(this->DirectDraw->QueryInterface(IID_IDirect3D7, (LPVOID*)&this->D3D))) {
        g_pSysCom->Log("Z:\\Engine\\Drawing\\_Wintel\\RenderD3D\\Source\\RenderWintelD3DInit.cpp", 408)
            ->LogMessage("Couldn't get the Direct3D interface");

        return ZERR_82000003;
    }

    if (FAILED(this->D3D->CreateDevice(*device, this->BackSurface, &this->Device))) {
        g_pSysCom->Log("Z:\\Engine\\Drawing\\_Wintel\\RenderD3D\\Source\\RenderWintelD3DInit.cpp", 416)
            ->LogMessage("Couldn't create the D3DDevice");

        return ZERR_82000004;
    }

    D3DVIEWPORT7 viewport;

    viewport.dwX = 0;
    viewport.dwY = 0;
    viewport.dwWidth = this->WindowWidth;
    viewport.dwHeight = this->WindowHeight;
    viewport.dvMinZ = 0.0f;
    viewport.dvMaxZ = 1.0f;

    if (FAILED(this->Device->SetViewport(&viewport))) {
        g_pSysCom->Log("Z:\\Engine\\Drawing\\_Wintel\\RenderD3D\\Source\\RenderWintelD3DInit.cpp", 325)
            ->LogMessage("Error: Couldn't set current viewport to device");

        return ZERR_82000007;
    }

    return DD_OK;
}

// 0x0fb873c0
HRESULT ZRenderD3D::CreateDepthBuffer(LPCGUID device) {
    D3DDEVICEDESC7 caps;

    this->Device->GetCaps(&caps);

    if (caps.dpcTriCaps.dwRasterCaps & D3DPRASTERCAPS_ZBUFFERLESSHSR) {
        g_pSysCom->Log("Z:\\Engine\\Drawing\\_Wintel\\RenderD3D\\Source\\RenderWintelD3DInit.cpp", 449)
            ->LogMessage("Using alternate HSR");

        return DD_OK;
    }

    DDSURFACEDESC2 desc;
    ZeroMemory(&desc, sizeof(DDSURFACEDESC2));

    desc.dwSize = sizeof(DDSURFACEDESC2);

    this->BackSurface->GetSurfaceDesc(&desc);

    desc.dwFlags = DDSD_PIXELFORMAT | DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS;
    desc.ddpfPixelFormat.dwSize = 0;
    desc.ddsCaps.dwCaps = this->Capabilities | DDSCAPS_ZBUFFER;

    this->D3D->EnumZBufferFormats(*device, EnumerateDepthBufferFormatsCallback, &desc.ddpfPixelFormat);

    if (desc.ddpfPixelFormat.dwSize == 0) {
        desc.ddpfPixelFormat.dwRGBBitCount = 24;
        this->D3D->EnumZBufferFormats(*device, EnumerateDepthBufferFormatsCallback, &desc.ddpfPixelFormat);

        if (desc.ddpfPixelFormat.dwSize == 0) {
            desc.ddpfPixelFormat.dwRGBBitCount = 16;
            this->D3D->EnumZBufferFormats(*device, EnumerateDepthBufferFormatsCallback, &desc.ddpfPixelFormat);

            if (desc.ddpfPixelFormat.dwSize == 0) {
                g_pSysCom->Log("Z:\\Engine\\Drawing\\_Wintel\\RenderD3D\\Source\\RenderWintelD3DInit.cpp", 484)
                    ->LogMessage("Device doesn't support requested zbuffer format");

                return ZERR_82000005;
            }
        }
    }

    HRESULT hr = DD_OK;
    if (FAILED(hr = this->DirectDraw->CreateSurface(&desc, &this->DepthSurface, nullptr))) {
        g_pSysCom->Log("Z:\\Engine\\Drawing\\_Wintel\\RenderD3D\\Source\\RenderWintelD3DInit.cpp", 493)
            ->LogMessage("Error: Couldn't create a ZBuffer surface");

        if (hr != DDERR_OUTOFVIDEOMEMORY) {
            return ZERR_82000005;
        }

        g_pSysCom->Log("Z:\\Engine\\Drawing\\_Wintel\\RenderD3D\\Source\\RenderWintelD3DInit.cpp", 496)
            ->LogMessage("Error: Out of video memory");

        return DDERR_OUTOFVIDEOMEMORY;
    }

    if (FAILED(this->BackSurface->AddAttachedSurface(this->DepthSurface))) {
        g_pSysCom->Log("Z:\\Engine\\Drawing\\_Wintel\\RenderD3D\\Source\\RenderWintelD3DInit.cpp", 502)
            ->LogMessage("Error: Couldn't attach zbuffer to render surface");

        return ZERR_82000005;
    }

    if (this->StereoView) {
        if (FAILED(this->StereoSurface->AddAttachedSurface(this->DepthSurface))) {
            g_pSysCom->Log("Z:\\Engine\\Drawing\\_Wintel\\RenderD3D\\Source\\RenderWintelD3DInit.cpp", 511)
                ->LogMessage("Error: Couldn't attach zbuffer to left render surface");

            return ZERR_82000005;
        }
    }

    if (FAILED(this->Device->SetRenderTarget(this->BackSurface, 0))) {
        g_pSysCom->Log("Z:\\Engine\\Drawing\\_Wintel\\RenderD3D\\Source\\RenderWintelD3DInit.cpp", 519)
            ->LogMessage("Error: SetRenderTarget() failed after attaching zbuffer!");

        return ZERR_82000005;
    }

    if (g_pSysInterface->DebugVideo) {
        g_pSysCom->Log("Z:\\Engine\\Drawing\\_Wintel\\RenderD3D\\Source\\RenderWintelD3DInit.cpp", 524)
            ->LogMessage("Using %d ZBuffer", desc.ddpfPixelFormat.dwRGBBitCount);
    }

    return DD_OK;
}

// 0x0fb876d0
HRESULT CALLBACK EnumerateDepthBufferFormatsCallback(LPDDPIXELFORMAT lpDDPixFmt, LPVOID lpContext) {
    LPDDPIXELFORMAT format = (LPDDPIXELFORMAT)lpContext;

    if (format->dwRGBBitCount == lpDDPixFmt->dwRGBBitCount) {
        CopyMemory(format, lpDDPixFmt, sizeof(DDPIXELFORMAT));

        return D3DENUMRET_CANCEL;
    }

    return D3DENUMRET_OK;
}

// 0x0fb87700
HRESULT ZRenderD3D::RestoreAllSurfaces() {
    g_pSysCom->Log("Z:\\Engine\\Drawing\\_Wintel\\RenderD3D\\Source\\RenderWintelD3DInit.cpp", 541)
        ->LogMessage("Restoring surfaces");

    HRESULT hr = DD_OK;
    if (FAILED(hr = this->DirectDraw->RestoreAllSurfaces())) {
        g_pSysCom->Log("Z:\\Engine\\Drawing\\_Wintel\\RenderD3D\\Source\\RenderWintelD3DInit.cpp", 545)
            ->LogMessage("RestoreAllSurfaces() failed");

        LogDirectDrawError(hr);
    }

    return DD_OK;
}

// 0x0fb87770
HRESULT ZRenderD3D::Blt(bool blt) {
    if (this->MainSurface == nullptr) {
        return ZERR_8200000F;
    }

    if (this->FullScreen && !blt) {
        while (this->BackSurface->GetFlipStatus(DDGFS_ISFLIPDONE) == DDERR_WASSTILLDRAWING) {}

        return this->MainSurface->Flip(NULL, this->StereoView ? (DDFLIP_STEREO | DDFLIP_WAIT) : DDFLIP_WAIT);
    }

    while (this->BackSurface->GetBltStatus(DDGBS_ISBLTDONE) == DDERR_WASSTILLDRAWING) {}

    return this->MainSurface->Blt(&this->WindowRectangle, this->BackSurface, NULL, DDBLT_WAIT, NULL);
}
