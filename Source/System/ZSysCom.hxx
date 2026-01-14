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

#pragma pack(push, 1)

// 0x0ffd32b8
class ZSysComBase {
public:
    ZSysComBase();

public:
    virtual s32 __cdecl FormatString(char*, const char*, ...) = 0;              // 0x0
    virtual ~ZSysComBase();                                                     // 0x4
    virtual void ReleaseUnk0x0C();                                              // 0x8
    virtual UINT GetID();                                                       // 0xC
    virtual UINT GetMsg();                                                      // 0x10
    virtual void Initialize(HWND) = 0;                                          // 0x14
    virtual void SendMsg(WPARAM, LPARAM, bool) = 0;                             // 0x18
    virtual LRESULT Method0x1C(UINT, WPARAM, LPARAM) = 0;                       // 0x1C
    virtual ZSysComBase* Log(const char*, u32) = 0;                             // 0x20
    virtual void __cdecl LogFatal(const char*, ...) = 0;                        // 0x24
    virtual void __cdecl DataToDebug(const char*, ...) = 0;                     // 0x28
    virtual void __cdecl LogMessage(const char*, ...) = 0;                      // 0x2C
    virtual void __cdecl Method0x30(const char*, ...) = 0;                      // 0x30
    virtual void __cdecl Method0x34(const char*, ...) = 0;                      // 0x34
    virtual void __cdecl Method0x38(const char* format, ...) = 0;               // 0x38
    virtual void Method0x3C() = 0;                                              // 0x3C

public:
    UINT Msg;                                                                   // 0x4
    UINT ID;                                                                    // 0x8
    void* Unk0x0C;                                                              // 0xC
    bool Unk0x10;                                                               // 0x10
};

// 0x0ffd3300
class ZSysCom: public ZSysComBase {
public:
    ZSysCom();

public:
    virtual s32 __cdecl FormatString(char*, const char* format, ...);           // 0x0
    virtual ~ZSysCom();                                                         // 0x4
    virtual void ReleaseUnk0x0C();                                              // 0x8
    virtual void Initialize(HWND hwnd);                                         // 0x14
    virtual void SendMsg(WPARAM wParam, LPARAM lParam, bool send);              // 0x18
    virtual LRESULT Method0x1C(UINT uMsg, WPARAM wParam, LPARAM lParam);        // 0x1C
    virtual ZSysCom* Log(const char* path, u32 line);                           // 0x20
    virtual void __cdecl LogFatal(const char* format, ...);                     // 0x24
    virtual void __cdecl DataToDebug(const char* format, ...);                  // 0x28
    virtual void __cdecl LogMessage(const char* format, ...);                   // 0x2C
    virtual void __cdecl Method0x30(const char* format, ...);                   // 0x30
    virtual void __cdecl Method0x34(const char* format, ...);                   // 0x34
    virtual void __cdecl Method0x38(const char* format, ...);                   // 0x38
    virtual void Method0x3C();                                                  // 0x3C
    virtual void Method0x40();                                                  // 0x40

protected:
    bool Lock;                                                                  // 0x11
    bool Init;                                                                  // 0x12
    const char* FilePath;                                                       // 0x13
    u32 FileLine;                                                               // 0x17
    HWND Window;                                                                // 0x1B
    HWND Debugger;                                                              // 0x1F
    void* Unk0x23;                                                              // 0x23
};

#pragma pack(pop)

#if defined(_DEBUG) && !defined(_WIN64)
static_assert(sizeof(ZSysCom)       == 0x27,    "ZSysCom size mismatch.");
static_assert(sizeof(ZSysComBase)   == 0x11,    "ZSysComBase size mismatch.");
#endif
