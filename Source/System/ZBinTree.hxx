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

#include "Common.hxx"

#define BINTREE_KEY_TO_PTR(X)           ((void*)X)   /* X64 */
#define BINTREE_PTR_TO_KEY(X)           ((u32)X)     /* X64 */

// ZBinTree is a red black tree implementation.

#pragma pack(push, 1)

struct ZBinTreeNode {
    ZBinTreeNode* Parent;                                                       // 0x0
    ZBinTreeNode* Left;                                                         // 0x4
    ZBinTreeNode* Right;                                                        // 0x8
    u32 Key;                                                                    // 0xC
    bool Color;                                                                 // 0x10
    void* Value[0];
};

class ZBinTree {
public:
    ZBinTree();

public:
    virtual void Insert(ZBinTreeNode* node);                                    // 0x0
    virtual ZBinTreeNode* Remove(ZBinTreeNode* node);                           // 0x4
    virtual ZBinTreeNode* GetMatch(u32 value, ZBinTreeNode* node);              // 0x8
    virtual ZBinTreeNode* GetClose(u32 value, ZBinTreeNode* node);              // 0xC
    virtual ZBinTreeNode* GetMinimum(ZBinTreeNode* node);                       // 0x10
    virtual ZBinTreeNode* GetMaximum(ZBinTreeNode* node);                       // 0x14
    virtual ZBinTreeNode* GetSuccessor(ZBinTreeNode* node);                     // 0x18
    virtual u32 GetDepth(ZBinTreeNode* node);                                   // 0x1C
    virtual void Validate(ZBinTreeNode* node);                                  // 0x20
    virtual void CopyValue(ZBinTreeNode* dst, ZBinTreeNode* src);               // 0x24

protected:
    void InsertNode(ZBinTreeNode* node);
    void BalanceNode(ZBinTreeNode* node);

protected:
    ZBinTreeNode* Nodes;                                                        // 0x4
    ZBinTreeNode* Null;                                                         // 0x8
    ZBinTreeNode Value;                                                         // 0xC
};

#pragma pack(pop)

#ifdef _WIN64
#error ZBinTree is incompatible with x64.
#endif

#if defined(_DEBUG) && !defined(_WIN64)
static_assert(sizeof(ZBinTree)  == 0x1D,    "ZBinTree size mismatch.");
#endif
