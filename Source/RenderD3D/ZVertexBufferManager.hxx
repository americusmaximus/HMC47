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

#include "ZRenderDevice.hxx"

#include <System/BlockRefTab.hxx>

#pragma pack(push, 1)

// 0x0fbb54d8
class ZVertexBufferManager {
public:
    ZVertexBufferManager(LPGUID id, LPDIRECT3DDEVICE7 device, u32 count, u32 todo1, u32 todo2, u32 todo3);

public:
    virtual ~ZVertexBufferManager();                                                // 0x0

protected:

    // TODO

    BlockRefTab* Unk0x58;                                                           // 0x58

    // TODO

};

#pragma pack(pop)

#if defined(_DEBUG) && !defined(_WIN64)
static_assert(sizeof(ZVertexBufferManager)  == 0x61,    "ZVertexBufferManager size mismatch.");
#endif
