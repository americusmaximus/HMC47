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

#include "BlockRefTab.hxx"
#include "FastLookup.hxx"
#include "Zip.hxx"
#include "ZList.hxx"

#include <stdio.h>

#pragma pack(push, 1)

class FileSystem {
public:
    FileSystem();

public:
    virtual void PrintStatus() = 0;                                                 // 0x0
    virtual bool Open(const char*, void*) = 0;                                      // 0x4
    virtual void Save(const char*) = 0;                                             // 0x8
    virtual u32 GetSize(const char*) = 0;                                           // 0xC
    virtual bool Exists(const char*) = 0;                                           // 0x10
    virtual u32 Method0x14(const char*) = 0;                                        // 0x14
    virtual void Method0x18() = 0;                                                  // 0x18
};

class FileSystemCache {
public:
    FileSystemCache();

public:
    virtual ~FileSystemCache();                                                     // 0x0
    virtual void Insert(const char*, LFHV*, u32) = 0;                               // 0x4
    virtual bool Find(const char*, LFHV*, u32*) = 0;                                // 0x8
    virtual void Method0xC() = 0;                                                   // 0xC
    virtual void Method0x10() = 0;                                                  // 0x10
};

class ZipFileSystemCache : public FileSystemCache {
public:
    ZipFileSystemCache();

public:
    virtual ~ZipFileSystemCache();                                                  // 0x0
    virtual void Insert(const char* path, LFHV* desc, u32 offset);                  // 0x4
    virtual bool Find(const char* path, LFHV* desc, u32* offset);                   // 0x8
    virtual void Method0xC();                                                       // 0xC
    virtual void Clear();                                                           // 0x10

protected:
    FastLookup Lookup;                                                              // 0x4
    BlockRefTab Items;                                                              // 0x8
};

class ZipFileSystem : public FileSystem {
public:
    ZipFileSystem();

public:
    virtual void PrintStatus();                                                     // 0x0
    virtual bool Open(const char* path, void* todo);                                // 0x4
    virtual void Save(const char* path);                                            // 0x8
    virtual u32 GetSize(const char* path);                                          // 0xC
    virtual bool Exists(const char* path);                                          // 0x10
    virtual u32 Method0x14(const char* path);                                       // 0x14
    virtual void Method0x18();                                                      // 0x18

public:
    u32 Status;                                                                     // 0x4
    bool Init;                                                                      // 0x5
    FILE* Handle;                                                                   // 0x9
    u32 Offset;                                                                     // 0xD
    s32 Unk0x11;                                                                    // 0x11
    bool Unk0x15;                                                                   // 0x15
    EOCDV Directory;                                                                // 0x16
    EOCDV Rune;                                                                     // 0x28
    ZList<CDFHV> Unk0x3A;                                                           // 0x3A
    ZList<CDFHV> Unk0x46;                                                           // 0x46
    ZList<u32> Offsets;                                                             // 0x52
    ZipFileSystemCache* Cache;                                                      // 0x5E
};

#pragma pack(pop)

#if defined(_DEBUG) && !defined(_WIN64)
static_assert(sizeof(ZipFileSystem)         == 0x62, "ZipFileSystem size mismatch.");
static_assert(sizeof(ZipFileSystemCache)    == 0xB0, "ZipFileSystemCache size mismatch.");
#endif
