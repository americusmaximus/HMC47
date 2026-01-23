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

#pragma once

#include "StaticRefTab.hxx"
#include "ZBinTree.hxx"

#pragma pack(push, 1)

struct ZValueTreeNode : ZBinTreeNode {
    void* Value;
};

class ZValueTree : public ZBinTree {
public:
    ZValueTree(u32 count);
    ~ZValueTree();

public:
    virtual ZBinTreeNode* Remove(ZBinTreeNode* node);                           // 0x4
    virtual void CopyValue(ZBinTreeNode* dst, ZBinTreeNode* src);               // 0x24
    virtual void InsertItem(u32 key, void* value);                              // 0x28
    virtual void RemoveItem(u32 key);                                           // 0x2C
    virtual void* GetItemValue(u32 key);                                        // 0x30

protected:
    StaticRefTab* Items;                                                        // 0x1D
};

#pragma pack(pop)

#ifdef _WIN64
#error ZValueTree is incompatible with x64.
#endif

#if defined(_DEBUG) && !defined(_WIN64)
static_assert(sizeof(ZValueTree) == 0x21, "ZValueTree size mismatch.");
#endif
