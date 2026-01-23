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

#include "ZMalloc.hxx"

// 0x0ffc7530
// 0x0ffd4408
ZMalloc::ZMalloc() {
    this->Unk0x0 = 0;
    this->Unk0x4 = 0;
    this->Unk0x1D8 = 0;
    this->Unk0x1DC = 0;
    this->Unk0x1FD = false;
}

// 0x0ffc7570
ZMalloc::~ZMalloc() {
    ZBinTreeNode* node = this->Unk0x1FE.Nodes;

    while (node != this->Unk0x1FE.Null && node != nullptr) {
        // TODO NOT IMPLEMENTED
    }
}
