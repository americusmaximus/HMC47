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

/*
LinkRefTab Block Structure:

|_______|_______..._______|______________|______________|......|_______|_______..._______|______________|______________|
|  Key  |      Value      |  Prev Link*  |  Next Link*  |      |  Key  |      Value      |  Prev Link*  |  Next Link*  |

Where:
    Key is the unsigned integer,
    Value is amout of bytes needed for storing of each value,
    Previous Link pointer is the address to previous link,
    Next link pointer is the address of the next link.

*/

#define KEY_TO_PTR(X)           ((void*)X)
#define PTR_TO_KEY(X)           ((u32)X)    /* X64 */

#pragma pack(push, 1)

class LinkRefTab : public RefTab {
public:
    LinkRefTab(u32 count, u32 size);

public:
    virtual ~LinkRefTab();                                                      // 0x0
    virtual void* Insert(u32 key);                                              // 0x4
    virtual void Clear();                                                       // 0xC
    virtual void Clear2();                                                      // 0x10
    virtual u32 GetCount();                                                     // 0x14
    virtual void RemoveKeyValue(RefKeyValue* kv);                               // 0x18
    virtual u32 GetKeyByIndex(u32 i);                                           // 0x24
    virtual RefKeyValue* GetByIndex(u32 i);                                     // 0x28
    virtual void PrintStatus();                                                 // 0x2C
    virtual void Remove(RefLink* link);                                         // 0x38
    virtual void GetStart(RefLink* link);                                       // 0x3C
    virtual void GetEnd(RefLink* link);                                         // 0x40
    virtual RefKeyValue* GetNext(RefLink* link);                                // 0x48
    virtual RefKeyValue* GetPrevious(RefLink* link);                            // 0x50
    virtual RefKeyValue* GetCurrent(RefLink* link);                             // 0x54
    virtual void* InsertAtStart(u32 key);                                       // 0x60
    virtual void* InsertAtEnd(u32 key);                                         // 0x64
    virtual void* InsertBefore(RefKeyValue* kv, u32 key);                       // 0x68
    virtual RefKeyValue* GetPreviousValue(RefKeyValue* kv);                     // 0x6C
    virtual RefKeyValue* GetNextValue(RefKeyValue* kv);                         // 0x70

protected:
    Ref* Next;                                                                  // 0x1C
    Ref* Previous;                                                              // 0x20
    RefTab* Links;                                                              // 0x24
};

#pragma pack(pop)

#ifdef _WIN64
#error LinkRefTab is incompatible with x64.
#endif

#if defined(_DEBUG) && !defined(_WIN64)
static_assert(sizeof(LinkRefTab)    == 0x28,    "LinkRefTab size mismatch.");
#endif
