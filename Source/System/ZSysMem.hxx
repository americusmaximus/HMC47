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

#include "AllocRefTab.hxx"
#include "CompareRefTab.hxx"
#include "ZMalloc.hxx"

#pragma pack(push, 1)

struct ZMemBlock {
    u32 Count;                                                                              // 0x0
    s32 Size;                                                                               // 0x4
    u32 Index;                                                                              // 0x8
    void* Value[0];
};

struct ZMemLink {
    ZMemLink* Next;                                                                         // 0x0
    ZMemLink* Previous;                                                                     // 0x4
    char* File;                                                                             // 0x8
    u32 Line;                                                                               // 0xC
    u32 Unk0x10;                                                                            // 0x10
    u32 Count;                                                                              // 0x14
    s32 Size;                                                                               // 0x18
    u32 Index;                                                                              // 0x1C
    void* Value;                                                                            // 0x20
};

// 0x0ffd3740
class ZSysMemBase {
public:
    ZSysMemBase();

public:
    virtual ~ZSysMemBase();                                                                 // 0x0
    virtual void DisplayMemoryUsageStatistics(s32, s32) = 0;                                // 0x4
    virtual void PrintMemoryUsageStatistics() = 0;                                          // 0x8
    virtual void PrintMemoryLink(const char*, ZMemLink*) = 0;                               // 0xC
    virtual void AllocCheck() = 0;                                                          // 0x10
    virtual bool IsMemoryLinkBroken(ZMemLink*) = 0;                                         // 0x14
    virtual void DeleteLinks() = 0;                                                         // 0x18
    virtual void Method0x1C();                                                              // 0x1C
    virtual void Method0x20();                                                              // 0x20
    virtual void* SetMemoryLinkDetails(void*, const char*, u32) = 0;                        // 0x24
    virtual void Index(void*) = 0;                                                          // 0x28
    virtual void* Allocate(size_t) = 0;                                                     // 0x2C
    virtual void* AllocateLinked(size_t) = 0;                                               // 0x30
    virtual bool Delete(void*) = 0;                                                         // 0x34
    virtual void Method0x38() = 0;                                                          // 0x38
    virtual void Method0x3C() = 0;                                                          // 0x3C
    virtual void Method0x40() = 0;                                                          // 0x40
    virtual void Method0x44() = 0;                                                          // 0x44
    virtual void Method0x48() = 0;                                                          // 0x48
    virtual void* GetByIndex(u32) = 0;                                                      // 0x4C
    virtual void Method0x50() = 0;                                                          // 0x50
    virtual u32 Method0x54() = 0;                                                           // 0x54
    virtual u32 Method0x58() = 0;                                                           // 0x58
    virtual void Method0x5C() = 0;                                                          // 0x5C
    virtual void Method0x60() = 0;                                                          // 0x60
    virtual void Method0x64() = 0;                                                          // 0x64
    virtual void Method0x68() = 0;                                                          // 0x68
    virtual void Method0x6C() = 0;                                                          // 0x6C

protected:
    bool Init;                                                                              // 0x4
    AllocRefTab* Indx;                                                                      // 0x5
    u32* Indexes;                                                                           // 0x9
    void** Values;                                                                          // 0xD
    bool Alloc;                                                                             // 0x11
};

// 0x0ffd37b8
class ZSysMem : public ZSysMemBase {
public:
    ZSysMem();

public:
    virtual ~ZSysMem();                                                                     // 0x0
    virtual void DisplayStatus(s32 x, s32 y);                                               // 0x4
    virtual void PrintStatus();                                                             // 0x8
    virtual void PrintMemoryLink(const char* mesage, ZMemLink* link);                       // 0xC
    virtual void AllocCheck();                                                              // 0x10
    virtual bool IsMemoryLinkBroken(ZMemLink* link);                                        // 0x14
    virtual void DeleteLinks();                                                             // 0x18
    virtual void Method0x1C();                                                              // 0x1C
    virtual void Method0x20();                                                              // 0x20
    virtual void* SetMemoryLinkDetails(void* value, const char* path, u32 line);            // 0x24
    virtual void Index(void* value);                                                        // 0x28
    virtual void* Allocate(size_t size);                                                    // 0x2C
    virtual void* AllocateLinked(size_t size);                                              // 0x30
    virtual bool Delete(void* value);                                                       // 0x34
    virtual void Method0x38();                                                              // 0x38
    virtual void Method0x3C();                                                              // 0x3C
    virtual void Method0x40();                                                              // 0x40
    virtual void Method0x44();                                                              // 0x44
    virtual void Method0x48();                                                              // 0x48
    virtual void* GetByIndex(u32 index);                                                    // 0x4C
    virtual void Method0x50();                                                              // 0x50
    virtual u32 Method0x54();                                                               // 0x54
    virtual u32 Method0x58();                                                               // 0x58
    virtual void Method0x5C();                                                              // 0x5C
    virtual void Method0x60();                                                              // 0x60
    virtual void Method0x64();                                                              // 0x64
    virtual void Method0x68();                                                              // 0x68
    virtual void Method0x6C();                                                              // 0x6C
    virtual void GetProcessStats(u32* allocated, u32* total);                               // 0x70

protected:
    u32 Unk0x12;                                                                            // 0x12
    ZMalloc Allocator;                                                                      // 0x16
    CompareRefTab* Textures;                                                                // 0x232
    CompareRefTab* Lights;                                                                  // 0x236
    ZMemLink* AllocLinks;                                                                   // 0x23A
    ZMemLink* AllocLinksTail;                                                               // 0x23E
    void* Unk0x242;                                                                         // 0x242
    s32 Allocations;                                                                        // 0x246
    s32 LinkAllocations;                                                                    // 0x24A
    s32 AllocatedSize;                                                                      // 0x24E
    s32 MaxAllocatedSize;                                                                   // 0x252
    u32 Unk0x256;                                                                           // 0x256
};

#pragma pack(pop)

#ifdef _WIN64
#error ZSysMem is incompatible with x64.
#endif

#if defined(_DEBUG) && !defined(_WIN64)
static_assert(sizeof(ZSysMem)       == 0x25A,   "ZSysMem size mismatch.");
static_assert(sizeof(ZSysMemBase)   == 0x12,    "ZSysMemBase size mismatch.");
#endif
