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

#include "ZArray.hxx"

#define ZCONSOLE_MAX_ITEM_COUNT     1000
#define ZCONSOLE_MAX_ITEM_LENGTH    150
#define ZCONSOLE_MAX_COMMAND_COUNT  20
#define ZCONSOLE_MAX_INPUT_LENGTH   200

#pragma pack(push, 1)

class ZConsoleCommand {
public:
    ZConsoleCommand(const char* command);

public:
    virtual ~ZConsoleCommand();                                                     // 0x0
    virtual void Method0x4(u32, u32);                                               // 0x4
    virtual void Execute(const char* value) = 0;                                    // 0x8

public:
    char* Command;                                                                  // 0x4
};

struct ZConsoleCommandNode {
    inline ZConsoleCommandNode(ZConsoleCommand* command) {
        this->Next = nullptr;
        this->Previous = nullptr;

        this->NextMatch = nullptr;
        this->PreviousMatch = nullptr;

        this->Command = command;
    }

    ZConsoleCommandNode* Next;                                                      // 0x0
    ZConsoleCommandNode* Previous;                                                  // 0x4
    ZConsoleCommandNode* PreviousMatch;                                             // 0x8
    ZConsoleCommandNode* NextMatch;                                                 // 0xC
    ZConsoleCommand* Command;                                                       // 0x10
};

class ZVisualConsoleCommand : public ZConsoleCommand {
public:
    ZVisualConsoleCommand(const char* command, f32* visibility);

public:
    virtual ~ZVisualConsoleCommand();                                               // 0x0
    virtual void Execute(const char* value);                                        // 0x8

protected:
    f32* Visibility;                                                                // 0x8
};

class ZHelpConsoleCommand;

class ZConsoleHandler {
public:
    ZConsoleHandler();
    ~ZConsoleHandler();

public:
    virtual void Register(ZConsoleCommand* command);                                // 0x0
    virtual void Unregister(ZConsoleCommand* command);                              // 0x4
    virtual ZConsoleCommandNode* Find(const char* command,
        bool direction, bool exact, ZConsoleCommandNode* node);                     // 0x8
    virtual bool Execute(const char* command, const char* value);                   // 0xC
    virtual void PrintStatus(const char* value);                                    // 0x10
    virtual ZConsoleCommandNode* GetNodes();                                        // 0x14

protected:
    ZConsoleCommandNode* Nodes;                                                     // 0x4
    ZHelpConsoleCommand* Help;                                                      // 0x8
};

class ZConsoleAutoCompleteHandler {
public:
    ZConsoleAutoCompleteHandler(ZArray<char const*>* items);

public:
    virtual ~ZConsoleAutoCompleteHandler();                                         // 0x0

public:
    const char* GetMatch(const char* value);

protected:
    ZArray<char const*>* Items;                                                     // 0x4

public:
    char* Match;                                                                    // 0x8
    s32 Index;                                                                      // 0xC
};

class ZConsole {
public:
    ZConsole();
    ~ZConsole();

public:
    virtual bool Animate();                                                         // 0x0
    virtual void __cdecl Append(const char* format, ...);                           // 0x4
    virtual const char* GetItem(s32 i);                                             // 0x8
    virtual bool IsOpen();                                                          // 0xC
    virtual void HandleKeyDown(WPARAM wParam, LPARAM lParam);                       // 0x10
    virtual void HandleKeyUp(WPARAM wParam, LPARAM lParam);                         // 0x14
    virtual void ExecuteCommand();                                                  // 0x18
    virtual void RegisterCommand(ZConsoleCommand* command);                         // 0x1C
    virtual void UnregisterCommand(ZConsoleCommand* command);                       // 0x20
    virtual void RunCommand(const char* command);                                   // 0x24
    virtual void ToggleVisibility();                                                // 0x28
    virtual void RotateCommands(bool direction);                                    // 0x2C
    virtual void AppendCommand(const char* command);                                // 0x30
    virtual f32 GetVisibility();                                                    // 0x34

public:
    bool Visible;                                                                   // 0x4
    bool Shift;                                                                     // 0x5
    bool Open;                                                                      // 0x6
    f32 Visibility;                                                                 // 0x7
    f32 VisibilityIteration;                                                        // 0xB
    char* Items[ZCONSOLE_MAX_ITEM_COUNT];                                           // 0xF
    s32 ItemCount;                                                                  // 0xFAF
    s32 ItemOffset;                                                                 // 0xFB3
    char* Commands[ZCONSOLE_MAX_COMMAND_COUNT];                                     // 0xFB7
    char Input[ZCONSOLE_MAX_INPUT_LENGTH];                                          // 0x1007
    s32 InputLength;                                                                // 0x10CF
    s32 CommandIndex;                                                               // 0x10D3
    s32 CommandCount;                                                               // 0x10D7
    ZConsoleHandler Handler;                                                        // 0x10DB
    u32 Unk0x10E7;                                                                  // 0x10E7
    bool AutoCompleting;                                                            // 0x10EB
    ZArray<char const*>* AutoComplete;                                              // 0x10EC
    ZConsoleAutoCompleteHandler* AutoCompleteHandler;                               // 0x10F0
};

class ZHelpConsoleCommand : public ZConsoleCommand {
public:
    ZHelpConsoleCommand(const char* command, ZConsoleHandler* handler);

public:
    virtual ~ZHelpConsoleCommand();                                                 // 0x0
    virtual void Execute(const char* value);                                        // 0x8

protected:
    ZConsoleHandler* Handler;                                                       // 0x4
};

#pragma pack(pop)

#if defined(_DEBUG) && !defined(_WIN64)
static_assert(sizeof(ZConsole)                      == 0x10F4,  "ZConsole size mismatch.");
static_assert(sizeof(ZConsoleAutoCompleteHandler)   == 0x10,    "ZConsoleAutoCompleteHandler size mismatch.");
static_assert(sizeof(ZConsoleCommand)               == 0x8,     "ZConsoleCommand size mismatch.");
static_assert(sizeof(ZConsoleCommandNode)           == 0x14,    "ZConsoleCommandNode size mismatch.");
static_assert(sizeof(ZConsoleHandler)               == 0xC,     "ZConsoleHandler size mismatch.");
static_assert(sizeof(ZHelpConsoleCommand)           == 0xC,     "ZHelpConsoleCommand size mismatch.");
static_assert(sizeof(ZVisualConsoleCommand)         == 0xC,     "ZVisualConsoleCommand size mismatch.");
#endif
