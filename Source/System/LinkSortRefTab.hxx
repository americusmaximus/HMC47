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

#include "LinkRefTab.hxx"

/*
LinkSortRefTabLinkRefTab Block Structure:

|_______|_______..._______|______|______________|______________|......|_______|_______..._______|______|______________|______________|
|  Key  |      Value      | Sort |  Prev Link*  |  Next Link*  |      |  Key  |      Value      | Sort |  Prev Link*  |  Next Link*  |

Where:
    Key is the unsigned integer,
    Value is amout of bytes needed for storing of each value,
    Sort is the sorting floating point value,
    Previous Link pointer is the address to previous link,
    Next link pointer is the address of the next link.

*/

enum LinkSortRefTabInsert {
    LINKSORTREFTAB_INSERT       = 0,
    LINKSORTREFTAB_BEFORE       = 1,
    LINKSORTREFTAB_AFTER        = 2,
    LINKSORTREFTAB_FORCE_DWORD  = 0x7FFFFFFF
};

#pragma pack(push, 1)

class LinkSortRefTab : public LinkRefTab {
public:
    LinkSortRefTab(u32 count, u32 size);

public:
    virtual ~LinkSortRefTab();                                                  // 0x0
    virtual void Clear();                                                       // 0xC
    virtual void Clear2();                                                      // 0x10
    virtual void RemoveKeyValue(RefKeyValue* kv);                               // 0x18
    virtual void PrintStatus();                                                 // 0x2C
    virtual void Remove(RefLink* link);                                         // 0x38
    virtual void* InsertSort(u32 key, f32 order, LinkSortRefTabInsert mode);    // 0x74
    virtual f32 GetSortValue(RefKeyValue* kv);                                  // 0x78
    virtual void ReplaceAt(RefKeyValue* kv, f32 order);                         // 0x7C

protected:
    RefKeyValue* Current;                                                       // 0x28
};

#pragma pack(pop)

#ifdef _WIN64
#error LinkSortRefTab is incompatible with x64.
#endif

#if defined(_DEBUG) && !defined(_WIN64)
static_assert(sizeof(LinkSortRefTab)    == 0x2C,    "LinkSortRefTab size mismatch.");
#endif
