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

#include "Globals.hxx"
#include <System/ZExceptionRender.hxx>
#include <System/ZRender.hxx>

#include <stdio.h>
#include <windowsx.h>

#define ZRENDER_WINDOW_CLASS_NAME           "ZRenderWintelDEFAULT000"
#define ZRENDER_WINDOW_CLASS_NAME_LENGTH    25
#define ZRENDER_WINDOW_NAME_LENGTH          500

#define ZRENDER_CONFIGURATION_COUNT         6
#define ZRENDER_8BIT_INDEX_COUNT            12

static char g_ZRenderWindowClassName[ZRENDER_WINDOW_CLASS_NAME_LENGTH];                     // 0x0fbc5290
static char g_ZRenderWindowName[ZRENDER_WINDOW_NAME_LENGTH];                                // 0x0fbc529c

static f32 g_ZRenderFogOverride;                                                            // 0x0fbc5490
static ZRender* g_ZRender;                                                                  // 0x0fbc5494
static BOOL g_ZRenderRunning;                                                               // 0x0fbc5498

static BYTE PaletteColors3Bit[8] = { 0x0, 0x24, 0x49, 0x6D, 0x92, 0xB6, 0xDB, 0xFF };       // 0x0fbbd0c8
static BYTE PaletteColors2Bit[4] = { 0x0, 0x55, 0xAA, 0xFF };                               // 0x0fbbd0d0
static BYTE PaletteColors1Bit[2] = { 0x0, 0xFF };                                           // 0x0fbbd0d4
static u32 PaletteIndexes8Bit[ZRENDER_8BIT_INDEX_COUNT] = {
        0x3, 0x18, 0x1B, 0x40, 0x43, 0x58, 0xAD, 0xB5, 0xEC, 0xF7, 0xA4, 0x5B };            // 0x0fbbd0dc
static PALETTEENTRY PaletteColors8Bit[19] = {
        0x80,       0x8000,     0x8080,     0x800000,   0x800080,   0x808000,
        0xC0C0C0,   0xC0DCC0,   0xF0CAA6,   0xF0FBFF,   0xA4A0A0,   0x808080,
        0xFF,       0xFF00,     0xFFFF,     0xFF0000,   0xFF00FF,   0xFFFF00, 0xFFFFFF };   // 0x0fbbd110
static f32 g_ZRenderFogNear = 0.65f;                                                        // 0x0fbbd15c
static f32 g_ZRenderFogFar = 0.95f;                                                         // 0x0fbbd160

static ZRenderConfiguration g_ZRenderConfigurations[ZRENDER_CONFIGURATION_COUNT] = {
    { 640,  480,    32, FALSE },
    { 800,  600,    32, FALSE },
    { 1024, 768,    32, FALSE },
    { 1280, 1024,   32, FALSE },
    { 1600, 1200,   32, FALSE },
    { 0,    0,      0,  FALSE }
}; // 0x0fbbd178

static BYTE GetPaletteEntryColor(u32 index, BYTE bits, BYTE shift);

// 0x0fb81a00
void ZRender::Method0x158() { }

// 0x0fb81a00
void ZRender::Initialize() { }

// 0x0fba5cb0
void CheckRenderWindowClass(HINSTANCE instance) {
    WNDCLASSEXA wndc;
    ZeroMemory(&wndc, sizeof(WNDCLASSEXA));

    wndc.cbSize = sizeof(WNDCLASSEXA);

    if (!g_ZRenderRunning) {
        strcpy(g_ZRenderWindowClassName, ZRENDER_WINDOW_CLASS_NAME);
        g_ZRenderRunning = TRUE;

        for (u32 i = 0; i < 1000; i++) {
            sprintf(g_ZRenderWindowName, "%3d", i);
            if (!GetClassInfoExA(instance, g_ZRenderWindowClassName, &wndc)) {
                return;
            }
        }
    }
}

