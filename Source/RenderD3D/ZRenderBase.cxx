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

#include <System/ZRender.hxx>

static f32 g_ZRenderBaseGamma = 1.0f;                       // 0x0fbbcd50

static f32 g_ZRenderBaseTextureStat;                        // 0x0fbc5278
static f32 g_ZRenderBaseShowDrawModes;                      // 0x0fbc527c

static f32 g_ZRenderBaseFog = 1.0f;                         // 0x0fbbd170

// 0x0fb819d0
void ZRenderBase::SetModule(const char*) {}

// 0x0fb819d0
void ZRenderBase::Method0x8(u32) {}

// 0x0fb819d0
void ZRenderBase::SetConfiguration(ZRenderConfiguration*) {}

// 0x0fb819d0
void ZRenderBase::ClipCursorToWindow(bool) {}

// 0x0fb819d0
void ZRenderBase::GetCaps(u32*) {}

// 0x0fb819d0
void ZRenderBase::Method0x60(u32) {}

// 0x0fb819d0
void ZRenderBase::Method0x64(u32) {}

// 0x0fb819d0
void ZRenderBase::Method0x120(u32) {}

// 0x0fb819d0
void ZRenderBase::Method0x124(u32) {}

// 0x0fb819d0
void ZRenderBase::Method0x134(u32) {}

// 0x0fb819d0
void ZRenderBase::Method0x13C(u32) {}

// 0x0fb819e0
bool ZRenderBase::BeginScene() {
    return true;
}

// 0x0fb819e0
bool ZRenderBase::EndScene() {
    return true;
}

// 0x0fb819f0
bool ZRenderBase::Method0x3C() {
    return false;
}

// 0x0fb819f0
bool ZRenderBase::Method0x5C() {
    return false;
}

// 0x0fb819f0
bool ZRenderBase::Method0xC4() {
    return false;
}

// 0x0fb81a00
void ZRenderBase::Method0x40() {}

// 0x0fb81a00
void ZRenderBase::Method0x44() {}

// 0x0fb81a00
void ZRenderBase::Method0x48() {}

// 0x0fb81a00
void ZRenderBase::Method0x54() {}

// 0x0fb81a00
void ZRenderBase::Method0xD4() {}

// 0x0fb81a00
void ZRenderBase::Method0xFC() {}

// 0x0fb81a00
void ZRenderBase::ApplyCurrentState() {}

// 0x0fb81a00
void ZRenderBase::InitializeCurrentState() {}

// 0x0fb81a00
void ZRenderBase::Method0x110() {}

// 0x0fb81a00
void ZRenderBase::Method0x114() {}

// 0x0fb81a00
void ZRenderBase::Method0x138() {}

// 0x0fb81a10
void ZRenderBase::Method0x4C(u32, u32, u32) {}

// 0x0fb81a10
void ZRenderBase::Method0x50(u32, u32, u32) {}

// 0x0fb81a10
void ZRenderBase::Method0xC8(u32, u32, u32) {}

// 0x0fb81a10
void ZRenderBase::Method0x11C(u32, u32, u32) {}

// 0x0fb81a10
void ZRenderBase::Method0x140(u32, u32, u32) {}

// 0x0fb84b30
f32 ZRenderBase::Method0xB4() {
    return this->Unk0x123;
}

// 0x0fb84b40
f32 ZRenderBase::Method0xB8() {
    return this->Unk0x127;
}

// 0x0fb84b50
f32 ZRenderBase::Method0xC0() {
    return this->Unk0xF3;
}

// 0x0fb84b60
ZCounter* ZRenderBase::GetCounter() {
    return this->Counter;
}

// 0x0fb84b70
ZRenderBase* ZRenderBase::GetRender() {
    return this->Render;
}

// 0x0fb85600
u32 ZRenderBase::GetMaximumTextures() {
    return this->MaximumTextures;
}

// 0x0fb85610
void ZRenderBase::SetMaximumTextures(u32 count) {
    this->MaximumTextures = count;
}

// 0x0fb85620
void ZRenderBase::Method0xD4() {
    // TODO NOT IMPLEMENTED
}

// 0x0fbac830
void ZRenderBase::SetFeature(u32, u32) {}

// 0x0fbac830
void ZRenderBase::Method0x1C(u32, u32) {}

// 0x0fbac830
void ZRenderBase::ClearSurface(u32, u32) {}

// 0x0fbac830
void ZRenderBase::Method0x12C(u32, u32) {}

// 0x0fbad210
void ZRenderBase::SetViewport(f32, f32, f32, f32) {}

