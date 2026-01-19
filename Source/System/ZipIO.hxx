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

#define ZIPIO_MODE_READ_ONLY    0x0
#define ZIPIO_MODE_READ_WRITE   0x1

#pragma pack(push, 1)

class ZipIO {
public:
    ZipIO();

public:
    virtual bool Open(const char* path, u32 mode);                                  // 0x0
    virtual void Save();                                                            // 0x4
    virtual void Method0x8();                                                       // 0x8
    virtual void Method0xC();                                                       // 0xC
    virtual void Method0x10();                                                      // 0x10
    virtual void Method0x14();                                                      // 0x14
    virtual void Method0x18();                                                      // 0x18
    virtual void Method0x1C();                                                      // 0x1C
    virtual void Method0x20();                                                      // 0x20
    virtual void Method0x24();                                                      // 0x24
    virtual bool GetDateTime(const char* path, LPFILETIME time);                    // 0x28
    virtual void Method0x2C();                                                      // 0x2C
    virtual void Method0x30();                                                      // 0x30
    virtual bool Reopen();                                                          // 0x34
    virtual void Method0x38();                                                      // 0x38
    virtual void Method0x3C();                                                      // 0x3C
    virtual void Method0x40();                                                      // 0x40
    virtual void Method0x44();                                                      // 0x44

protected:
    u32 GetDirectoryOffset();
    void ReadZipFile();
    void WriteZipFile();

protected:
    u32 Mode;                                                                       // 0x4
    char* Name;                                                                     // 0x8
    ZipFileSystem ZFS;                                                              // 0xC
};

#pragma pack(pop)

#if defined(_DEBUG) && !defined(_WIN64)
static_assert(sizeof(ZipIO)     == 0x6E,    "ZipIO size mismatch.");
#endif