// 0x0fba5f00
// 0x0fbb5d70
ZRender::ZRender(HINSTANCE instance, HWND window) : ZRenderBase(instance, window) {
    if (!g_pSysInterface->Unk0x38F1) {
        this->ParentWindow = NULL;

        if (g_pSysInterface->ColorDepth == 0 || !g_pSysInterface->FullScreen) {

            HWND hwnd = GetDesktopWindow();
            HDC hdc = GetDC(hwnd);
            const s32 planes = GetDeviceCaps(hdc, PLANES);
            const s32 bits = GetDeviceCaps(hdc, BITSPIXEL);

            ReleaseDC(hwnd, hdc);

            g_pSysInterface->ColorDepth = planes * bits;

            if (g_pSysInterface->ColorDepth < 16) {
                throw ZExceptionRender("Fatal",
                    "This program requires that the display settings are set to high color or true color.");
            }

            if (g_pSysInterface->DebugVideo) {
                g_pSysCom->Log("Z:\\Engine\\Drawing\\_Wintel\\Source\\RenderWintel.cpp", 246)
                    ->LogMessage("Color depth %d", g_pSysInterface->ColorDepth);
            }
        }
    }

    this->Unk0x12C = true;
    this->Unk0x12B = false;
    this->Unk0x139 = 0; // TODO

    CheckRenderWindowClass(this->Instance);

    for (u32 i = 0; i < 512 /* TODO */; i++) {
        this->Unk0x14D[i] = new RefTab(32, 0);
    }

    this->FogOverrideCommand = new ZConsoleCommand("r_fog_near_far_override", &g_ZRenderFogOverride);
    g_pSysInterface->EnqueueConsoleCommand(this->FogOverrideCommand);

    this->FogNearCommand = new ZConsoleCommand("r_fog_near", &g_ZRenderFogNear);
    g_pSysInterface->EnqueueConsoleCommand(this->FogNearCommand);

    this->FogFarCommand = new ZConsoleCommand("r_fog_far", &g_ZRenderFogFar);
    g_pSysInterface->EnqueueConsoleCommand(this->FogFarCommand);
}

// 0x0fba6260
// 0x0fba6280
ZRender::~ZRender() {
    this->ClipCursorToWindow(false);

    if (g_pSysInterface->Unk0x38F1) {
        SetWindowLongA(this->Window, GWL_USERDATA, 0);
    }

    for (u32 i = 0; i < 512 /* TODO*/; i++) {
        delete this->Unk0x14D[i];
    }

    g_pSysInterface->DequeueConsoleCommand(this->FogOverrideCommand);
    if (this->FogOverrideCommand != nullptr) {
        delete this->FogOverrideCommand;
    }

    g_pSysInterface->DequeueConsoleCommand(this->FogNearCommand);
    if (this->FogOverrideCommand != nullptr) {
        delete this->FogNearCommand;
    }

    g_pSysInterface->DequeueConsoleCommand(this->FogFarCommand);
    if (this->FogFarCommand != nullptr) {
        delete this->FogFarCommand;
    }
}

// 0x0fba63a0
void ZRender::Method0x1B0() {
    // TODO NOT IMPLEMENTED
}

// 0x0fba6410
void ZRender::Method0x30(u32 todo1, bool todo2) {
    if (this->LockCount == 0) {
        this->Method0x10C();
    }
    else {
        this->Method0x1B0();
    }

    this->Unk0xDF = new ZUnk0xC();

    this->Method0x110();

    // TODO NOT IMPLEMENTED
}

// 0x0fba69c0
void ZRender::Method0x38() {
    if (this->LockCount < 1) {
        throw ZExceptionRender("Render", "Fatal Lock error in renderer");
    }

    this->Method0x1B0();
    this->Method0x120(0); // TODO

    this->Unk0xDF->FUN_0fba5e90(this);

    // TODO NOT IMPLEMENTED
}

// 0x0fba6a60
void ZRender::Method0x34(u32 todo1, u32 todo2) {
    // TODO NOT IMPLEMENTED
}

// 0x0fba6d00
void ZRender::HandleMiddleButtonDown(WPARAM wParam, s32 x, s32 y) {
    g_pSysInterface->Unk0x4 = false;

    this->HandleLeftButtonUp(wParam, x, y);

    *(undefined1*)(*(int*)g_pSysInterface_exref + 0xaf3) = 1;
    *(undefined1*)(*(int*)g_pSysInterface_exref + 0xaf2) = 1;
    *(undefined1*)(*(int*)g_pSysInterface_exref + 0xd1e) = 0;
    *(undefined1*)(*(int*)g_pSysInterface_exref + 0x2eb1) = 1;
    *(undefined1*)(*(int*)g_pSysInterface_exref + 0x2eb0) = 1;
    *(undefined1*)(*(int*)g_pSysInterface_exref + 0x2ec0) = 0;

    if (this->ParentWindow != NULL) {
        this->FUN_0fba8f40();

        SetCursor(LoadCursorA(this->Instance, (LPCSTR)0x196)); // TODO

        this->Unk0x20 = this->Unk0x20 | 0x00800000; // TODO

        this->Method0x9C();
        this->Method0x98();

        this->Unk0x56 = g_pSysInterface->Unk0x59->Unk0x1C1->Method0x18();
        this->Unk0x20 = this->Unk0x20 & 0xFF7FFFFF; // TODO
    }
}

