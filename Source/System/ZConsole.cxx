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
    this->Visibility = 0.0f;
    this->VisibilityIteration = 0.2f;
    this->Visible = false;
    this->Open = false;

    strcpy(this->Input, ">");

    this->InputLength = 1;
    this->Unk0x10E7 = 0;
    this->AutoCompleting = false;
    this->ItemCount = 0;
    this->ItemOffset = 0;
    this->Shift = false;

    for (u32 i = 0; i < ZCONSOLE_MAX_ITEM_COUNT; i++) {
        this->Items[i] = new char[ZCONSOLE_MAX_ITEM_LENGTH];
    }

    ZeroMemory(&this->Commands, ZCONSOLE_MAX_COMMAND_COUNT * sizeof(char*));

    this->CommandCount = 0;
    this->CommandIndex = 0;

    this->AutoComplete = new ZArray<char const*>();
    this->AutoCompleteHandler = new ZConsoleAutoCompleteHandler(this->AutoComplete);
}

// 0x0ffc5560
bool ZConsole::IsOpen() {
    return this->Open;
}

// 0x0ffc5570
void ZConsole::ToggleVisibility() {
    this->Visible = !this->Visible;
}

// 0x0ffc5580
f32 ZConsole::GetVisibility() {
    return this->Visibility;
}

// 0x0ffc5590
ZConsole::~ZConsole() {
    for (u32 i = 0; i < ZCONSOLE_MAX_ITEM_COUNT; i++) {
        delete this->Items[i];
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
    char buffer[1024];

    va_list args;
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);

    char* parenthesis = strchr(buffer, ')');

    if (parenthesis == nullptr) {
        parenthesis = buffer - 1;
    }

    strncpy(this->Items[this->ItemCount], parenthesis + 1, ZCONSOLE_MAX_ITEM_LENGTH - 1);

    this->Items[this->ItemCount][ZCONSOLE_MAX_ITEM_LENGTH - 1] = NULL;
    this->ItemCount++;

    if (this->ItemCount == ZCONSOLE_MAX_ITEM_COUNT) {
        this->ItemCount = 0;
    }
}

