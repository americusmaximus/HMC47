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

#include <System/BlockRefTab.hxx>

// 0x0fbaf710
// 0x0fbb6204
BlockRefTab::BlockRefTab() : RefTab(32, 0) {
    this->MaxBlockSize = 32;
    this->BlockSize = 1;
    this->BlockCount = 32;
}

// 0x0fbaf740
// 0x0fbaf780
BlockRefTab::~BlockRefTab() {
    this->BlockCount = this->BlockCount | REFTAB_UNLOCK;

    Ref* ref = this->Head;

    while (ref != nullptr) {
        Ref* next = ref->Next;
        this->DeleteRef(ref);
        ref = next;
    }

    this->Count = 0;
    this->Head = nullptr;
}

// 0x0fbaf800
void BlockRefTab::DeleteRef(Ref* ref) {
    if (ref != (Ref*)&this->Block) {
        delete[] ref;
    }
}

// 0x0fbaf830
Ref* BlockRefTab::NewRef() {
    return this->Head == nullptr
        ? (Ref*)&this->Block
        : (Ref*)new u8[this->MaxBlockSize * sizeof(u32*) + sizeof(Ref)];
}