// 0x0fba6df0
void ZRender::HandleMiddleButtonUp(WPARAM wParam, s32 x, s32 y) {
    g_pSysInterface->Unk0x4 = false;

    this->HandleLeftButtonUp(wParam, x, y);

    *(undefined1*)(*(int*)g_pSysInterface_exref + 0xaf3) = 0;
    *(undefined1*)(*(int*)g_pSysInterface_exref + 0xaf2) = 0;
    *(undefined1*)(*(int*)g_pSysInterface_exref + 0x2eb1) = 0;
    *(undefined1*)(*(int*)g_pSysInterface_exref + 0x2eb0) = 0;

    if (this->ParentWindow != 0) {
        this->FUN_0fba8fd0();

        this->Unk0x20 = this->Unk0x20 | 0x02000000;

        this->Method0x9C();
        this->Method0x98();

        this->Unk0x20 = this->Unk0x20 & 0xFDFFFFFF;
    }

    this->Unk0x56 = false;
}

// 0x0fba7250
void ZRender::HandleKeyDown(WPARAM wParam, LPARAM lParam) {
    g_pSysInterface->Unk0x4 = false;

    if (wParam == VK_DELETE && g_pSysInterface->Unk0x38F1) {
        g_pSysInterface->Unk0x59->Method0x16C();
    }

    if (this->ParentWindow != NULL) {
        void* todo_a = g_pSysInterface->Method0xEC(); // TODO

        if (todo_a != nullptr) {
            if (((lParam & 0xff0000) == 0x290000) && ((lParam & 0x40000000) == 0)) { // TODO
                this->Unk0x80 = true;
                return;
            }

            if (todo_a->Method0xC()) {
                todo_a->Method0x10(wParam, lParam);
                return;
            }
        }

        if (((int)wParam < 0x100) && ((wParam & 0xff) != 1)) { // TODO
            iVar3 = (wParam & 0xff) * 0xf;
            *(undefined1*)(iVar3 + 0xab7 + *(int*)g_pSysInterface_exref) = 1;
            iVar4 = ((int)lParam >> 0x10 & 0x1ffU) * 0xf;
            *(undefined1*)(iVar4 + 0x19b7 + *(int*)g_pSysInterface_exref) = 1;
            if ((lParam & 0x40000000) == 0) {
                *(undefined1*)(iVar3 + 0xab6 + *(int*)g_pSysInterface_exref) = 1;
                *(undefined1*)(iVar4 + 0x19b6 + *(int*)g_pSysInterface_exref) = 1;
                iVar2 = *(int*)g_pSysInterface_exref;
                if (0.25 <= *(double*)(iVar2 + 0x37c5) - *(double*)(iVar3 + 0xab8 + iVar2)) {
                    *(undefined1*)(iVar3 + 0xab5 + iVar2) = 0;
                    *(undefined1*)(iVar4 + 0x19b5 + *(int*)g_pSysInterface_exref) = 0;
                }
                else {
                    *(undefined1*)(iVar3 + 0xab5 + iVar2) = 1;
                    *(undefined1*)(iVar4 + 0x19b5 + *(int*)g_pSysInterface_exref) = 1;
                }
                iVar2 = *(int*)g_pSysInterface_exref;
                *(undefined4*)(iVar3 + 0xab8 + iVar2) = *(undefined4*)(iVar2 + 0x37c5);
                *(undefined4*)(iVar3 + 0xabc + iVar2) = *(undefined4*)(iVar2 + 0x37c9);
                iVar3 = *(int*)g_pSysInterface_exref;
                *(undefined4*)(iVar4 + 0x19b8 + iVar3) = *(undefined4*)(iVar3 + 0x37c5);
                *(undefined4*)(iVar4 + 0x19bc + iVar3) = *(undefined4*)(iVar3 + 0x37c9);
            }
        }
    }
}

// 0x0fba77b0
void ZRender::HandleSetFocus() {
    FUN_0fba77a0();

    SetFocus(this->Window);

    if (!g_pSysInterface->Unk0x38F1) {
        this->ClipCursorToWindow(true);
        ShowCursor(FALSE);
    }
}

// 0x0fba77f0
void ZRender::HandleKillFocus() {
    if (!g_pSysInterface->Unk0x38F1) {
        this->ClipCursorToWindow(false);
        ShowCursor(TRUE);
    }
}

// 0x0fba7820
void ZRender::HandleMouseWheel(s16 value) {
    g_pSysInterface->MouseWheel += value;

    if (this->ParentWindow != NULL) {
        this->FUN_0fba8f40();
        this->FUN_0fba8fd0();

        this->Method0x9C();
        this->Method0x98();
    }
}

// 0x0fba7870
bool ZRender::HandleSetCursor() {
    if (!g_pSysInterface->Unk0x38F1) {
        return true;
    }

    void* todo_a = FUN_0fba79b0(this->Method0x7C(0)); // TODO

    // TODO NOT IMPLEMENTED


    return true;
}

