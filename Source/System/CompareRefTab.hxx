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

class CompareRefTab : public RefTab {
public:
    CompareRefTab(u32 count, u32 size);

public:
    virtual ~CompareRefTab();                                                   // 0x0
    virtual void PrintStatus();                                                 // 0x2C
    virtual RefKeyValue* GetEquatable(u32 value);                               // 0x60
    virtual void* InsertEquatable(u32 key, u32 value);                          // 0x64
};

#pragma pack(pop)

#ifdef _WIN64
#error CompareRefTab is incompatible with x64.
#endif

#if defined(_DEBUG) && !defined(_WIN64)
static_assert(sizeof(CompareRefTab)     == 0x1C,    "CompareRefTab size mismatch.");
#endif
