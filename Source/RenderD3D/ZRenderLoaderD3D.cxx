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
#include "ZRenderD3D.hxx"
#include "ZRenderModuleD3D.hxx"

ZRenderModuleD3D* g_Module;                                                         // 0x0fbbdac0

// 0x0fba9980
void ZRenderModuleD3D::Method0x28() {
    g_pSysInterface->Method0x24();
}

// 0x0fb81060
// 0x0fbb5190
ZRenderModuleD3D::ZRenderModuleD3D() {
    this->Manager = new ZTextureManagerD3D();

    this->Method0x44();
    this->Method0x50();
}

// 0x0fb810e0
// 0x0fb81110
ZRenderModuleD3D::~ZRenderModuleD3D() {
    if (this->Manager != nullptr) {
        delete this->Manager;
    }

    delete this->Unk0x70;
    delete this->Unk0x74;
}

// 0x0fb81180
void ZRenderModuleD3D::Initialize() {
    this->Method0x48(1); // TODO
}

// 0x0fb81190
void ZRenderModuleD3D::InitializeRenderer(HWND window) {
    ZRenderD3D* render =
        new ZRenderD3D(g_pSysInterface->Module, window, (ZTextureManagerD3D*)this->Manager);

    render->Unk0xC11 = this->Unk0x70;
    render->Unk0xC15 = this->Unk0x74;

    render->Method0x1C8(window);
}

// 0x0fb81240
void ZRenderModuleD3D::Method0x40() {
    RefLink link;

    if (this->Unk0x18 != nullptr) {
        this->Unk0x18->GetEnd(&link);

        u32 result = this->Unk0x18->GetPreviousKey(&link);

        while (link.Next != nullptr) {
            // TODO result->Release();
            result = this->Unk0x18->GetPreviousKey(&link);
        }
    }

    this->Unk0x18->Clear();
}

// 0x0fb812f0
// 0x0fba9990
void ZRenderModuleD3D::Method0x3C(u32* todo) {
    ZRenderModule::Method0x3C(todo);
}

// 0x0fb90a60
void ZRenderModuleD3D::Method0x44() {
    // TODO NOT IMPLEMENTED
}

// 0x0fb96db0
void ZRenderModuleD3D::Method0x50() {
    // TODO NOT IMPLEMENTED
}