// 0x0fba6e90
void ZRender::HandleLeftButtonDown(WPARAM wParam, s32 x, s32 y) {
    g_pSysInterface->Unk0x4 = false;

    this->HandleLeftButtonUp(wParam, x, y);

    *(undefined1*)(*(int*)g_pSysInterface_exref + 0xac6) = 1;
    *(undefined1*)(*(int*)g_pSysInterface_exref + 0xd1e) = 0;
    *(undefined1*)(*(int*)g_pSysInterface_exref + 0xac5) = 1;
    *(undefined1*)(*(int*)g_pSysInterface_exref + 0x2e93) = 1;
    *(undefined1*)(*(int*)g_pSysInterface_exref + 0x2ec0) = 0;
    *(undefined1*)(*(int*)g_pSysInterface_exref + 0x2e92) = 1;

    if (this->ParentWindow != NULL) {
        this->FUN_0fba8f40();
        this->Method0x9C();
        this->Method0x98();

        this->Unk0x56 = g_pSysInterface->Unk0x59->Unk0x1C1->Method0x18();
    }
}

// 0x0fba6f50
void ZRender::HandleLeftButtonUp(WPARAM wParam, s32 x, s32 y) {
    g_pSysInterface->Unk0x4 = false;

    this->HandleLeftButtonUp(wParam, x, y);

    *(undefined1*)(*(int*)g_pSysInterface_exref + 0xac6) = 0;
    *(undefined1*)(*(int*)g_pSysInterface_exref + 0xac5) = 0;
    *(undefined1*)(*(int*)g_pSysInterface_exref + 0x2e93) = 0;
    *(undefined1*)(*(int*)g_pSysInterface_exref + 0x2e92) = 0;

    if (this->ParentWindow != NULL) {
        this->FUN_0fba8fd0();
        this->Method0x9C();
        this->Method0x98();
    }

    *(undefined1*)(*(int*)g_pSysInterface_exref + 0xd1e) = 0;
    *(undefined1*)(*(int*)g_pSysInterface_exref + 0x2ec0) = 0;

    this->Unk0x56 = false;
}

// 0x0fba7000
void ZRender::HandleLeftButtonDoubleClick(WPARAM wParam, s32 x, s32 y) {
    g_pSysInterface->Unk0x4 = false;

    this->HandleLeftButtonUp(wParam, x, y);

    *(undefined1*)(*(int*)g_pSysInterface_exref + 0xac6) = 1;
    *(undefined1*)(*(int*)g_pSysInterface_exref + 0xd1e) = 1;
    *(undefined1*)(*(int*)g_pSysInterface_exref + 0x2e93) = 1;
    *(undefined1*)(*(int*)g_pSysInterface_exref + 0x2ec0) = 1;

    if (this->ParentWindow != NULL) {
        this->FUN_0fba8f40();
        this->Method0x9C();
        this->Method0x98();

        this->Unk0x56 = g_pSysInterface->Unk0x59->Unk0x1C1->Method0x18();
    }
}

// 0x0fba70a0
void ZRender::HandleRightButtonDown(WPARAM wParam, s32 x, s32 y) {
    g_pSysInterface->Unk0x4 = false;

    this->HandleLeftButtonUp(wParam, x, y);

    if (g_pSysInterface->Unk0x38F1) {
        this->FUN_0fba8f40();
    }

    *(undefined1*)(*(int*)g_pSysInterface_exref + 0xad5) = 1;
    *(undefined1*)(*(int*)g_pSysInterface_exref + 0xd1e) = 0;
    *(undefined1*)(*(int*)g_pSysInterface_exref + 0xad4) = 1;
    *(undefined1*)(*(int*)g_pSysInterface_exref + 0x2ea2) = 1;
    *(undefined1*)(*(int*)g_pSysInterface_exref + 0x2ec0) = 0;
    *(undefined1*)(*(int*)g_pSysInterface_exref + 0x2ea1) = 1;

    if (0.25f <= g_pSysInterface->Unk0x37C5 - g_pSysInterface->Unk0xAD6) {
        g_pSysInterface->Unk0xAD3 = false;
        g_pSysInterface->Unk0x2EA0 = false;
    }
    else {
        g_pSysInterface->Unk0xAD3 = true;
        g_pSysInterface->Unk0x2EA0 = true;
    }

    g_pSysInterface->Unk0xAD6 = g_pSysInterface->Unk0x37C5;
    g_pSysInterface->Unk0xADA = g_pSysInterface->Unk0x37C9;
    g_pSysInterface->Unk0x2EA3 = g_pSysInterface->Unk0x37C5;
    g_pSysInterface->Unk0x2EA7 = g_pSysInterface->Unk0x37C9;

    if (this->ParentWindow != NULL) {
        this->FUN_0fba8f40();
        this->Method0x9C();
        this->Method0x98();

        this->Unk0x56 = g_pSysInterface->Unk0x59->Unk0x1C1->Method0x18();
    }
}

