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

// 0x0ffb1860
ZSysMemBase::ZSysMemBase() {
    this->Unk0x4 = true;
    this->Unk0x11 = true;
}

// 0x0ffb1880
// 0x0ffb18c0
ZSysMemBase::~ZSysMemBase() {}

// 0x0ffb18d0
void ZSysMem::DisplayStatus(s32 x, s32 y) {
    if (g_pSysInterface == nullptr
        || g_pSysInterface->Unk0x38F1 || g_pSysInterface->DebugOptionsVisibility != 0.0f) {
        u32 allocated, total;
        this->GetProcessStats(&allocated, &total);

        g_pSysInterface->DisplayDebugString(x, y,
            "Process %.3f / %.3f  Internal %.3f / %.3f : %.3f Count %d",
            allocated / 1048576.0f, total / 1048576.0f,
            this->Unk0x24E / 1048576.0f, this->Unk0x252 / 1048576.0f,
            (this->Unk0x1A - this->Unk0x16) / 1048576.0f, this->Unk0x24A + this->Unk0x246);
    }
}

// 0x0ffb19d0
void ZSysMem::PrintStatus() {
    u32 allocated, total;
    this->GetProcessStats(&allocated, &total);

    g_pSysCom->Log("Z:\\Engine\\System\\_Wintel\\Source\\SysMemWintel.cpp", 36)
        ->LogMessage("Alloc  : Process %.3f/%.3f Internal %.3f/%.3f : Count %.3f",
            allocated / 1048576.0f, total / 1048576.0f,
            this->Unk0x24E / 1048576.0f, this->Unk0x252 / 1048576.0f, this->Unk0x24A + this->Unk0x246);

    g_pSysCom->Log("Z:\\Engine\\System\\_Wintel\\Source\\SysMemWintel.cpp", 37)
        ->LogMessage("Texture: Light %d Texture %d",
            this->Lights->GetCount(), this->Textures->GetCount());

    if (this->Unk0x242 != nullptr) {
        this->Unk0x242->FUN_0ffa3cd0(false);
    }
}

// 0x0ffb1ad0
void ZSysMem::GetProcessStats(u32* allocated, u32* total) {
    *allocated = 0;
    *total = 0;
}

// 0x0ffb1af0
ZSysMem::ZSysMem() {
    this->Unk0x242 = nullptr;
    this->Unk0x23A = 0;
    this->Unk0x23E = 0;
    this->Unk0x246 = 0;
    this->Unk0x24A = 0;
    this->Unk0x24E = 0;
    this->Unk0x252 = 0;
    this->Unk0x256 = 0;

    g_pSysMem = this;

    this->Textures = new CompareRefTab(256, 0);
    this->Lights = new CompareRefTab(256, 0);
    this->Tab = new AllocRefTab();

    this->Sizes = new u32[this->Tab->GetCapacity() + 1];
    ZeroMemory(this->Sizes, (this->Tab->GetCapacity() + 1) * sizeof(u32));

    this->TabItems = this->Tab->GetItems();
    
    this->Allocator.Unk0x21B = true;
}

// 0x0ffb1d20
// 0x0ffb1d60
ZSysMem::~ZSysMem() {
    // TODO NOT IMPLEMENTED
}

// 0x0ffb1e30
void ZSysMem::Method0x28() {
    // TODO NOT IMPLEMENTED
}

// 0x0ffb1e90
u32 ZSysMem::Method0x4C() {
    // TODO NOT IMPLEMENTED
}

// 0x0ffb1ed0
void ZSysMem::Method0x50() {
    // TODO NOT IMPLEMENTED
}

// 0x0ffb1f00
void ZSysMem::Method0xC() {
    char buffer[512];

    // TODO NOT IMPLEMENTED
    //"File:%-12s Line:%4d, Links: %3d, Size:%6d"

    // g_pSysCom->Method0x34(buffer, )
}

// 0x0ffc74e0
void ZSysMemBase::Method0x1C() {}

// 0x0ffc74e0
void ZSysMemBase::Method0x20() {}
