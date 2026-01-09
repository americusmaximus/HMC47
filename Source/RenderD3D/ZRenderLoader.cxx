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

#include <System/Texture.hxx>
#include <System/ZRenderLoader.hxx>

// 0x0fb81a00
void ZRenderLoader::Method0x40() { }

// 0x0fb81a00
void ZRenderLoader::Method0x44() { }

// 0x0fb9aa90
void ZRenderLoader::InitializeRenderer(HWND) { }

// 0x0fba51c0
// 0x0fbb5c48
ZRenderLoader::ZRenderLoader() {
    this->Unk0x1C = new LinkRefTab(8, 0);
    this->Unk0x18 = nullptr;
    this->Count = -1;

    this->Method0x48(0);
}

// 0x0fba5250
// 0x0fba5270
ZRenderLoader::~ZRenderLoader() {
    this->Method0x48(-1);

    if (this->Unk0x1C != nullptr) {
        delete this->Unk0x1C;
    }
}

// 0x0x0fba52d0
void ZRenderLoader::Method0x10() {
    this->Method0x48(this->Count + 1);
}

// 0x0fba52e0
void ZRenderLoader::Method0x14() {
    this->Method0x48(this->Count - 1);
}

// 0x0fba52f0
void ZRenderLoader::Method0x48(s32 count) {
    if (this->Count < count) {
        while (this->Count < count) {
            this->Unk0x18 = new RefTab(64, 0);
            this->Unk0x1C->InsertAtStart(this->Unk0x18);

            // TODO NOT IMPLEMENTED

            this->Count++;
        }

        DAT_0fbc5280 = 0; // TODO
    }

    while (count < this->Count) {
        this->Method0x40();

        // TODO NOT IMPLEMENTED

        this->Count--;
    }

    DAT_0fbc5280 = this->Unk0x20[this->Count];
}

// 0x0fba5410
void ZRenderLoader::Method0x3C(u32* todo) {
    {
        const u32 size = g_pSysFile->GetSize("Pack.LGT", false);

        if (size != INVALID_FILE_SIZE) {
            void* ptr = new u8[size];

            if (size == g_pSysFile->ReadAt("Pack.LGT", ptr, size, 0, false)) {
                LGT* lgt = (LGT*)ptr;

                u32 bytes = 0;

                const u32 multiple = lgt->Unk0x4 & LGT_MULTIPLE_TEXTURES;
                const u32 count = multiple ? lgt->Count : 0;

                // TODO NOT IMPLEMENTED

                for (u32 i = 0; i < count; i++) {
                    if (type == TEXTURE_TYPE_PALN) {
                        // TODO NOT IMPLEMENTED
                    }
                    else if (type == TEXTURE_TYPE_RGBA) {
                        // TODO NOT IMPLEMENTED
                    }
                    else {
                        g_pSysCom->Log("Z:\\Engine\\Drawing\\Source\\RenderBase.cpp", 1262)
                            ->LogMessage("ERROR: Unsupported texture format");
                    }

                    if (i % 50 == 0) { // TODO
                        g_pSysInterface->Unk0x59->Method0x54(1, 0, ((count - i) * 0.5f) / (f32)count);
                    }

                    local_18 = (int*)((int)local_18 + (local_18[1] & 0x3fffffffU));
                }

                if (bytes != 0) {
                    g_pSysCom->Log("Z:\\Engine\\Drawing\\Source\\RenderBase.cpp", 1297)
                        ->LogMessage("Loaded %.2fMb in %d LGT textures", bytes / 1048576.0, count);
                }
            }

            delete[] ptr;
        }
    }

    {
        u32 size = 0;
        if (!g_pSysInterface->DisableDXT
            && (size = g_pSysFile->GetSize("Pack.DXT", false)) != INVALID_FILE_SIZE) {
            void* ptr = new u8[size];

            if (size != g_pSysFile->ReadAt("Pack.DXT", ptr, size, 0, false)) {
                delete[] ptr;
                goto LAB_0fba57f7;
            }
            if (puVar5 == (undefined*)0x0) goto LAB_0fba57f7;
        }
    else

    // TODO NOT IMPLEMENTED
}
