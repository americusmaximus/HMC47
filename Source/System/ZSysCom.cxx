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

#define ZSYSCOM_ID_MESSAGE                  1234

#define ZSYSCOM_ID_MESSAGE_2                2 /* TODO */
#define ZSYSCOM_ID_MESSAGE_3                3 /* TODO */
#define ZSYSCOM_ID_MESSAGE_4                4 /* TODO */
#define ZSYSCOM_ID_MESSAGE_5                5 /* TODO */
#define ZSYSCOM_ID_MESSAGE_6                6 /* TODO */
#define ZSYSCOM_ID_MESSAGE_7                7 /* TODO */
#define ZSYSCOM_ID_MESSAGE_8                8 /* TODO */
#define ZSYSCOM_ID_MESSAGE_INIT             9
#define ZSYSCOM_ID_MESSAGE_EXIT             10
#define ZSYSCOM_ID_MESSAGE_12               12 /* TODO */
#define ZSYSCOM_ID_MESSAGE_22               22 /* TODO */

#define ZSYSCOM_MAKE_ID_MESSAGE(X)          (this->ID * 256 + X)

#define ZSYSCOM_ID_MESSAGE_MASK             0xFFFFFF00
#define ZSYSCOM_CHECK_ID_MESSAGE_MASK(X)    (X >> 8)
#define ZSYSCOM_GET_ID_MESSAGE(X)           (X & 0x000000FF)

#define ZSYSCOM_COPYDATA_STRING             0

#define MAX_ZSYSCOM_DBG_ITERATION_COUNT     200

#define MAX_ZSYSOM_CONFIG_BUFFER_LENGTH     256

// 0x0ffa44d0
ZSysComBase::ZSysComBase() {
    this->ID = ZSYSCOM_ID_MESSAGE;
    this->Unk0x0C = nullptr;
    this->Unk0x10 = false;
}

// 0x0ffa44f0
ZSysComBase::~ZSysComBase() {}

// 0x0ffa4540
void ZSysComBase::Release() {
    if (this->Unk0x0C != nullptr) {
        delete this->Unk0x0C;
    }

    this->Unk0x0C = nullptr;
}

// 0x0ffa4560
UINT ZSysComBase::GetID() {
    return this->ID;
}

// 0x0ffc8a90
UINT ZSysComBase::GetMsg() {
    return this->Msg;
}

// 0x0ffa4570
ZSysCom::ZSysCom() {
    this->Unk0x23 = nullptr;
    this->Lock = false;
    this->FilePath = nullptr;
    this->FileLine = 0;
    this->Init = false;
    this->Debugger = NULL;

    this->Msg = RegisterWindowMessageA("ZSystemMessage");

    g_pSysCom = this;
}

// 0x0ffa45b0
// 0x0ffa45f0
ZSysCom::~ZSysCom() {
    HWND window = this->Debugger == NULL
        ? HWND_BROADCAST : this->Debugger;

    PostMessageA(window, this->Msg, ZSYSCOM_MAKE_ID_MESSAGE(ZSYSCOM_ID_MESSAGE_EXIT), 0);

    g_pSysCom = nullptr;
}

// 0x0ffa4630
void ZSysCom::Release() {
    if (this->Unk0x23 != nullptr) {
        delete this->Unk0x23;
        this->Unk0x23 = nullptr;
    }

    ZSysComBase::Release();
}

// 0x0ffa4650
void ZSysCom::Initialize(HWND hwnd) {
    this->Window = hwnd;
    this->SendMsg(ZSYSCOM_ID_MESSAGE_INIT, (LPARAM)hwnd, false);

    if (FindWindowA(NULL, "ZDebug") != NULL) {
        for (u32 i = 0; i < MAX_ZSYSCOM_DBG_ITERATION_COUNT; i++) {
            MSG msg;

            while (PeekMessageA(&msg, hwnd, 0, 0, PM_NOREMOVE)) {
                if (GetMessageA(&msg, hwnd, 0, 0)) {
                    TranslateMessage(&msg);
                    DispatchMessageA(&msg);
                }
            }

            if (this->Debugger != NULL) {
                break;
            }

            Sleep(5);
        }
    }

    this->Init = true;
}

