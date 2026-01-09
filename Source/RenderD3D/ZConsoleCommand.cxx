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

#include "Globals.hxx"

// 0x0fbb5ac0
ZConsoleCommandBase::ZConsoleCommandBase(const char* name) {
    this->Name = new char[strlen(name) + 1];
    strcpy(this->Name, name);
}

// 0x0fba35c0
ZConsoleCommandBase::~ZConsoleCommandBase() {
    delete[] this->Name;
}

// 0x0fba35f0
// 0x0fba3610
ZConsoleCommand::~ZConsoleCommand() {}

// 0x0fbac830
void ZConsoleCommandBase::Method0x4() {}

// 0x0fbac830
void ZConsoleCommand::Method0x4() {}

// 0x0fbb0550
void ZConsoleCommand::SetVisibility(const char* value) {
    if (value == nullptr) {
        g_pSysCom->Log("Z:\\Engine\\ZStdLib\\Source\\ConsoleCommand.cpp", 54)
            ->LogMessage("%s(%p) = %f", this->Name, this->Visibility, *this->Visibility);

        return;
    }

    *this->Visibility = this->FUN_0fbb210c(value);
}


// 0x0fbb5ab4
ZConsoleCommand::ZConsoleCommand(const char* name, f32* visibility)
    : ZConsoleCommandBase(name) {
    this->Visibility = visibility;
}