// 0x0fba7200
void ZRender::HandleRightButtonUp(WPARAM wParam, s32 x, s32 y) {
    g_pSysInterface->Unk0x4 = false;

    if (g_pSysInterface->Unk0x38F1) {
        this->FUN_0fba8fd0();
    }

    g_pSysInterface->Unk0xAD5 = false;
    g_pSysInterface->Unk0x2EA2 = fale;
}

// 0x0fba7400
void ZRender::HandleKeyUp(WPARAM wParam, LPARAM lParam) {
    g_pSysInterface->Unk0x4 = false;

    void* todo_a = g_pSysInterface->Method0xEC(); // TODO

    if (todo_a != nullptr) {
        if (todo_a->Method0xC()) {
            thodo_a->Method0x14(wParam, lParam);
        }
    }

    if (wParam < 0x100) { // TODO
        *(undefined1*)((wParam & 0xff) * 0xf + 0xab7 + *(int*)g_pSysInterface_exref) = 0;
        *(undefined1*)((lParam >> 0x10 & 0x1ffU) * 0xf + 0x19b7 + *(int*)g_pSysInterface_exref) = 0;
    }
}

// 0x0fba7490
void ZRender::HandleMouseMove(WPARAM wParam, s32 x, s32 y) {
    g_pSysInterface->Unk0x4 = false;

    this->PreviousMouseX = this->MouseX;
    this->PreviousMouseY = this->MouseY;

    const f32 todo_a = (f32)this->Method0xA0(); // TODO
    const f32 todo_b = (f32)this->Method0xA4(); // TODO

    POINT point;
    point.x = x;
    point.y = y;

    ClientToScreen(this->Window, &point);

    this->MouseX = point.x;
    this->MouseY = point.y;

    if (g_pSysInterface->Unk0x38F1) {
        if (!this->Unk0x5B) {
            this->Unk0x3E = x / todo_a - 0.5f;
            this->Unk0x42 = y / todo_b - 0.5f;

            if (!this->Unk0x56) {
                return;
            }

            this->Method0x9C();
            this->Method0x98();

            return;
        }

        const s32 width = GetSystemMetrics(SM_CXSCREEN);
        const s32 height = GetSystemMetrics(SM_CYSCREEN);

        const s32 mx = this->MouseX;
        const s32 pmx = this->PreviousMouseX;

        const s32 my = this->MouseY;
        const s32 pmy = this->PreviousMouseY;
        bool set = false;

        if (mx == 0 && 0 < pmx) {
            this->MouseX = width - 11;
            set = true;
        }
        else if (mx == (width - 1) && pmx < (width - 1)) {
            this->MouseX = 10;
            set = true;
        }

        if (my == 0 && 0 < pmy) {
            this->MouseY = height - 11;
            set = true;
        }

        else if (my == (height - 1) && pmy < (height - 1)) {
            this->MouseY = 10;
            set = true;
        }

        this->Unk0x3E = (f32)(mx - pmx) / todo_a + this->Unk0x3E;
        this->Unk0x42 = (f32)(my - pmy) / todo_b + this->Unk0x42;

        if (set) {
            SetCursorPos(this->MouseX, this->MouseY);
        }

        if (!this->Unk0x56) {
            return;
        }

        this->Method0x9C();
        this->Method0x98();
    }
    else {
        if (!this->Unk0x12B) {
            return;
        }

        RECT rect;
        GetClientRect(this->Window, &rect);
        ClientToScreen(this->Window, (LPPOINT)&rect);

        const s32 dx = x - (s32)(todo_a * 0.5f);
        const s32 dy = y - (s32)(todo_b * 0.5f);

        if (dx == 0 && dy == 0) {
            return;
        }

        if (!this->GetShowCursorVisibility()) {
            g_pSysInterface->Unk0xA95 += dx;
            g_pSysInterface->Unk0xA99 += dy;
        }

        SetCursorPos(rect.left + (s32)(todo_a * 0.5f), rect.top + (s32)(todo_b * 0.5f));

        this->Unk0x3E += (f32)dx / todo_a;
        this->Unk0x42 += (f32)dy / todo_b;

        if (0.5f < todo_a) {
            this->Unk0x3E = 0.5f;
        }

        if (0.5f < todo_b) {
            this->Unk0x42 = 0.5f;
        }

        if (this->Unk0x3E < -0.5f) {
            this->Unk0x3E = -0.5f;
        }

        if (this->Unk0x42 < -0.5f) {
            this->Unk0x42 = -0.5f;
        }
    }
}