// 0x0ffa4730
ZSysCom* ZSysCom::Log(const char* path, u32 line) {
    this->FilePath = path;
    this->FileLine = line;

    return this;
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
    char buffer[4096];

    if (g_pSysInterface != nullptr
        && !g_pSysInterface->Unk0x38F1 && g_pSysInterface->DebugOptionsVisibility == 0.0f) {
        return;
    }

    if (!this->Init) {
        return;
    }

    while (this->Lock) {}

    this->Lock = true;

    this->FormatString(buffer, "(%s:%d)", this->FilePath, this->FileLine);

    va_list args;
    va_start(args, format);
    vsprintf(&buffer[strlen(buffer) + 1], format, args);
    va_end(args);

    // TODO '(' ???

    if (buffer[0] != NULL) {
        const size_t len = strlen(buffer);

        if (buffer[len - 1] != '\n') {
            strcat(buffer, "\n");
        }
    }

    if (g_pSysMem->Unk0x4 && g_pSysInterface != nullptr) {
        ZConsole* console = g_pSysInterface->GetConsole();

        if (console != nullptr) {
            console->Append(buffer);
        }
    }

    if (this->Debugger != NULL) {
        COPYDATASTRUCT cds;

        cds.dwData = ZSYSCOM_COPYDATA_STRING;
        cds.cbData = strlen(buffer) + 1;
        cds.lpData = buffer;

        SendMessageA(this->Debugger, WM_COPYDATA, NULL, (LPARAM)&cds);
    }

    this->Lock = false;
}

// 0x0ffa4960
void ZSysCom::Method0x30(const char* format, ...) {
    char buffer[4096];

    if (this->Init) {
        if (g_pSysInterface == nullptr
            || g_pSysInterface->Unk0x38F1 || g_pSysInterface->DebugOptionsVisibility != 0.0f) {
            if (this->Unk0x23 == nullptr) {
                this->Unk0x23 = new StringRefTab(32, 0);
            }

            this->FormatString(buffer, "(%s:%d)", this->FilePath, this->FileLine);

            va_list args;
            va_start(args, format);
            vsprintf(&buffer[strlen(buffer) + 1], format, args);
            va_end(args);

            // TODO '(' ???

            if (this->Unk0x23->TryInsertString(buffer)) {
                this->LogMessage(buffer);
            }
        }
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
            console->Append(buffer);
        }
    }
}

// 0x0ffa4b70
void ZSysCom::Method0x34(const char* format, ...) {
    char buffer[1024];

    if (this->Lock) {
        return;
    }

    while (this->Lock) {}

    this->Lock = true;

    va_list args;
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);

    if (buffer[0] != NULL) {
        if (buffer[strlen(buffer) - 1] != '\n') {
            strcat(buffer, "\n");
        }
    }

    if (this->Debugger == NULL) {
        MessageBoxA(NULL, buffer, "ZSystem Default Output", MB_TOPMOST);
    }
    else {
        COPYDATASTRUCT cds;

        cds.dwData = ZSYSCOM_COPYDATA_STRING;
        cds.cbData = strlen(buffer) + 1;
        cds.lpData = buffer;

        SendMessageA(this->Debugger, WM_COPYDATA, NULL, (LPARAM)&cds);
    }

    this->Lock = false;
}

// 0x0ffa4c70
void ZSysCom::DataToDebug(const char* format, ...) {
    char buffer[1024];

    if (this->Debugger != NULL) {
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
            this->SendMsg(ZSYSCOM_ID_MESSAGE_12, atom, false);
            return;
        }

        g_pSysCom->Log("Z:\\Engine\\System\\_Wintel\\Source\\SysComWintel.cpp", 216)
            ->LogMessage("ZSysComWintel::DataToDebug: Couldn't send \"%s\"", buffer);
    }
}

