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

#include "AllocRefTab.hxx"
#include "Globals.hxx"

#define ALLOCREFTAB_CAPACITY        0x00040000
#define ALLOCREFTAB_MASK            0xFFFC0000

#define ALLOCREFTAB_POWER           18          /* 1 << 18 == 0x40000 */

// 0x0ffc1350
// 0x0ffd4214
AllocRefTab::AllocRefTab() {
    this->Power = ALLOCREFTAB_POWER;
    this->Capacity = ALLOCREFTAB_CAPACITY - 1;
    this->Nodes = new RefTab(128, 128, 1);
    this->Items = new u32[ALLOCREFTAB_CAPACITY];

    for (u32 i = 0; i < ALLOCREFTAB_CAPACITY; i++) {
        this->Items[i] = 1;
    }

    this->Count = 0;
}

// 0x0ffc1400
AllocRefTab::~AllocRefTab() {
    delete[] this->Items;

    if (this->Nodes != nullptr) {
        delete this->Nodes;
    }
}

// 0x0ffc14e0
u32 AllocRefTab::Create() {
    RefLink link;

    if (this->Nodes != nullptr) {
        this->Nodes->GetStart(&link);
        u32 key = this->Nodes->GetNextKey(&link);

        while (link.Next != nullptr) {
            this->Nodes->Remove(&link);

            u32 indx = key - 1 | this->Items[key - 1] << (this->Power & 0x1F);

            if (indx & ~this->Capacity) {
                return indx;
            }

            g_pSysCom->Log("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 1381)
                ->LogMessage("Ref %x usedup\n", this->Capacity & indx);

            key = this->Nodes->GetNextKey(&link);
        }
    }

    if (this->Capacity < this->Count) {
        g_pSysCom->LogFatal("AllocRefTab too small %d\n", this->Capacity + 1);
    }

    const u32 result = this->Count++;

    return 1 << (this->Power & 0x1F) | result;
}

// 0x0ffc15c0
bool AllocRefTab::Release(u32 value) {
    if (!(value & ALLOCREFTAB_MASK)) {
        g_pSysCom->Log("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 1392)
            ->LogMessage("Tried to free non valid ref REF %x", value);

        return false;
    }

    if (!this->IsAvailable(value)) {
        g_pSysCom->Log("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 1397)
            ->LogMessage("Tried to free unallocated REF %x", value);

        this->Print(value);

        return false;
    }

    const u32 indx = value & this->Capacity;

    this->Items[indx]++;
    this->Nodes->Insert(indx + 1);

    return true;
}

// 0x0ffc14c0
bool AllocRefTab::IsAvailable(u32 value) {
    const u32 indx = value & this->Capacity;

    return this->Items[indx] == value >> (this->Power & 0x1F);
}

// 0x0ffc1470
void AllocRefTab::Print(u32 value) {
    g_pSysCom->Log("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 1365)
        ->LogMessage("REF %x lo=%d hi=%d\n", value, this->Capacity & value, value >> (this->Power & 0x1F));
}
