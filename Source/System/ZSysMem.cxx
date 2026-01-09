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
void ZSysMem::Method0x4(u32 p2 /* TODO */, u32 p3 /* TODO */) {
    if (g_pSysInterface == nullptr
        || g_pSysInterface->Unk0x38F1 || g_pSysInterface->Unk0xA75 != 0.0f) {
        u32 todo1, todo2;
        this->Method0x70(&todo1, &todo2);

        g_pSysInterface->Method0xD8(p2, p3,
            "Process %.3f / %.3f  Internal %.3f / %.3f : %.3f Count %d",
            todo1 / 1048576.0, todo2 / 1048576.0,
            this->Unk0x24E / 1048576.0, this->Unk0x252 / 1048576.0,
            (this->Unk0x1A - this->Unk0x16) / 1048576.0, this->Unk0x24A + this->Unk0x246);
    }
}

// 0x0ffb19d0
void ZSysMem::Method0x8() {
    u32 todo1, todo2;
    this->Method0x70(&todo1, &todo2);

    g_pSysCom->Log("Z:\\Engine\\System\\_Wintel\\Source\\SysMemWintel.cpp", 36)
        ->LogMessage("Alloc  : Process %.3f/%.3f Internal %.3f/%.3f : Count %.3f",
            todo1 / 1048576.0, todo2 / 1048576.0,
            (this->Unk0x1A - this->Unk0x16) / 1048576.0, this->Unk0x24A + this->Unk0x246);

    // TODO NOT IMPLEMENTED
    //g_pSysCom->Log("Z:\\Engine\\System\\_Wintel\\Source\\SysMemWintel.cpp", 37)
    //    ->LogMessage("Texture: Light %d Texture %d", )
}

// 0x0ffb1ad0
void ZSysMem::Method0x70(u32* todo1, u32* todo2) {
    *todo1 = 0; // TODO
    *todo2 = 0; // TODO
}

// 0x0ffb1af0
ZSysMem::ZSysMem() {
    // TODO NOT IMPLEMENTED
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
