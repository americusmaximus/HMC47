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

#include "ZValueTree.hxx"

#define ZVALUETREE_KEYVALUE(X)      ((RefKeyValue*)X)
#define ZVALUETREE_TREENODE(X)      ((ZValueTreeNode*)((size_t)X - sizeof(u32)))

// 0x0ffc8580
// 0x0ffd4458
ZValueTree::ZValueTree(u32 count) {
    this->Items = new StaticRefTab(count, 5);
}

// 0x0ffc8610
ZValueTree::~ZValueTree() {
    while (this->Nodes != this->Null && this->Nodes != nullptr) {
        this->Items->RemoveKeyValue(ZVALUETREE_KEYVALUE(ZBinTree::Remove(this->Nodes)));
        this->Items->RemoveKeyValue(nullptr);
    }

    if (this->Items != nullptr) {
        delete this->Items;
    }
}

// 0x0ffc8660
ZBinTreeNode* ZValueTree::Remove(ZBinTreeNode* node) {
    this->Items->RemoveKeyValue(ZVALUETREE_KEYVALUE(ZBinTree::Remove(node)));

    return nullptr;
}

// 0x0ffc75b0
void ZValueTree::CopyValue(ZBinTreeNode* dst, ZBinTreeNode* src) {
    ((ZValueTreeNode*)dst)->Value = ((ZValueTreeNode*)src)->Value;
}

// 0x0ffc8690
void ZValueTree::InsertItem(u32 key, void* value) {
    ZValueTreeNode* result =
        ZVALUETREE_TREENODE(this->Items->Insert(0));

    result->Key = key;
    result->Value = value;

    this->Insert(result);
}

// 0x0ffc86c0
void ZValueTree::RemoveItem(u32 key) {
    ZBinTreeNode* node = this->GetMatch(key, nullptr);

    if (node != nullptr) {
        this->Items->RemoveKeyValue(ZVALUETREE_KEYVALUE(this->Remove(node)));
    }
}

// 0x0ffc86f0
void* ZValueTree::GetItemValue(u32 key) {
    ZValueTreeNode* node = (ZValueTreeNode*)this->GetMatch(key, nullptr);

    return node == nullptr ? nullptr : node->Value;
}