// 0x0ffc5770
const char* ZConsole::GetItem(s32 i) {
    if (i == 0) {
        return this->Input;
    }

    s32 index = this->ItemOffset + i;

    if (ZCONSOLE_MAX_ITEM_COUNT < index || index < -ZCONSOLE_MAX_ITEM_COUNT) {
        return "";
    }

    index = this->ItemCount + this->ItemOffset + i;

    if (index < 0) {
        index += ZCONSOLE_MAX_ITEM_COUNT * (((ZCONSOLE_MAX_ITEM_COUNT - 1) - index) / ZCONSOLE_MAX_ITEM_COUNT);
    }

    if (index >= ZCONSOLE_MAX_ITEM_COUNT) {
        index %= ZCONSOLE_MAX_ITEM_COUNT;
    }

    return this->Items[index];
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
void ZConsole::RotateCommands(bool direction) {
    if (this->Commands[0] != nullptr) {
        if (direction) {
            do {
                this->CommandCount++;

                if (this->CommandCount == ZCONSOLE_MAX_COMMAND_COUNT) {
                    this->CommandCount = 0;
                }
            } while (this->Commands[this->CommandCount] == nullptr);
        }
        else {
            do {
                this->CommandCount--;

                if (this->CommandCount == -1) {
                    this->CommandCount = ZCONSOLE_MAX_COMMAND_COUNT - 1;
                }
            } while (this->Commands[this->CommandCount] == nullptr);
        }

        if (this->Commands[this->CommandCount] != nullptr) {
            strcpy(&this->Input[1], this->Commands[this->CommandCount]);
            this->InputLength = strlen(this->Input);
        }
    }
}

// 0x0ffc5920
void ZConsole::AppendCommand(const char* command) {
    if (this->Commands[this->CommandIndex] != nullptr) {
        delete[] this->Commands[this->CommandIndex];
    }

    this->Commands[this->CommandIndex] = new char[strlen(command)];
    strcpy(this->Commands[this->CommandIndex], &command[1]);

    this->CommandIndex++;

    if (this->CommandIndex > ZCONSOLE_MAX_COMMAND_COUNT - 1) {
        this->CommandIndex = 0;
        this->CommandCount = this->CommandIndex;
    }
    else {
        this->CommandCount = this->CommandIndex;
    }
}

// 0x0ffc5a00
void ZConsole::RunCommand(const char* command) {
    sprintf(this->Input, ">%s", command);
    this->ExecuteCommand();
}

// 0x0ffc5a30
void ZConsole::ExecuteCommand() {
    if (this->Input[1] != NULL) {
        this->AppendCommand(this->Input);
    }

    this->ItemOffset = 0;

    char* command = strtok(&this->Input[1], " ");

    if (command != nullptr && command[0] != NULL) {
        char* value = strtok(nullptr, "");

        if (this->Handler.Execute(command, value)) {
            this->Append("%s %s", command, value);
        }
        else {
            this->Append("%s - Unknown command - use <commands> to display list", command);
        }

        this->Input[1] = NULL;
        this->InputLength = 1;
    }
}

// 0x0ffc5ae0
void ZConsole::HandleKeyDown(WPARAM wParam, LPARAM lParam) {
    u32 count = lParam & 0xFFFF;

    if (count == 0) {
        count = 1;
    }

    if (wParam != VK_TAB && wParam != VK_SHIFT) {
        this->AutoCompleting = false;
        this->Unk0x10E7 = 0;
    }

    switch (wParam) {
    case VK_BACK:
    case VK_DELETE: {
        if (count != 0) {
            for (u32 i = 0; i < count; i++) {
                if (this->InputLength > 1) {
                    this->InputLength--;
                    this->Input[this->InputLength] = NULL;
                }
            }

            return;
        }

        break;
    }
    case VK_TAB: {
        // Workflow:
        // Default: auto complete is OFF.
        // Pressing TAB key turns on auto complete.
        // Further TAB key strokes iterate through the autocomplete suggestions.
        //      Additionally, when SHIFT is ressed - autocomplete suggestion iterations are in reverse order.

        if (this->AutoCompleting) {
            if (this->Shift) {
                if (this->AutoCompleteHandler->Match != nullptr) {
                    const s32 index = this->AutoCompleteHandler->Index--;

                    if (index != 0) {
                        const char* match = this->AutoComplete->Get(this->AutoCompleteHandler->Index);

                        if (_strnicmp(this->AutoCompleteHandler->Match,
                            match, strlen(this->AutoCompleteHandler->Match)) == 0) {
                            strcpy(&this->Input[1], match);
                            strcat(this->Input, " ");

                            this->InputLength = strlen(this->Input);
                            this->AutoCompleting = true;

                            return;
                        }
                    }

                    this->AutoCompleteHandler->Index++;
                }
            }
            else {
                if (this->AutoCompleteHandler->Match != nullptr) {
                    this->AutoCompleteHandler->Index++;

                    if (this->AutoCompleteHandler->Index < this->AutoComplete->Count) {
                        const char* match = this->AutoComplete->Get(this->AutoCompleteHandler->Index);

                        if (_strnicmp(this->AutoCompleteHandler->Match,
                            match, strlen(this->AutoCompleteHandler->Match)) == 0) {
                            strcpy(&this->Input[1], match);
                            strcat(this->Input, " ");

                            this->InputLength = strlen(this->Input);
                            this->AutoCompleting = true;

                            return;
                        }
                    }

                    this->AutoCompleteHandler->Index--;
                }
            }
        }
        else {
            this->AutoComplete->Clear();

            for (ZConsoleCommandNode* node = this->Handler.GetNodes(); node != nullptr; node = node->Next) {
                this->AutoComplete->Insert(node->Command->Command);
            }

            const char* match = this->AutoCompleteHandler->GetMatch(&this->Input[1]);

            if (match != nullptr) {
                strcpy(&this->Input[1], match);
                strcat(this->Input, " ");

                this->InputLength = strlen(this->Input);
            }
        }

        this->AutoCompleting = true;

        return;
    }
    case VK_RETURN: {
        this->ExecuteCommand();
        return;
    }
    case VK_SHIFT: {
        this->Shift = true;
        return;
    }
    case VK_ESCAPE: {
        this->InputLength = 1;
        this->Input[1] = NULL;
        return;
    }
    case VK_PRIOR: {
        if (-ZCONSOLE_MAX_ITEM_COUNT < this->ItemOffset) {
            this->ItemOffset--;
            return;
        }

        break;
    }
    case VK_NEXT: {
        if (this->ItemOffset != 0) {
            this->ItemOffset++;
            return;
        }

        break;
    }
    case VK_UP: {
        this->RotateCommands(false);
        return;
    }
    case VK_DOWN: {
        this->RotateCommands(true);
        return;
    }
    }

    for (u32 i = 0; i < count; i++) {
        if (wParam == VK_OEM_MINUS) {
            wParam = '_';
        }
        else if (wParam == VK_OEM_PERIOD) {
            wParam = '.';
        }
        else if (wParam == VK_OEM_2) {
            wParam = '*';
        }

        if (isalnum(wParam) != 0 || wParam == '_' || wParam == ' ' || wParam == '.' || wParam == '*') {
            this->Input[this->InputLength] = (char)wParam;
            this->InputLength++;
            this->Input[this->InputLength] = NULL;
        }
    }
}

// 0x0ffc6000
void ZConsole::HandleKeyUp(WPARAM wParam, LPARAM) {
    if (wParam == VK_SHIFT) {
        this->Shift = false;
    }
}

// 0x0ffc6010
bool ZConsole::Animate() {
    if (this->Visible) {
        this->Open = true;
        this->Visibility += this->VisibilityIteration;

        if (1.0f <= this->Visibility) {
            this->Visibility = 1.0f;

            return true;
        }
    }
    else {
        this->Visibility -= this->VisibilityIteration;

        if (this->Visibility < 0.0f) {
            this->Visibility = 0.0f;
            this->Open = false;

            return true;
        }
    }

    return false;
}