// 0x0fba2cc0
// 0x0fbb5acc
ZRenderBase::ZRenderBase(HINSTANCE instance, HWND window) {
    this->Initialized = false;
    this->Unk0x3E = 0.0f;
    this->Unk0x42 = 0.0f;
    this->MouseX = 0;
    this->MouseY = 0;
    this->PreviousMouseX = 0;
    this->PreviousMouseY = 0;

    this->Window = NULL;
    this->ParentWindow = window;
    this->Instance = instance;

    this->ShowCursorVisibility = 0.0f;
    this->Unk0x56 = false;
    this->Unk0x60 = 10;
    this->Unk0x64 = 10;
    this->Unk0x5C = 0;
    this->DisplayFrequency = 60;
    this->Unk0x81 = false;
    this->Unk0x82 = true;
    this->Unk0x5B = false;
    this->NativeDrawVisibility = 0.0f;
    this->Unk0x80 = 0;
    this->Unk0xDF = 0;
    this->LockCount = 0;
    this->Unk0x20 = 0;
    this->Unk0x68 = 0;
    this->Unk0x6C = 0;
    this->Unk0x24 = 0;
    this->Unk0x78 = 0;
    this->Unk0x2D = 0;
    this->Unk0x31 = 0;
    this->Unk0x35 = 0;
    this->Unk0x39 = 0;
    this->Unk0x93 = 0;
    this->Unk0x9F = 0;

    g_ZRenderBaseGamma = 1.0f;

    this->DisableTripleBuffering = false;
    this->MaximumTextures = 0;

    if (!g_pSysInterface->EnableTripleBuffering) {
        this->DisableTripleBuffering = true;
    }

    this->Unk0xF3 = 1.0f;
    this->Unk0x123 = 5.0f;
    this->Unk0x127 = 6.0f;

    if (!g_pSysInterface->Unk0x38F1) {
        this->Unk0x82 = false;
    }

    this->Unk0x8B = new LinkSortRefTab(8, 0);
    this->Render = this;
    this->Counter = new ZCounter();

    this->Previous = nullptr;
    this->Current = g_pSysInterface->Render;

    if (g_pSysInterface->Render != nullptr) {
        g_pSysInterface->Render->Current = this;
    }

    g_pSysInterface->Render = this;
    this->Unk0x4 = 0;

    if (!g_pSysInterface->Unk0x38F1) {
        this->DebugCommand = new ZConsoleCommand("ip_debug", &g_pSysInterface->DebugOptionsVisibility);
        g_pSysInterface->EnqueueConsoleCommand(this->DebugCommand);

        this->TimersCommand = new ZConsoleCommand("ip_timers", &g_pSysInterface->TimersVisibility);
        g_pSysInterface->EnqueueConsoleCommand(this->TimersCommand);

        this->TimeMultiplierCommand = new ZConsoleCommand("ip_timemultiplier", &g_pSysInterface->TimeMultiplierVisibility);
        g_pSysInterface->EnqueueConsoleCommand(this->TimeMultiplierCommand);

        this->FogCommand = new ZConsoleCommand("r_fog", &g_ZRenderBaseFog);
        g_pSysInterface->EnqueueConsoleCommand(this->FogCommand);

        this->NativeDrawCommand = new ZConsoleCommand("r_native_draw", &this->NativeDrawVisibility);
        g_pSysInterface->EnqueueConsoleCommand(this->NativeDrawCommand);

        this->ShowCursorCommand = new ZConsoleCommand("r_show_cursor", &this->ShowCursorVisibility);
        g_pSysInterface->EnqueueConsoleCommand(this->ShowCursorCommand);

        this->GammaCommand = new ZConsoleCommand("r_gamma", &g_ZRenderBaseGamma);
        g_pSysInterface->EnqueueConsoleCommand(this->GammaCommand);

        this->TextureStatCommand = new ZConsoleCommand("r_texturestat", &g_ZRenderBaseTextureStat);
        g_pSysInterface->EnqueueConsoleCommand(this->TextureStatCommand);

        this->FontSizeCommand = new ZRenderConsoleCommand("r_font_size", this);
        g_pSysInterface->EnqueueConsoleCommand(this->FontSizeCommand);

        this->AntialiasCommand = new ZRenderConsoleCommand("r_antialias", this);
        g_pSysInterface->EnqueueConsoleCommand(this->AntialiasCommand);

        this->ShowDrawModesCommand = new ZConsoleCommand("r_show_draw_modes", &g_ZRenderBaseShowDrawModes);
        g_pSysInterface->EnqueueConsoleCommand(this->ShowDrawModesCommand);
    }

    this->Unk0xE7 = new RefTab(32, 0);
    this->Unk0xEB = new RefTab(32, 0);
    this->Unk0xE3 = new RefTab(32, 0);
    this->Unk0xEF = new RefTab(32, 0);
}

// 0x0fba3a10
bool ZRenderBase::SetFog(u32 value) {
    bool result = g_ZRenderBaseFog != 0.0f;
    g_ZRenderBaseFog = (f32)value;
    return result;
}

// 0x0fba3a50
void ZRenderBase::Method0xD0() {
    if (g_pSysInterface->DebugOptionsVisibility != 0.0f) {
        if (g_ZRenderBaseShowDrawModes != 0.0f) {



            // TODO NOT IMPLEMENTED
        }

        // TODO NOT IMPLEMENTED
    }
}

// 0x0fba36f0
// 0x0fba3710
ZRenderBase::~ZRenderBase() {
    // TODO NOT IMPLEMENTED
}

