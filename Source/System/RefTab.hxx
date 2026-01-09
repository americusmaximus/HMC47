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

/*
RefTab Block Structure:

|_______|_______..._______|......|_______|_______..._______|
|  Key  |      Value      |      |  Key  |      Value      |

Where:
    Key is the unsigned integer,
    Value is amout of bytes needed for storing of each value.

*/

#define REFTAB_TRAVERSE_FORWARD     1
#define REFTAB_TRAVERSE_BACKWARD    (-1)

#define REFTAB_COPY         0x3FFFFFFF
#define REFTAB_LOCK         0x7FFFFFFF
#define REFTAB_UNLOCK       0x80000000

#pragma pack(push, 1)

struct Ref {
    Ref* Previous;                                                              // 0x0
    Ref* Next;                                                                  // 0x4
    u32 Size;                                                                   // 0xC
    void* Value[0];
};

struct RefLink {
    Ref* Next;                                                                  // 0x0
    u32 Size;                                                                   // 0x4
    s32 Direction;                                                              // 0x8
};

struct RefKeyValue {
    u32 Key;                                                                    // 0x0
    u8 Value[0];
};

class RefTab {
public:
    RefTab(u32 count, u32 size);
    RefTab(u32 count, u32 max, u32 size);

public:
    virtual ~RefTab();                                                          // 0x0
    virtual void* Insert(u32 key);                                              // 0x4
    virtual void* TryInsert(u32 key);                                           // 0x8
    virtual void Clear();                                                       // 0xC
    virtual void Clear2();                                                      // 0x10
    virtual u32 GetCount();                                                     // 0x14
    virtual void RemoveKeyValue(RefKeyValue* kv);                               // 0x18
    virtual bool ContainsKey(u32 key);                                          // 0x1C
    virtual void* GetByKey(u32 key);                                            // 0x20
    virtual u32 GetKeyByIndex(u32 i);                                           // 0x24
    virtual RefKeyValue* GetByIndex(u32 i);                                     // 0x28
    virtual void PrintStatus();                                                 // 0x2C
    virtual void RemoveByKey(u32 key);                                          // 0x30
    virtual bool TryRemoveByKey(u32 key);                                       // 0x34
    virtual void Remove(RefLink* link);                                         // 0x38
    virtual void GetStart(RefLink* link);                                       // 0x3C
    virtual void GetEnd(RefLink* link);                                         // 0x40
    virtual u32 GetNextKey(RefLink* link);                                      // 0x44
    virtual RefKeyValue* GetNext(RefLink* link);                                // 0x48
    virtual u32 GetPreviousKey(RefLink* link);                                  // 0x4C
    virtual RefKeyValue* GetPrevious(RefLink* link);                            // 0x50
    virtual RefKeyValue* GetCurrent(RefLink* link);                             // 0x54
    virtual void DeleteRef(Ref* ref);                                           // 0x58
    virtual Ref* NewRef();                                                      // 0x5C

protected:
    Ref* Head;                                                                  // 0x4
    Ref* Tail;                                                                  // 0x8
    u32 BlockCount;                                                             // 0xC
    u32 MaxBlockSize;                                                           // 0x10
    u32 Count;                                                                  // 0x14
    u32 BlockSize;                                                              // 0x18
};

#pragma pack(pop)

#if defined(_DEBUG) && !defined(_WIN64)
static_assert(sizeof(RefTab)    == 0x1C,    "RefTab size mismatch.");
#endif
