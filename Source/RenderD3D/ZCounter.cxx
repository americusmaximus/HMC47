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

// 0x0fba99e0
ZCounter::ZCounter() {
    this->Items = nullptr;

    this->Unk0x14 = 0;
    this->Unk0x10 = 0;
    this->Unk0x1C = 0;

    this->Unk0x20 = false;

    this->Unk0xC = 0;
    this->Unk0x4 = 0;
    this->Unk0x8 = 0;
}

// 0x0fba9a10
ZCounter::~ZCounter() {
    if (this->Items != nullptr) {
        delete this->Items;
    }
}

// 0x0fba9a30
void ZCounter::SetNameValue(const char* name, u64 value) {
    if (this->Unk0x20) {
        return;
    }

    // TODO NOT IMPLEMENTED
}

// 0x0fba9ec0
void ZCounter::Method0xC() {
    RefLink link;

    if (g_pSysInterface->DebugOptionsVisibility != 0.0f && g_pSysInterface->TimersVisibility != 0.0f) {
        if (!this->Unk0x20 && this->Items != nullptr) {
            this->Items->GetStart(&link);
            // TODO NOT IMPLEMENTED
        }
    }
}

// 0x0fba9f50
u32 ZCounter::GetCount() {
    return this->Items == nullptr
        ? 0 : this->Items->GetCount();
}

// 0x0fba5ba0
u32 ZCounter::GetUnk0xC() {
    return this->Unk0xC;
}

// 0x0fba9f60
LinkRefTab* ZCounter::GetItems() {
    return this->Items;
}
