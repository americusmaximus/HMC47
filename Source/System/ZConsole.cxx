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

#include "ZConsole.hxx"

#include <stdio.h>

// 0x0ffc5400
// 0x0ffd4370
ZConsole::ZConsole() {
    this->Unk0x7 = 0.0f;
    this->Unk0xB = 0.2f;
    this->Visible = false;
    this->Unk0x6 = false;

    strcpy(this->Input, ">");

    this->Unk0x10CF = 1;
    this->Unk0x10E7 = 0;
    this->Unk0x10EB = false;
    this->LineCount = 0;
    this->Unk0xFB3 = 0;
    this->Unk0x5 = false;

    for (u32 i = 0; i < ZCONSOLE_MAX_LINE_COUNT; i++) {
        this->Lines[i] = new char[ZCONSOLE_MAX_LINE_LENGTH];
    }

    ZeroMemory(&this->Commands, ZCONSOLE_MAX_COMMAND_COUNT * sizeof(char*));

    this->Unk0x10D7 = 0;
    this->Unk0x10D3 = 0;

    this->AutoComplete = new ZConsoleAutoComplete();
    this->AutoCompleteHandler = new ZConsoleAutoCompleteHandler(this->AutoComplete);
}

// 0x0ffc5560
bool ZConsole::Method0xC() {
    return this->Unk0x6;
}

// 0x0ffc5570
void ZConsole::ToggleVisibility() {
    this->Visible = !this->Visible;
}

// 0x0ffc5580
f32 ZConsole::Method0x34() {
    return this->Unk0x7;
}

// 0x0ffc5590
ZConsole::~ZConsole() {
    for (u32 i = 0; i < ZCONSOLE_MAX_LINE_COUNT; i++) {
        delete this->Lines[i];
    }

    for (u32 i = 0; i < ZCONSOLE_MAX_COMMAND_COUNT; i++) {
        if (this->Commands[i] != nullptr) {
            delete[] this->Commands[i];
        }
    }

    if (this->AutoCompleteHandler != nullptr) {
        delete this->AutoCompleteHandler;
    }

    if (this->AutoComplete != nullptr) {
        delete this->AutoComplete;
    }
}

// 0x0ffc56e0
void ZConsole::Append(const char* format, ...) {
    // TODO NOT IMPLEMENTED
}

// 0x0ffc5770
const char* ZConsole::Method0x8(s32 index) {
    if (index == 0) {
        return this->Input;
    }

    iVar1 = this->Unk0xFB3 + index;

    if (ZCONSOLE_MAX_LINE_COUNT < iVar1 || iVar1 < -ZCONSOLE_MAX_LINE_COUNT) {
        return "";
    }

    uVar2 = this->LineCount + this->Unk0xFB3 + index;

    if (uVar2 < 0) {
        uVar2 = uVar2 + ((999 - uVar2) / ZCONSOLE_MAX_LINE_COUNT) * ZCONSOLE_MAX_LINE_COUNT;
    }

    if (uVar2 >= ZCONSOLE_MAX_LINE_COUNT) {
        uVar2 = uVar2 % ZCONSOLE_MAX_LINE_COUNT;
    }

    return this->Lines[uVar2];
}

// 0x0ffc5810
void ZConsole::RegisterCommand(ZConsoleCommand* command) {
    this->Handler.Register(command);
}

// 0x0ffc5830
void ZConsole::UnregisterCommand(ZConsoleCommand* command) {
    this->Handler.Unregister(command);
}

// 0x0ffc5850
void ZConsole::Method0x2C(bool todo) {
    // TODO NOT IMPLEMENTED
}

// 0x0ffc5920
void ZConsole::Method0x30(const char* command) {
    // TODO NOT IMPLEMENTED
}

// 0x0ffc5a00
void ZConsole::Method0x24(const char* command) {
    sprintf(this->Input, ">%s", command);
    this->Method0x18();
}

// 0x0ffc5a30
void ZConsole::Method0x18() {
    if (this->Input[1] != NULL) {
        this->Method0x30(this->Input);
    }

    this->Unk0xFB3 = 0;

    char* command = strtok(this->Input, " ");

    if (command != nullptr && command[0] != NULL) {
        char* value = strtok(nullptr, "");

        if (this->Handler.Execute(command, value)) {
            this->Append("%s %s", command, value);
        }
        else {
            this->Append("%s - Unknown command - use <commands> to display list", command);
        }

        this->Input[0] = NULL;
        this->Unk0x10CF = 1;
    }
}

// 0x0ffc5ae0
void ZConsole::Method0x10(WPARAM wParam, LPARAM lParam) {
    // TODO NOT IMPLEMENTED
}

// 0x0ffc6000
void ZConsole::Method0x14(u32 todo, u32) {
    if (todo == 16) { // TODO
        this->Unk0x5 = false;
    }
}

// 0x0ffc6010
bool ZConsole::Method0x0() {
    // TODO NOT IMPLEMENTED
}