// 0x0fba79e0
static LRESULT WINAPI ZRenderWindowMessageHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    ZRender* render = (ZRender*)GetWindowLongA(hwnd, GWL_USERDATA);

    if (render == nullptr) {
        render = g_ZRender;
        if (g_ZRender == nullptr) {
            return;
        }
    }

    render->Window = hwnd;

    return render->HandleRenderWindowMessages(uMsg, wParam, lParam);
}

// 0x0fba7a20
LRESULT ZRender::HandleRenderWindowMessages(UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CREATE: {
        this->InitializeRenderWindow(lParam);
        break;
    }
    case WM_DESTROY: {
        if (g_pSysInterface->Unk0x38F1) {
            this->ReleaseRenderWindow();
            return 0;
        }

        break;
    }
    case WM_SIZE: {
        this->HandleSize(LOWORD(lParam), HIWORD(lParam));
        break;
    }
    case WM_SETFOCUS: {
        this->HandleSetFocus();
        break;
    }
    case WM_KILLFOCUS: {
        this->HandleKillFocus();
        break;
    }
    case WM_PAINT: {
        this->HandleUpdate();
        break;
    }
    case WM_CLOSE: {
        if (!g_pSysInterface->Unk0x38F1) {
            this->ReleaseRenderWindow();
            return 0;
        }
    }
    case WM_ACTIVATEAPP: {
        if (wParam) {
            this->HandleSetFocus();
        }
        else {
            this->HandleKillFocus();
        }

        break;
    }
    case WM_SETCURSOR: {
        if (this->HandleSetCursor()) {
            return TRUE;
        }

        break;
    }
    case WM_WINDOWPOSCHANGED: {
        if ((g_pSysInterface == nullptr || !g_pSysInterface->Unk0x38EF) && g_pSysInterface->Unk0x38EF) {
            this->Method0x9C();
            this->Method0x98();
        }

        break;
    }
    case WM_NCMOUSEMOVE:
    case WM_MOUSEMOVE: {
        if (!this->Unk0x81) {
            this->HandleLeftButtonUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        }

        break;
    }
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN: {
        if (!this->Unk0x81) {
            this->HandleKeyDown(wParam, lParam);
        }

        break;
    }
    case WM_KEYUP:
    case WM_SYSKEYUP: {
        if (!this->Unk0x81) {
            this->HandleKeyUp(wParam, lParam);
        }

        break;
    }
    case WM_SYSCOMMAND: {
        if (wParam != SC_MAXIMIZE && wParam != SC_SIZE && wParam != SC_MOVE) {
            break;
        }
        else if (wParam != SC_SCREENSAVE && wParam != SC_MONITORPOWER) {
            break;
        }

        if (g_pSysInterface->FullScreen) {
            return 0;
        }

        break;
    }
    case WM_LBUTTONDOWN: {
        if (!this->Unk0x81) {
            this->HandleLeftButtonDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        }

        break;
    }
    case WM_LBUTTONUP: {
        if (!this->Unk0x81) {
            this->HandleLeftButtonUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        }

        break;
    }
    case WM_LBUTTONDBLCLK: {
        if (!this->Unk0x81) {
            this->HandleLeftButtonDoubleClick(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            return DefWindowProcA(this->Window, WM_LBUTTONDBLCLK, wParam, lParam);
        }

        break;
    }
    case WM_RBUTTONDOWN:
    case WM_RBUTTONDBLCLK: {
        if (!this->Unk0x81) {
            this->HandleRightButtonDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        }

        break;
    }
    case WM_RBUTTONUP: {
        if (!this->Unk0x81) {
            this->HandleRightButtonUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        }

        break;
    }
    case WM_MBUTTONDOWN: {
        if (!this->Unk0x81) {
            this->HandleMiddleButtonDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        }

        break;
    }
    case WM_MBUTTONUP: {
        if (!this->Unk0x81) {
            this->HandleMiddleButtonUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        }

        break;
    }
    case WM_MOUSEWHEEL: {
        if (!this->Unk0x81) {
            this->HandleMouseWheel(GET_WHEEL_DELTA_WPARAM(wParam));
        }

        break;
    }
    case WM_POWERBROADCAST: {
        return BROADCAST_QUERY_DENY;
    }
    case 0x7E3: { // TODO
        if (g_pSysInterface == nullptr || !g_pSysInterface->Unk0x38EF) {
            this->Method0x9C();
            this->Method0x98();
            this->HandleUpdate();
        }

        return 0;
    }
    case 0x7EE: { // TODO
        this->Unk0x81 = wParam != 0; // TODO
        break;
    }
    case 0x7EF: { // TODO
        this->Method0x150((const char*)wParam);

        return DefWindowProcA(this->Window, 0x7EF /* TODO */, wParam, lParam);
    }
    }

    return DefWindowProcA(this->Window, uMsg, wParam, lParam);
}