// 0x0fba47a0
void ZRenderBase::Method0x78() {
    // TODO NOT IMPLEMENTED
}

// 0x0fba47c0
void* ZRenderBase::Method0x7C(u32 todo) {
    // TODO NOT IMPLEMENTED
    return nullptr; // TODO
}

// 0x0fba4840
void ZRenderBase::Method0x74() {
    // TODO NOT IMPLEMENTED
}

// 0x0fba4890
void ZRenderBase::Method0x80() {
    // TODO NOT IMPLEMENTED
}

// 0x0fba4910
void ZRenderBase::Method0x84() {
    // TODO NOT IMPLEMENTED
}

// 0x0fba4920
bool ZRenderBase::Method0x88() {
    // TODO NOT IMPLEMENTED
}

// 0x0fba4960
void ZRenderBase::Method0x68() {
    if (this->Unk0x8B != nullptr) {
        delete this->Unk0x8B;
    }

    this->Unk0x8B = nullptr;
}

// 0x0fba4980
LinkSortRefTab* ZRenderBase::Method0x6C() {
    return this->Unk0x8B;
}

// 0x0fba4990
void ZRenderBase::Method0x70(LinkSortRefTab* todo) {
    this->Unk0x8B = todo;
}

// 0x0fba49a0
void ZRenderBase::Method0x8C() {
    // TODO NOT IMPLEMENTED
}

// 0x0fba4a20
void ZRenderBase::Method0xDC(f64* value) {
    value[0] = this->Unk0x3E;
    value[1] = this->Unk0x42;
}

// 0x0fba4a40
void ZRenderBase::Method0xE0(f32* value) {
    value[0] = this->Unk0x3E;
    value[1] = this->Unk0x42;
    value[2] = 0.0f;
}

// 0x0fba4a60
void ZRenderBase::SetShowCursorVisibility(u32 visibility) {
    this->ShowCursorVisibility = (f32)(visibility != 0); // TODO
}

// 0x0fba4a80
u32 ZRenderBase::GetShowCursorVisibility() {
    return this->ShowCursorVisibility != 0.0 ? 1 : 0; // TODO
}

// 0x0fba4aa0
s32 ZRenderBase::GetWindowWidth() {
    return this->W;
}

// 0x0fba4ab0
s32 ZRenderBase::GetWindowHeight() {
    return this->H;
}

// 0x0fba4ac0
void ZRenderBase::SetWindowSize(s32 width, s32 height) {
    this->W = width;
    this->H = height;
}

// 0x0fba4ae0
void ZRenderBase::Method0xF8(u32, u32, u32) {}

// 0x0fba4af0
f32 ZRenderBase::Method0x9C() {
    if (g_pSysInterface->IsEngineRunning && this->Unk0x8B != nullptr) {
        // TODO NOT IMPLEMENTED
    }
}

// 0x0fba4b90
void ZRenderBase::Method0x98() {
    // TODO NOT IMPLEMENTED
}

// 0x0fba4f30
void ZRenderBase::Method0xE8(s32 todo1, s32 todo2, const char* value, ...) {
    if (g_pSysInterface->DebugOptionsVisibility != 0.0f) {
        const s32 todo_1 = this->Method0xAC(); // TODO
        const s32 todo_2 = this->Method0xB0(); // TODO

        if (todo1 < todo_1 && todo2 < todo_2) {


            //TODO NOT IMPLEMENTED
        }
    }
}

// 0x0fba5030
void ZRenderBase::Method0xBC(f32 value) {
    if (0.0f < value) {
        this->Unk0xF3 = value;
        this->Unk0x123 = value * 5.0f;
        this->Unk0x127 = value * 6.0f;

        if (this->Unk0x5C != nullptr) {
            delete this->Unk0x5C;
            this->Unk0x5C = nullptr;
        }
    }
}

// 0x0fba5090
s32 ZRenderBase::Method0xAC() {
    return (s32)(this->GetWindowWidth() / this->Unk0x123);
}

// 0x0fba50c0
const ZRenderConfiguration* ZRenderBase::GetSupportedConfigurations() {
    return nullptr;
}

// 0x0fba50c0
const char* ZRenderBase::GetRenderName() {
    return nullptr;
}

// 0x0fba50d0
s32 ZRenderBase::Method0xB0() {
    return (s32)(this->GetWindowHeight() / this->Unk0x127);
}

// 0x0fba5100
void ZRenderBase::Method0x130(f64* value) {
    value[0] = (s32)(this->GetWindowWidth() * value[0]) / this->GetWindowWidth();
    value[1] = (s32)(this->GetWindowHeight() * value[1]) / this->GetWindowHeight();
}

// 0x0fba5190
void ZRenderBase::Method0xEC(const char* value) {
    const u32 value = this->Unk0x60;

    this->Unk0x60 = this->Unk0x60 + 1;

    this->Method0xE8(0 /* TODO */, this->Unk0x60, value);
}

// 0x0fba5b90
void ZRenderBase::Method0x118() {}