// 0x0ffa4d40
void ZSysCom::SendMsg(WPARAM wParam, LPARAM lParam, bool send) {
    if (send) {
        if (this->Debugger != NULL) {
            SendMessageA(NULL, this->Msg, ZSYSCOM_MAKE_ID_MESSAGE(wParam), lParam);
        }
    }
    else {
        PostMessageA(this->Debugger == NULL ? HWND_BROADCAST : this->Debugger,
            this->Msg, ZSYSCOM_MAKE_ID_MESSAGE(wParam), lParam);
    }
}

// 0x0ffa4da0
LRESULT ZSysCom::Method0x1C(UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (!(wParam & ZSYSCOM_ID_MESSAGE_MASK)
        || ZSYSCOM_CHECK_ID_MESSAGE_MASK(wParam) == this->ID) {
        switch (ZSYSCOM_GET_ID_MESSAGE(wParam)) {
        case ZSYSCOM_ID_MESSAGE_2: {
            if (this->Debugger == NULL) {
                this->Debugger = (HWND)lParam;
            }

            break;
        }
        case ZSYSCOM_ID_MESSAGE_3: {
            if (this->Debugger == NULL) {
                this->Debugger = (HWND)lParam;
                this->SendMsg(ZSYSCOM_ID_MESSAGE_8, (LPARAM)this->Window, false);
            }

            break;
        }
        case ZSYSCOM_ID_MESSAGE_4: {
            if (this->Debugger == (HWND)lParam) {
                this->Debugger = NULL;
            }

            if (this->Unk0x0C != nullptr) {
                this->Method0x40();
                g_pSysInterface->WindowHasFocus--;
            }

            this->Unk0x10 = false;

            break;
        }
        case ZSYSCOM_ID_MESSAGE_5: {
            if (lParam == NULL) {
                g_pSysCom->Method0x34("ZSM_DOSETTING: Didn't get any string\n");
            }
            else {
                if (!this->Init) {
                    GlobalDeleteAtom((ATOM)lParam);
                }
                else {
                    char buffer[MAX_ZSYSOM_CONFIG_BUFFER_LENGTH];

                    GlobalGetAtomNameA((ATOM)lParam, buffer, MAX_ZSYSOM_CONFIG_BUFFER_LENGTH - 1);
                    GlobalDeleteAtom((ATOM)lParam);
                    g_pSysInterface->Method0x18(buffer, 11); // TODO
                }
            }

            break;
        }
        case ZSYSCOM_ID_MESSAGE_6: {
            if (this->Unk0x0C == nullptr) {
                this->Unk0x0C = new LinkRefTab(128, 5);
                g_pSysInterface->WindowHasFocus++;
            }

            this->Unk0x10 = true;

            break;
        }
        case ZSYSCOM_ID_MESSAGE_7: {
            if (this->Unk0x0C != nullptr) {
                this->Method0x40();
                g_pSysInterface->WindowHasFocus--;
            }

            this->Unk0x10 = false;

            break;
        }
        }
    }

    return 0;
}

// 0x0ffa4f60
void ZSysCom::Method0x3C() {
    RefLink link;

    if (g_pSysCom->Unk0x0C != nullptr) {
        if (this->Unk0x10 && this->Unk0x0C != nullptr) {
            this->Unk0x0C->GetStart(&link);
            RefKeyValue* kv = this->Unk0x0C->GetNext(&link);

            while (kv != nullptr) {

                // TODO NOT IMPLEMENTED



                kv = this->Unk0x0C->GetNext(&link);
            }
        }

        this->SendMsg(ZSYSCOM_ID_MESSAGE_22, 0, false);
        this->Unk0x10 = false;
    }
}

// 0x0ffa51f0
void ZSysCom::Method0x40() {
    RefLink link;

    if (this->Unk0x0C != nullptr) {
        this->Unk0x0C->GetStart(&link);
        RefKeyValue* kv = this->Unk0x0C->GetNext(&link);

        while (kv != nullptr) {

            // TODO NOT IMPLEMENTED

            kv = this->Unk0x0C->GetNext(&link);
        }

        this->Unk0x0C->Clear();
        if (this->Unk0x0C != nullptr) {
            delete this->Unk0x0C;
        }

        this->Unk0x0C = nullptr;
    }

    if (this->Unk0x10) {
        this->SendMsg(ZSYSCOM_ID_MESSAGE_22, 0, false);
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