// 0x0fba8250
void ZRender::Method0x154(u32 todo) {
    this->Method0x9C();
    this->Method0x98();
    this->Method0x64(todo);
}

// 0x0fba8280
void ZRender::Method0x150(const char* path) {
    const f32 debug = g_pSysInterface->DebugOptionsVisibility;
    g_pSysInterface->DebugOptionsVisibility = 0.0f;

    ZString name = path;

    char* ext = strrchr(name, '.');
    if (ext != nullptr) {
        ext[0] = NULL;

        // TODO NOT IMPLEMENTED
    }

    g_pSysInterface->DebugOptionsVisibility = debug;
}

// 0x0fba8dd0
bool ZRender::CreateRenderWindow(const char* cls, const char* name, DWORD style, DWORD estyle, LPRECT size, HWND window) {
    RECT rect;
    rect.right = size->right;
    rect.left = size->left;
    rect.top = size->top;
    rect.bottom = size->bottom;

    AdjustWindowRectEx(&rect, style, NULL, estyle);

    if (cls == nullptr) {
        WNDCLASSEXA wndc;
        if (!GetClassInfoExA(this->Instance, g_ZRenderWindowClassName, &wndc)) {
            wndc.cbSize = sizeof(WNDCLASSEXA);
            wndc.style = CS_OWNDC | CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
            wndc.lpfnWndProc = ZRenderWindowMessageHandler;
            wndc.cbClsExtra = 0;
            wndc.cbWndExtra = 0;
            wndc.hInstance = this->Instance;
            wndc.hIcon = LoadIconA(NULL, IDI_APPLICATION);
            wndc.hCursor = LoadCursorA(NULL, IDI_APPLICATION);
            wndc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
            wndc.lpszMenuName = nullptr;
            wndc.lpszClassName = g_ZRenderWindowClassName;
            wndc.hIconSm = NULL;

            if (RegisterClassExA(&wndc) == 0) {
                return false;
            }
        }

        cls = g_ZRenderWindowClassName;
    }

    g_ZRender = this;

    CreateWindowExA(estyle, cls, name, style,
        rect.left, rect.top, rect.right - rect.left,
        rect.bottom - rect.top, window, NULL, this->Instance, nullptr);
    SetWindowLongA(this->Window, GWL_USERDATA, (LONG_PTR)this);

    g_ZRender = nullptr;

    return true;
}

// 0x0fba8f20
void ZRender::ShowRenderWindow(int cmd) {
    ShowWindow(this->Window, cmd);
}

// 0x0fba8fe0
void ZRender::InitializeRenderWindow(LPARAM) {
    this->Initialize();
    g_pSysInterface->Method0x9C();
}

// 0x0fba9000
void ZRender::ReleaseRenderWindow() {
    this->HandleKillFocus();

    if (!g_pSysInterface->Unk0x38F1) {
        if (!g_pSysInterface->Unk0x38ED) {
            g_pSysInterface->Unk0x6 = true;
        }
    }
    else if (this != nullptr) {
        delete this;
    }
}

// 0x0fba9040
void ZRender::GetRenderWindowDimensions(LPRECT rect) {
    RECT dims;

    if (this->ParentWindow == NULL) {
        GetWindowRect(this->Window, &dims);
    }
    else {
        GetClientRect(this->Window, &dims);
    }

    rect->left = dims.left;
    rect->top = dims.top;
    rect->right = dims.right;
    rect->bottom = dims.bottom;
}

// 0x0fba9090
void ZRender::ClipCursorToWindow(bool value) {
    this->Unk0x12B = value;

    if (value) {
        RECT rect;
        
        GetClientRect(this->Window, &rect);

        ClientToScreen(this->Window, (LPPOINT)&rect);
        ClientToScreen(this->Window, (LPPOINT)&rect.right);

        ClipCursor(&rect);
    }
    else {
        ClipCursor(nullptr);
    }
}

// 0x0fba9100
bool ZRender::Method0x5C() {
    return this->Unk0x12B;
}

// 0x0fba9110
void ZRender::Method0xF8(u32 todo1, u32 todo2, u32 todo3) {
    // TODO NOT IMPLEMENTED

    SendMessageA(GetParent(GetParent(this->ParentWindow)), 0x7E4 /* TODO */, wParam, lParam);
}

