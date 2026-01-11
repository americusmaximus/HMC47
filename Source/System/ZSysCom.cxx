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
#include "ZSysCom.hxx"

#include <stdio.h>

// 0x0ffa44d0
ZSysComBase::ZSysComBase() {
    this->Unk0x08 = 1234; // TODO
    this->Unk0x0C = 0;
    this->Unk0x10 = 0;
}

// 0x0ffa44f0
ZSysComBase::~ZSysComBase() {}

// 0x0ffa4540
void ZSysComBase::ReleaseUnk0x0C() {
    if (this->Unk0x0C != nullptr) {
        delete this->Unk0x0C;
    }

    this->Unk0x0C = nullptr;
}

// 0x0ffa4560
u32 ZSysComBase::GetUnk0x08() {
    return this->Unk0x08;
}

// 0x0ffc8a90
UINT ZSysComBase::GetWindowMessage() {
    return this->WindowMessage;
}

// 0x0ffa4570
ZSysCom::ZSysCom() {
    this->Unk0x23 = 0; // TODO
    this->Unk0x11 = 0; // TODO
    this->FilePath = nullptr;
    this->FileLine = 0;
    this->Unk0x12 = false;
    this->Window2 = NULL;

    this->WindowMessage = RegisterWindowMessageA("ZSystemMessage");

    g_pSysCom = this;
}

// 0x0ffa45b0
// 0x0ffa45f0
ZSysCom::~ZSysCom() {
    PostMessageA(this->Window2 == NULL
        ? HWND_BROADCAST : this->Window2, this->WindowMessage, this->Unk0x08 * 0x100 + 10, 0); // TODO

    g_pSysCom = nullptr;
}

// 0x0ffa4630
void ZSysCom::ReleaseUnk0x0C() {
    if (this->Unk0x23 != nullptr) {
        delete this->Unk0x23;
    }

    ZSysComBase::ReleaseUnk0x0C();
}

// 0x0ffa4650
void ZSysCom::Method0x14(HWND hwnd) {
    this->Window1 = hwnd;
    this->Method0x18(9, (LPARAM)hwnd, false); // TODO

    HWND window = FindWindowA(NULL, "ZDebug");

    if (window == NULL) {
        this->Unk0x12 = true;
        return;
    }

    for (u32 i = 0; i < 200 /* TODO */; i++) {
        MSG msg;

        while (PeekMessageA(&msg, hwnd, 0, 0, PM_NOREMOVE)) {
            if (GetMessageA(&msg, hwnd, 0, 0)) {
                TranslateMessage(&msg);
                DispatchMessageA(&msg);
            }
        }

        if (this->Window2 != NULL) {
            break;
        }

        Sleep(5);
    }

    this->Unk0x12 = true;
}

// 0x0ffa4730
ZSysCom* ZSysCom::Log(const char* path, u32 line) {
    this->FilePath = path;
    this->FileLine = line;
}

// 0x0ffa4750
void ZSysCom::LogFatal(const char* format, ...) {
    char buffer[1024];

    va_list args;
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);

    g_pSysInterface->Method0x24();

    g_pSysCom->Log("Z:\\Engine\\System\\_Wintel\\Source\\SysComWintel.cpp", 93)
        ->LogMessage("ERROR: %s", buffer);

    MessageBoxA(NULL, buffer, "Fatal error", MB_TOPMOST | MB_ICONHAND);

    exit(-1);
}

// 0x0ffa47c0
void ZSysCom::LogMessage(const char* format, ...) {
    // TODO NOT IMPLEMENTED
}

// 0x0ffa4960
void ZSysCom::Method0x30(const char* format, ...) {
    char buffer[4096];

    if ((g_pSysInterface == nullptr
        || g_pSysInterface->Unk0x38F1 || g_pSysInterface->DebugOptionsVisibility != 0.0f)
        && this->Unk0x12) {
        if (this->Unk0x23 == nullptr) {
            // TODO NOT IMPLEMENTED
            //this->Unk0x23 = new TODO();
        }

        this->FormatString(buffer, "(%s:%d)", this->FilePath, this->FileLine);

        // TODO NOT IMPLEMENTED
    }
}

