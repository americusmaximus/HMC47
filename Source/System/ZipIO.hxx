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

#include "ZipFileSystem.hxx"

#include <zlib.h>

#define ZIPIO_MODE_READ         0x0
#define ZIPIO_MODE_WRITE        0x1
#define ZIPIO_MODE_OPENFILE     0x3

#define ZIPIO_CONTEXT_BUFFER_LENGTH 1024

#pragma pack(push, 1)

struct ZipIOContext {
    u32 Mode;                                                                       // 0x0
    ZIPLFHV Header;                                                                 // 0x4
    u32 CurrentOffset;                                                              // 0x1E
    u32 InitialOffset;                                                              // 0x22
    z_stream Stream;                                                                // 0x26
    u32 Size;                                                                       // 0x5E
    u8 Value[ZIPIO_CONTEXT_BUFFER_LENGTH];                                          // 0x62
    s32 Status;                                                                     // 0x462
};

class ZipIO {
public:
    ZipIO();

public:
    virtual bool Initialize(const char* path, u32 mode);                            // 0x0
    virtual void Release();                                                         // 0x4
    virtual ZipIOContext* OpenFile(const char* path, u32 mode);                     // 0x8
    virtual u32 ReadFile(ZipIOContext* context, void* value, u32 size);             // 0xC
    virtual u32 WriteFile(ZipIOContext* context, void* value, u32 size);            // 0x10
    virtual void CloseFile(ZipIOContext* context);                                  // 0x14
    virtual bool IsEndOfFile(ZipIOContext* context);                                // 0x18
    virtual bool Compare(const char* a, const char* b);                             // 0x1C
    virtual bool Remove(const char* path);                                          // 0x20
    virtual void Append(const char* path, LPFILETIME time, void* value, u32 size);  // 0x24
    virtual bool GetDateTime(const char* path, LPFILETIME time);                    // 0x28
    virtual void SetCompression(s32 level);                                         // 0x2C
    virtual void Close();                                                           // 0x30
    virtual bool ReInitialize();                                                    // 0x34
    virtual void SetIsRune();                                                       // 0x38
    virtual void SetIsCentral();                                                    // 0x3C
    virtual void Method0x40();                                                      // 0x40
    virtual void Method0x44();                                                      // 0x44

public:
    void* Unpack(ZIPLFHV* desc, void* value, void*, u32 size);
    void SaveFile(const char* path, LPFILETIME time, void* value, u32 size);
    bool FindFile(const char* path, ZIPLFHV* desc, u32* offset);

protected:
    u32 GetDirectoryOffset();
    void ReadZipFile();
    void WriteZipFile();
    void Pack(ZIPCDHV* dir, void* value);

protected:
    u32 Mode;                                                                       // 0x4
    char* Name;                                                                     // 0x8
    ZipFileSystem ZFS;                                                              // 0xC
};

#pragma pack(pop)

#if defined(_DEBUG) && !defined(_WIN64)
static_assert(sizeof(ZipIO)         == 0x6E,    "ZipIO size mismatch.");
static_assert(sizeof(ZipIOContext)  == 0x466,   "ZipIOContext size mismatch.");
#endif
