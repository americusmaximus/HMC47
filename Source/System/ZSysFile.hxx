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

struct SysFileInfo {
    u32 Size;
    char* Name;
};

// 0x0ffd3344
class ZSysFileBase {
public:
    ZSysFileBase();

public:
    virtual void Method0x0() = 0;                                                           // 0x0
    virtual void Method0x4() = 0;                                                           // 0x4
    virtual void Method0x8() = 0;                                                           // 0x8
    virtual HANDLE Open(const char*) = 0;                                                   // 0xC
    virtual HANDLE OpenForAppend(const char*) = 0;                                          // 0x10
    virtual HANDLE OpenForRandomAccess(const char*) = 0;                                    // 0x14
    virtual HANDLE Create(const char*) = 0;                                                 // 0x18
    virtual bool Touch(const char*) = 0;                                                    // 0x1C
    virtual void Close(HANDLE) = 0;                                                         // 0x20
    virtual bool GetTime(const char*, LPFILETIME, bool) = 0;                                // 0x24
    virtual u32 GetSize(const char*, bool) = 0;                                             // 0x28
    virtual bool Exists(const char*, bool) = 0;                                             // 0x2C
    virtual bool WriteTo(HANDLE, const void*, u32) = 0;                                     // 0x30
    virtual u32 ReadFrom(HANDLE, void*, u32) = 0;                                           // 0x34
    virtual void CD(const char*) = 0;                                                       // 0x38
    virtual void MakeDir(const char*) = 0;                                                  // 0x3C
    virtual void Rename(const char*, const char*) = 0;                                      // 0x40
    virtual void Copy(const char*, const char*) = 0;                                        // 0x44
    virtual bool Delete(const char*) = 0;                                                   // 0x48
    virtual u32 ReadAt(const char*, void*, u32, u32, bool) = 0;                             // 0x4C
    virtual void Method0x50() = 0;                                                          // 0x50
    virtual u32 Method0x54(const char*, u32*) = 0;                                          // 0x54
    virtual void Method0x58(const char*, const void*, u32, u32) = 0;                        // 0x58
    virtual void Append(const char*, const void*, u32) = 0;                                 // 0x5C
    virtual void Method0x60() = 0;                                                          // 0x60
    virtual void Method0x64() = 0;                                                          // 0x64
    virtual void Method0x68() = 0;                                                          // 0x68
    virtual void Method0x6C() = 0;                                                          // 0x6C
    virtual void Method0x70() = 0;                                                          // 0x70
    virtual void Method0x74() = 0;                                                          // 0x74
    virtual void Method0x78() = 0;                                                          // 0x78
    virtual void Method0x7C() = 0;                                                          // 0x7C
    virtual void Method0x80() = 0;                                                          // 0x80
    virtual void Method0x84() = 0;                                                          // 0x84
    virtual void Method0x88() = 0;                                                          // 0x88
    virtual void Method0x8C() = 0;                                                          // 0x8C
    virtual bool Method0x90(HMODULE) = 0;                                                   // 0x90
    virtual void Method0x94() = 0;                                                          // 0x94
    virtual ~ZSysFileBase();                                                                // 0x98

protected:
    void* Unk0x4;                                                                           // 0x4
    void* Unk0x8;                                                                           // 0x8
};

// 0x0ffd33e0
class ZSysFile : public ZSysFileBase {
public:
    ZSysFile();

public:
    virtual void Method0x0();                                                               // 0x0
    virtual void Method0x4();                                                               // 0x4
    virtual void Method0x8();                                                               // 0x8
    virtual HANDLE Open(const char* path);                                                  // 0xC
    virtual HANDLE OpenForAppend(const char* path);                                         // 0x10
    virtual HANDLE OpenForRandomAccess(const char* path);                                   // 0x14
    virtual HANDLE Create(const char* path);                                                // 0x18
    virtual bool Touch(const char* path);                                                   // 0x1C
    virtual void Close(HANDLE file);                                                        // 0x20
    virtual bool GetTime(const char* path, LPFILETIME time, bool real);                     // 0x24
    virtual u32 GetSize(const char* path, bool real);                                       // 0x28
    virtual bool Exists(const char* path, bool real);                                       // 0x2C
    virtual bool WriteTo(HANDLE file, const void* ptr, u32 size);                           // 0x30
    virtual u32 ReadFrom(HANDLE file, void* ptr, u32 size);                                 // 0x34
    virtual void CD(const char* path);                                                      // 0x38
    virtual void MakeDir(const char* path);                                                 // 0x3C
    virtual void Rename(const char* src, const char* dst);                                  // 0x40
    virtual void Copy(const char* src, const char* dst);                                    // 0x44
    virtual bool Delete(const char* path);                                                  // 0x48
    virtual u32 ReadAt(const char* path, void* ptr, u32 size, u32 offset, bool real);       // 0x4C
    virtual void Method0x50();                                                              // 0x50
    virtual u32 Method0x54(const char* path, u32* token);                                   // 0x54
    virtual void Method0x58(const char* path, const void* ptr, u32 size, u32 offset);       // 0x58
    virtual void Append(const char* path, const void* ptr, u32 size);                       // 0x5C
    virtual void Method0x60();                                                              // 0x60
    virtual void Method0x64();                                                              // 0x64
    virtual void Method0x68();                                                              // 0x68
    virtual void Method0x6C();                                                              // 0x6C
    virtual void Method0x70();                                                              // 0x70
    virtual void Method0x74();                                                              // 0x74
    virtual void Method0x78();                                                              // 0x78
    virtual void Method0x7C();                                                              // 0x7C
    virtual void Method0x80();                                                              // 0x80
    virtual void Method0x84();                                                              // 0x84
    virtual void Method0x88();                                                              // 0x88
    virtual void Method0x8C();                                                              // 0x8C
    virtual bool Method0x90(HMODULE);                                                       // 0x90
    virtual void Method0x94();                                                              // 0x94
    virtual ~ZSysFile();                                                                    // 0x98
    virtual void Method0x9C(const char* path);                                              // 0x9C
    virtual void Method0xA0();                                                              // 0xA0
    virtual void Method0xA4();                                                              // 0xA4
    virtual void Method0xA8();                                                              // 0xA8
    virtual void Method0xAC();                                                              // 0xAC
    virtual void Method0xB0();                                                              // 0xB0

protected:
    void FUN_0ffa7d20(bool todo);
    const char* FUN_0ffa5730(const char* path);

protected:
    void* Unk0xC;                                                                           // 0xC
    RefTab* Files;                                                                          // 0x10
};

#pragma pack(pop)

#if defined(_DEBUG) && !defined(_WIN64)
static_assert(sizeof(ZSysFileBase)  == 0xC,     "ZSysFileBase size mismatch.");
static_assert(sizeof(ZSysFile)      == 0x14,    "ZSysFile size mismatch.");
#endif
