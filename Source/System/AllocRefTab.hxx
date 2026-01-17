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

#include "RefTab.hxx"

#pragma pack(push, 1)

class AllocRefTab {
public:
    AllocRefTab();

public:
    virtual ~AllocRefTab();                                                     // 0x0
    virtual u32 Create();                                                       // 0x4
    virtual bool Release(u32 value);                                            // 0x8
    virtual bool IsAvailable(u32 value);                                        // 0xC
    virtual void Print(u32 value);                                              // 0x10

protected:
    u32* Items;                                                                 // 0x4
    RefTab* Nodes;                                                              // 0x8
    u32 Count;                                                                  // 0xC
    u32 Capacity;                                                               // 0x10
    u32 Power;                                                                  // 0x14

public:
    inline u32 GetCapacity() {
        return this->Capacity;
    }

    inline u32* GetItems() {
        return this->Items;
    }
};

#pragma pack(pop)

#ifdef _WIN64
#error AllocRefTab is incompatible with x64.
#endif

#if defined(_DEBUG) && !defined(_WIN64)
static_assert(sizeof(AllocRefTab)   == 0x18,    "AllocRefTab size mismatch.");
#endif