// 0x0ffa4ac0
void ZSysCom::Method0x38(const char* format, ...) {
    char buffer[1024];

    va_list args;
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);

    if (buffer[0] != NULL) {
        if (buffer[strlen(buffer) - 1] != '\n') {
            strcat(buffer, "\n");
        }
    }

    if (g_pSysInterface != nullptr) {
        ZConsole* console = g_pSysInterface->GetConsole();

        if (console != nullptr) {
            console->Method0x4(buffer);
        }
    }
}

// 0x0ffa4b70
void ZSysCom::Method0x34(const char* format, ...) {
    char buffer[1024];

    if (this->Unk0x11) {
        return;
    }

    while (this->Unk0x11) {}

    this->Unk0x11 = true;

    va_list args;
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);

    if (buffer[0] != NULL) {
        if (buffer[strlen(buffer) - 1] != '\n') {
            strcat(buffer, "\n");
        }
    }

    if (this->Window2 == NULL) {
        MessageBoxA(NULL, buffer, "ZSystem Default Output", MB_TOPMOST);
    }
    else {
        COPYDATASTRUCT value;

        value.dwData = 0; // TODO
        value.cbData = strlen(buffer) + 1;
        value.lpData = buffer;

        SendMessageA(this->Window2, WM_COPYDATA, NULL, (LPARAM)&value);
    }

    this->Unk0x11 = false;
}

// 0x0ffa4c70
void ZSysCom::DataToDebug(const char* format, ...) {
    char buffer[1024];

    if (this->Window2 != NULL) {
        va_list args;
        va_start(args, format);
        vsprintf(buffer, format, args);
        va_end(args);

        if (buffer[0] == NULL) {
            g_pSysCom->Log("Z:\\Engine\\System\\_Wintel\\Source\\SysComWintel.cpp", 209)
                ->LogMessage("ZSysComWintel::DataToDebug: Tried to send empty string");
            return;
        }

        ATOM atom = GlobalAddAtomA(buffer);

        if (atom != NULL) {
            this->Method0x18(12 /* TODO */, atom, false);
            return;
        }

        g_pSysCom->Log("Z:\\Engine\\System\\_Wintel\\Source\\SysComWintel.cpp", 216)
            ->LogMessage("ZSysComWintel::DataToDebug: Couldn't send \"%s\"", buffer);
    }
}

// 0x0ffa4d40
void ZSysCom::Method0x18(int wp, LPARAM param, bool send) {
    if (!send) {
        PostMessageA(this->Window2 == NULL
            ? HWND_BROADCAST : this->Window2, this->WindowMessage, this->Unk0x08 * 0x100 + wp, param); // TODO
    }
    else if (this->Window2 != NULL) {
        SendMessageA(NULL, this->WindowMessage, this->Unk0x08 * 0x100 + wp, param); // TODO
    }
}

// 0x0ffa4da0
LRESULT ZSysCom::Method0x1C(UINT uMsg, WPARAM wParam, LPARAM lParam) {
    // TODO NOT IMPLEMENTED
}

// 0x0ffa4f60
void ZSysCom::Method0x3C() {
    if (g_pSysCom->Unk0x0C != nullptr) {
        if (this->Unk0x10 && this->Unk0x0C != nullptr) {
            // TODO NOT IMPLEMENTED
        }

        this->Method0x18(0x16, 0, false); // TODO
        this->Unk0x10 = false;
    }
}

// 0x0ffa51f0
void ZSysCom::Method0x40() {
    if (this->Unk0x0C != nullptr) {
        // TODO NOT IMPLEMENTED
    }

    if (this->Unk0x10) {
        this->Method0x18(0x16, 0, false); // TODO
        this->Unk0x10 = false;
    }
}

// 0x0ffa95e0
s32 ZSysCom::FormatString(char* buffer, const char* format, ...) {
    va_list args;
    va_start(args, format);

    const s32 result = vsprintf(buffer, format, args);

    va_end(args);

    return result;
}
