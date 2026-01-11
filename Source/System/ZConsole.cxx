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

// 0x0ffc5400
// 0x0ffd4370
ZConsole::ZConsole() {
    FUN_0ffc8900((undefined4*)&this->field_0x10db);

    this->Unk0x7 = 0;
    this->Unk0xB = 0.2f;
    this->Unk0x4 = false;
    this->Unk0x6 = false;

    strcpy(this->Input, ">");

    this->Unk0x10CF = 1;
    this->Unk0x10E7 = 0;
    this->Unk0x10EB = false;
    this->Unk0xFAF = 0;
    this->Unk0xFB3 = 0;
    this->Unk0x5 = false;

    for (u32 i = 0; i < ZCONSOLE_MAX_LINE_COUNT; i++) {
        this->Lines[i] = new char[ZCONSOLE_MAX_LINE_LENGTH];
    }

    ZeroMemory(&this->Commands, ZCONSOLE_MAX_COMMAND_COUNT * sizeof(char*));

    this->Unk0x10D7 = 0;
    this->Unk0x10D3 = 0;


    TODO
}
