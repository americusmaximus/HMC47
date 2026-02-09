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

#include "ZCopiableBinTree.hxx"

#define MAX_ZMALLOC_AVAILABLE_BUCKET_COUNT      100

#pragma pack(push, 1)

class ZMalloc {
public:
    ZMalloc();
    ~ZMalloc();

public:
    void* Allocate(size_t size);
    void Release(void* value);

protected:
    void SetValues(s32* block, ZValueTreeNode* node);

public:
    u32 Allocated;                                                                  // 0x0
    u32 Capacity;                                                                   // 0x4

protected:
    s32* AvailableBlocks[16];                                                       // 0x8
    ZValueTreeNode* AvailableNodes[MAX_ZMALLOC_AVAILABLE_BUCKET_COUNT];             // 0x48
    u32 AvailableNodeCount;                                                         // 0x1D8
    u32 AvailableBlockCount;                                                        // 0x1DC
    ZCopiableBinTree Unk0x1E0;                                                      // 0x1E0
    bool Lock;                                                                      // 0x1FD
    ZBinTree Unk0x1FE;                                                              // 0x1FE

public:
    bool Unk0x21B;                                                                  // 0x21B
};

#pragma pack(pop)

#ifdef _WIN64
#error ZSysMem is incompatible with x64.
#endif

#if defined(_DEBUG) && !defined(_WIN64)
static_assert(sizeof(ZMalloc)   == 0x21C,   "ZMalloc size mismatch.");
#endif