// 0x0fba9180
static BYTE GetPaletteEntryColor(u32 index, BYTE bits, BYTE shift) {
    u32 indx = index >> (shift & 0x1F);

    if (bits == 1) {
        return PaletteColors1Bit[indx & 1];
    }
    else if (bits == 2) {
        return PaletteColors2Bit[indx & 3];
    }
    else if (bits == 3) {
        return PaletteColors3Bit[indx & 7];
    }

    return 0;
}

// 0x0fba91c0
void ZRender::CreateRenderPalette(HDC hdc) {
    const s32 format = GetPixelFormat(hdc);

    PIXELFORMATDESCRIPTOR desc;
    DescribePixelFormat(hdc, format, sizeof(PIXELFORMATDESCRIPTOR), &desc);

    if (desc.dwFlags & PFD_NEED_PALETTE) {
        const u32 entries = 1 << (desc.cColorBits & 0x1F);
        PLOGPALETTE palette =
            (PLOGPALETTE)new u8[entries * sizeof(PALETTEENTRY) + 2 * sizeof(u32)];
        palette->palVersion = 0x300;
        palette->palNumEntries = (WORD)entries;

        for (u32 i = 0; i < entries; i++) {
            palette->palPalEntry[i].peRed =
                GetPaletteEntryColor(i, desc.cRedBits & 0xFF, desc.cRedShift);
            palette->palPalEntry[i].peGreen =
                GetPaletteEntryColor(i, desc.cGreenBits & 0xFF, desc.cGreenShift);
            palette->palPalEntry[i].peBlue =
                GetPaletteEntryColor(i, desc.cBlueBits & 0xFF, desc.cBlueShift);
            palette->palPalEntry[i].peFlags = 0;
        }

        if (desc.cColorBits == 8 && desc.cRedBits == 3 && desc.cRedShift == 0
            && desc.cGreenBits == 3 && desc.cGreenShift == 3
            && desc.cBlueBits == 2 && desc.cBlueShift == 6) {
            for (u32 i = 0; i < ZRENDER_8BIT_INDEX_COUNT; i++) {
                palette->palPalEntry[PaletteIndexes8Bit[i]] = PaletteColors8Bit[i];
            }
        }

        this->Palette = CreatePalette(palette);

        delete[] palette;

        this->ActivePalette = SelectPalette(hdc, this->Palette, 0);
        RealizePalette(hdc);
    }
}

// 0x0fba96a0
void ZRender::SetModule(const char* path) {
    if (strcmpi(g_pSysInterface->DrawDll, path) != 0) {
        ZString module = ZString(path);

        if (g_pSysInterface->DrawDll != module) {
            g_pSysInterface->DrawDll = module;
        }

        g_pSysInterface->PreviousWindowWidth = g_pSysInterface->WindowWidth;
        g_pSysInterface->PreviousWindowHeight = g_pSysInterface->WindowHeight;
        g_pSysInterface->Unk0x38ED = true;
    }
}

// 0x0fba98d0
void ZRender::SetConfiguration(ZRenderConfiguration* config) {
    if (g_pSysInterface->WindowWidth == config->Width) {
        g_pSysInterface->PreviousWindowWidth = g_pSysInterface->WindowWidth;
    }
    else {
        g_pSysInterface->PreviousWindowWidth = config->Width;
        g_pSysInterface->Unk0x38ED = true;
    }

    if (g_pSysInterface->WindowHeight == config->Height) {
        g_pSysInterface->PreviousWindowHeight = g_pSysInterface->WindowWidth;
    }
    else {
        g_pSysInterface->PreviousWindowHeight = config->Height;
        g_pSysInterface->Unk0x38ED = true;
    }

    if (g_pSysInterface->ColorDepth == config->ColorDepth) {
        g_pSysInterface->PreviousColorDepth = g_pSysInterface->ColorDepth;
    }
    else {
        g_pSysInterface->PreviousColorDepth = config->ColorDepth;
        g_pSysInterface->Unk0x38ED = true;
    }

    if (g_pSysInterface->ColorDepth == config->ColorDepth) {
        g_pSysInterface->PreviousColorDepth = g_pSysInterface->ColorDepth;
    }
    else {
        g_pSysInterface->PreviousColorDepth = config->ColorDepth;
        g_pSysInterface->Unk0x38ED = true;
    }

    if (g_pSysInterface->FullScreen == config->FullScreen) {
        g_pSysInterface->PreviousFullScreen = g_pSysInterface->FullScreen;
    }
    else {
        g_pSysInterface->PreviousFullScreen = config->FullScreen;
        g_pSysInterface->Unk0x38ED = true;
    }
}

// 0x0fba98c0
const ZRenderConfiguration* ZRender::GetSupportedConfigurations() {
    return g_ZRenderConfigurations;
}
