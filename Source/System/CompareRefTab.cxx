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

#include "CompareRefTab.hxx"
#include "Globals.hxx"

#define COMPARELINKREFTAB_GET_COMPAREVALUE(X)   \
            (*(u32*)((size_t)X + (this->BlockSize - 1) * 4))

#define COMPARELINKREFTAB_SET_COMPAREVALUE(X)   \
            (*(u32*)((size_t)X + (this->BlockSize - 2) * 4))

#define COMPARELINKREFTAB_INVALID_VALUE (-1)

// 0x0ffbfe80
// 0x0ffd4040
CompareRefTab::CompareRefTab(u32 count, u32 size) : RefTab(count, size) {
    if (count < 1) {
        count = 1;
    }

    this->MaxBlockSize = (size + 2) * count;
    this->BlockSize = size + 2;
}

// 0x0ffbfed0
// 0x0ffbff10
CompareRefTab::~CompareRefTab() { }

// 0x0ffbff50
void CompareRefTab::PrintStatus() {
    RefLink link;

    if (this != nullptr) {
        this->GetStart(&link);
        RefKeyValue* kv = this->GetNext(&link);

        while (kv != nullptr) {
            g_pSysCom->Log("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 828)
                ->LogMessage("Ref %x %d\n", kv->Key, COMPARELINKREFTAB_GET_COMPAREVALUE(kv->Value));

            kv = this->GetNext(&link);
        }
    }
}

// 0x0ffbffd0
RefKeyValue* CompareRefTab::GetEquatable(u32 value) {
    RefLink link;

    if (this != nullptr) {
        this->GetStart(&link);
        RefKeyValue* kv = this->GetNext(&link);

        while (kv != nullptr) {
            const u32 comparator = COMPARELINKREFTAB_GET_COMPAREVALUE(kv);

            if (comparator != COMPARELINKREFTAB_INVALID_VALUE && comparator == value) {
                return kv;
            }

            kv = this->GetNext(&link);
        }

    }

    return nullptr;
}

// 0x0ffc0030
void* CompareRefTab::InsertEquatable(u32 key, u32 value) {
    void* result = this->Insert(key);

    COMPARELINKREFTAB_SET_COMPAREVALUE(result) = value;

    return result;
}
