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

#define ZCONSOLE_MAX_LINE_COUNT     1000
#define ZCONSOLE_MAX_LINE_LENGTH    150
#define ZCONSOLE_MAX_COMMAND_COUNT  20
#define ZCONSOLE_MAX_INPUT_LENGTH   200

#pragma pack(push, 1)

class ZConsoleCommandHandler {
public:
    ZConsoleCommandHandler(const char* command);

public:
    virtual ~ZConsoleCommandHandler();                                              // 0x0
    virtual void Method0x4(u32, u32);                                               // 0x4
    virtual void Execute(const char* command) = 0;                                  // 0x8

protected:
    char* Command;                                                                  // 0x4
};

class ZVisualConsoleCommandHandler : public ZConsoleCommandHandler {
public:
    ZVisualConsoleCommandHandler(const char* command, f32* visibility);

public:
    virtual ~ZVisualConsoleCommandHandler();
    virtual void Execute(const char* command);                                      // 0x8

protected:
    f32* Visibility;                                                                // 0x8
};

class ZConsole {
public:
    ZConsole();
    ~ZConsole();

public:
    void Method0x0();                                                               // 0x0
    void Method0x4();                                                               // 0x4
    void Method0x8();                                                               // 0x8
    void Method0xC();                                                               // 0xC
    void Method0x10();                                                              // 0x10
    void Method0x14();                                                              // 0x14
    void Method0x18();                                                              // 0x18
    void Method0x1C();                                                              // 0x1C
    void Method0x20();                                                              // 0x20
    void Method0x24();                                                              // 0x24
    void Method0x28();                                                              // 0x28
    void Method0x2C();                                                              // 0x2C
    void Method0x30();                                                              // 0x20
    void Method0x34();                                                              // 0x34

protected:
    bool Unk0x4;                                                                    // 0x4
    bool Unk0x5;                                                                    // 0x5
    bool Unk0x6;                                                                    // 0x6
    u32 Unk0x7;                                                                     // 0x7
    f32 Unk0xB;                                                                     // 0xB
    char* Lines[ZCONSOLE_MAX_LINE_COUNT];                                           // 0xF
    u32 Unk0xFAF;                                                                   // 0xFAF
    u32 Unk0xFB3;                                                                   // 0xFB3
    char* Commands[ZCONSOLE_MAX_COMMAND_COUNT];                                     // 0xFB3
    char Input[ZCONSOLE_MAX_INPUT_LENGTH];                                          // 0x1007
    u32 Unk0x10CF;                                                                  // 0x10CF
    u32 Unk0x10D3;                                                                  // 0x10D3
    u32 Unk0x10D7;                                                                  // 0x10D7

    // TODO

    u32 Unk0x10E7;                                                                  // 0x10E7
    bool Unk0x10EB;                                                                 // 0x10EB
    void* Unk0x10EC;                                                                // 0x10EC
    void* Unk0x10F0;                                                                // 0x10F0
};

#pragma pack(pop)

#if defined(_DEBUG) && !defined(_WIN64)
static_assert(sizeof(ZConsoleCommandHandler)        == 0x8,     "ZConsoleCommandHandler size mismatch.");
static_assert(sizeof(ZVisualConsoleCommandHandler)  == 0xC,     "ZVisualConsoleCommandHandler size mismatch.");
static_assert(sizeof(ZConsole)                      == 0x10F4,  "ZConsole size mismatch.");
#endif
