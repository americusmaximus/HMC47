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
#include "ZExceptionRender.hxx"

#include <stdio.h>

#define MAIN_WINDOW_CLASS_NAME_LENGTH   15

#define GRAPGICS_RESOLUTION_640         640
#define GRAPGICS_RESOLUTION_480         480

static const char MainWindowClassName[MAIN_WINDOW_CLASS_NAME_LENGTH + 1] = "ZSystemClass000";

static LRESULT WINAPI MainWindowHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

bool StartEngine(); // TODO

// 0x0ffaad90
ZSysInterface::ZSysInterface(HMODULE module) {
    this->Unk0xA8D = 0; // TODO
    this->Unk0x37B1 = nullptr;

    this->WindowWidth = GRAPGICS_RESOLUTION_640;
    this->WindowHeight = GRAPGICS_RESOLUTION_480;

    this->WindowX = -1;
    this->WindowY = -1;
    this->Unk0x41 = 0; // TODO

    this->Unk0x38EF = false;
    this->IsEngineRunning = false;

    this->Module = module;

    this->Unk0xAA5 = 0; //TODO
    this->Unk0xAA1 = 0; // TODO

    this->RenderModule = nullptr;
    this->ScriptModule = nullptr;
    this->DirectPlayModule = nullptr;
    this->LocaleModule = nullptr;
    this->SoundModule = nullptr;

    this->Unk0x5D = 0; // TODO

    this->Unk0xA85 = NULL;
    this->MainWindow = NULL;

    this->Unk0xAA9 = 0; // TODO
    this->Unk0xAAD = 0.2f;

    ZeroMemory(&this->Unk0xAB1, sizeof(Unk0xF00));

    for (u32 i = 0; i < 256 /* TODO */; i++) {
        this->Unk0xAB1.Unk0x3[i] = true;
    }

    ZeroMemory(&this->Unk0x19B1, sizeof(Unk0x1E00));

    for (u32 i = 0; i < 512 /* TODO */; i++) {
        this->Unk0xAB1.Unk0x3[i] = true;
    }

    this->Unk0x37BD = 0; // TODO
    this->Unk0x37C5 = 0; // TODO
    this->Unk0x37CD = 0; // TODO
    this->Unk0xA79 = 0; // TODO                     
    this->Unk0x37C1 = 0; // TODO
    this->Unk0x37C9 = 0; // TODO
    this->Unk0x37D1 = 0; // TODO                      
    this->Unk0x38F1 = 0; // TODO
    this->Unk0xA95 = 0;
    this->Unk0xA99 = 0;
    this->MouseWheel = 0.0f;
    this->Unk0xA91 = 120; // TODO

    this->Render = nullptr;

    this->DebugOptionsVisibility = 0.0f;
    this->TimersVisibility = 0.0f;
    this->FullScreen = true;

    this->Engine = nullptr;
    this->Unk0x59 = nullptr;

    {
        char* path = new char[MAX_PATH];
        GetModuleFileNameA(this->Module, path, MAX_PATH);

        strrchr(path, '\\')[0] = NULL;

        this->ModulePath = path;

        delete[] path;
    }

    FUN_0ffc40b0(aiStack_44, 0x454d5054, 0, -1);
    FUN_0ffc46a0(aiStack_44);

    g_pSysInterface = this;

    if (this->CreateMainWindow()) {
        this->CommandLine = "";

        this->Unk0x3BCD = 0;
        this->ScriptDebug = false;
        this->ScriptDebugPrint = false;

        this->PlayVideo = "";
        this->RunAction = ExecuteEngine;
        this->LogPath = "Z:\\error.log";

        this->UseTryCatchMainLoop = false;
        this->ExceptionCount = 0;

        FUN_0ffb408e(FUN_0ffaa860);
    }

    TODO
}

// 0x0ffab510
void ZSysInterface::Initialize() {
    this->ProcessingWindowMessages = false;

    g_pSysCom->Log("Z:\\Engine\\System\\_Wintel\\Source\\SysInterfaceWintel.cpp", 643)
        ->LogMessage("---------------------------------------------------------------");

    this->SetWindowTitle(nullptr);
    this->Continue = false;

    this->Unk0x3B4A = "pic0001.tga";
    this->MasterControl = new ZMasterControl();
    this->Continue = true;
}

// 0x0ffab8c0
void ZSysInterface::Method0x20(bool todo) {
    g_pSysFile->Method0x8();

    if (!this->Unk0x6) {
        g_pSysMem->Method0x8();

        if (!todo) {
            if (g_pSysInterface->SoundModule != nullptr) {
                // TODO NOT IMPLEMENTED
            }
        }
    }
}

// 0x0ffacd70
bool ZSysInterface::IsKeyPressed(s32 key) {
    return GetAsyncKeyState(key) & 0x8000;
}

// 0x0ffad1f0
bool ZSysInterface::Execute(u32 code) {
    if (!this->IsEngineRunning) {
        if (!this->Continue) {
            return false;
        }

        this->SetWindowTitle(nullptr);
        this->Continue = false;

        g_pSysFile->Method0x4();

        if (!StartEngine()) {
            return false;
        }

        this->IsEngineRunning = true;
    }

    this->Unk0x5 = this->Unk0x4;
    this->Unk0x4 = true;

    if (this->HandleWindowMessages(NULL) && !this->Continue && !this->Unk0x6) {
        this->RunAction(code);
        this->Method0x8C();

        return true;
    }

    this->RestoreDisplaySettings();
    this->IsEngineRunning = false;

    return false;
}

// 0x0ffad290
void ZSysInterface::ExecuteWithArgs(const char* ini) {
    this->SetCommandLine(ini);

    bool run = this->HandleWindowMessages(NULL);

    if (run) {
        while (this->Execute(1)) {} // TODO

        run = this->Continue;
    }
}

// 0x0ffad2e0
void ZSysInterface::SetCommandLine(const char* ini) {
    this->CommandLine = ini;
}

// 0x0ffad510
void ZSysInterface::Method0xC4() {
    // TODO TRY CATCH

    // TODO

    g_pSysCom->Log("Z:\\Engine\\System\\_Wintel\\Source\\SysInterfaceWintel.cpp", 990)
        ->LogMessage("Closing old render");

    this->Method0x24();

    if (this->RenderModule != nullptr) {
        this->RenderModule->Release();
        this->ReleaseModule(this->RenderModule);
        this->RenderModule = nullptr;
    }

    this->WindowWidth = this->PreviousWindowWidth;
    this->WindowHeight = this->PreviousWindowHeight;

    // TODO

    DestroyWindow(this->MainWindow);

    MSG msg;
    while (PeekMessageA(&msg, NULL, 0, 0, PM_NOREMOVE)) {
        GetMessageA(&msg, NULL, 0, 0);
    }

    Sleep(500);

    while (PeekMessageA(&msg, NULL, 0, 0, PM_NOREMOVE)) {
        GetMessageA(&msg, NULL, 0, 0);
    }

    this->CreateMainWindow();

    g_pSysCom->Log("Z:\\Engine\\System\\_Wintel\\Source\\SysInterfaceWintel.cpp", 1022)
        ->LogMessage("Loading new Render %s", this->DrawDll.AsString());

    this->InitializeModule(&this->RenderModule, this->DrawDll);

    if (this->RenderModule != nullptr) {
        this->RenderModule->InitializeRenderer(this->MainWindow);
    }
    else {
        MessageBoxA(NULL,
            "This render is not supported. Using old render.", "Fatal error", MB_TOPMOST | MB_ICONHAND);
    }

    this->FUN_0ffad7b4();
}

// 0x0ffada10
void ZSysInterface::InitializeModule(ZModule** result, const char* name) {
    if (*result == nullptr && name != nullptr) {
        if (strlen(name) != 0) {
            ZModule* object = this->LoadModule(name);

            *result = object;

            if (object != nullptr) {
                object->Initialize();

                if (object->ModuleHandle == NULL) {
                    *result = nullptr;
                    delete object;

                    g_pSysCom->Log("Z:\\Engine\\System\\_Wintel\\Source\\SysInterfaceWintel.cpp", 1084)
                        ->LogMessage("ERROR: %s initialization error1", name);
                }
            }
            else {
                g_pSysCom->Log("Z:\\Engine\\System\\_Wintel\\Source\\SysInterfaceWintel.cpp", 1087)
                    ->LogMessage("ERROR: %s initialization error2", name);
            }
        }
    }
}

// 0x0ffadad0
bool StartEngine() {
    if (!g_pSysInterface->Unk0x38F1) {
        g_pSysCom->Log("Z:\\Engine\\System\\_Wintel\\Source\\SysInterfaceWintel.cpp", 1106)
            ->LogMessage("Starting engine, Commandline=\"%s\"\n", g_pSysInterface->CommandLine.AsString());
    }

    if (g_pSysInterface->Method0x18(g_pSysInterface->CommandLine, 21 /* TODO */)) {
        if (g_pSysInterface->UseTryCatchMainLoop) {
            g_pSysInterface->RunAction = &ZSysInterface::ExecuteEngineWrapper;
            g_pSysFile->Touch(g_pSysInterface->LogPath);

            // TODO NOT IMPLEMENTED
        }

        g_pSysInterface->Method0x9C();
        g_pSysInterface->Method0xA0();

        if (strlen(g_pSysInterface->PlayVideo) != 0) {
            // TODO NOT IMPLEMENTED
        }

        g_pSysInterface->InitializeModule(&g_pSysInterface->Engine, "EngineData.dll");
        g_pSysInterface->InitializeModule(&g_pSysInterface->RenderModule, g_pSysInterface->DrawDll);

        if (g_pSysInterface->RenderModule == nullptr) {
            if (g_pSysInterface->DrawDll == nullptr) {
                g_pSysCom->LogFatal("No Render is selected. Please select one.");
            }
            else {
                g_pSysCom->LogFatal("Render '%s' is not supported.", g_pSysInterface->DrawDll.AsString());
            }
        }

        g_pSysInterface->InitializeModule(&g_pSysInterface->ScriptModule, g_pSysInterface->ScriptDll);
        g_pSysInterface->InitializeModule(&g_pSysInterface->SoundModule, g_pSysInterface->SoundDll);
        g_pSysInterface->InitializeModule(&g_pSysInterface->DirectPlayModule, g_pSysInterface->DirectPlayDll);
        g_pSysInterface->InitializeModule(&g_pSysInterface->LocaleModule, g_pSysInterface->LocaleDll);

        // TODO NOT IMPLEMENTED

        return true;
    }

    return false;
}

// 0x0ffadff0
void ZSysInterface::RestoreDisplaySettings() {
    ChangeDisplaySettingsA(NULL, 0);
    exit(EXIT_FAILURE);
}

// 0x0ffae080
void ZSysInterface::ExecuteEngineWrapper(u32 code) {
    try {
        this->ExecuteEngine(code);
    }
    catch (...) {
        g_pSysCom->Log("Z:\\Engine\\System\\_Wintel\\Source\\SysInterfaceWintel.cpp", 1325)
            ->LogMessage("Exception caught in main loop");
        this->ExceptionCount++;
    }
}

// 0x0ffae110
void ZSysInterface::ExecuteEngine(u32 code) {
    if (!this->Unk0x38F1 && !this->WindowHasFocus) {
        MSG msg;
        if (!PeekMessageA(&msg, NULL, 0, 0, PM_NOREMOVE)) {
            WaitMessage();
            return;
        }
    }
    else {
        const DWORD64 start = __rdtsc();

        g_pSysInterface->Method0x10C("Z:\\Engine\\System\\_Wintel\\Source\\SysInterfaceWintel.cpp", 1341);
        this->Method0x90();

        for (ZRenderBase* render = this->Render; render != nullptr; render = render->Current) {
            render->Method0x24();

            if (render->Method0x6C() != nullptr) {
                RefLink link;

                LinkSortRefTab* list = render->Method0x6C();
                list->GetStart(&link);
                void* result = REFTAB_KEY_TO_PTR(list->GetNextKey(&link)); // TODO

                while (link.Next != nullptr) {
                    if (result != nullptr) {
                        // TODO NOT IMPLEMENTED

                        result = REFTAB_KEY_TO_PTR(list->GetNextKey(&link)); // TODO
                    }
                }
            }
        }

        this->Unk0x59->Method0x48();
        this->Method0x38(code);

        const DWORD64 end = __rdtsc();

        if (this->TimersVisibility != 0.0f) {
            g_pSysInterface->Method0xD8(0, 30, "Mainloop: %f", (end - start) * 60.0f / this->ProcessorCounter);
        }
    }
}

// 0x0ffb0270
bool ZSysInterface::Method0x18(const char* args, u32 todo) {
    if (strlen(args) != 0) {




        // TODO NOT IMPLEMENTED
    }
}

// 0x0ffb0860
void ZSysInterface::Method0xD8(u32 todo1, u32 todo2, const char* format, ...) {
    char buffer[1023];

    if (g_pSysInterface->DebugOptionsVisibility != 0.0f) {
        // TODO NOT IMPLEMENTED
    }
}

// 0x0ffb09e0
void ZSysInterface::Method0x24() {
    // TODO NOT IMPLEMENTED
}

// 0x0ffb0a00
bool ZSysInterface::CreateMainWindow() {
    WNDCLASSEXA wndc;

    for (u32 i = 0; i != 1000; i++) {
        sprintf((char*)&MainWindowClassName[MAIN_WINDOW_CLASS_NAME_LENGTH - 3], "%3d", i);

        wndc.cbSize = sizeof(WNDCLASSEXA);

        if (!GetClassInfoExA(this->Module, MainWindowClassName, &wndc)) {
            break;
        }
    }

    const s32 width = GetSystemMetrics(SM_CXFULLSCREEN);
    const s32 height = GetSystemMetrics(SM_CYFULLSCREEN);

    const s32 x = (width / 2) - (GRAPGICS_RESOLUTION_640 / 2);
    const s32 y = (height / 2) - (GRAPGICS_RESOLUTION_480 / 2);

    wndc.cbSize = sizeof(WNDCLASSEXA);
    wndc.style = CS_PARENTDC | CS_OWNDC | CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
    wndc.lpfnWndProc = MainWindowHandler;
    wndc.cbClsExtra = 0;
    wndc.cbWndExtra = sizeof(ZSysInterface*);
    wndc.hInstance = this->Module;
    wndc.hIcon = NULL;
    wndc.hCursor = LoadCursorA(NULL, IDC_ARROW);
    wndc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wndc.lpszMenuName = nullptr;
    wndc.lpszClassName = MainWindowClassName;
    wndc.hIconSm = NULL;

    RegisterClassExA(&wndc);

    this->MainWindow = CreateWindowExA(WS_EX_LEFT,
        MainWindowClassName, "", WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, x, y,
        (width / 2 + (GRAPGICS_RESOLUTION_640 / 2)) - x, (height / 2 + (GRAPGICS_RESOLUTION_480 / 2)) - y,
        NULL, NULL, this->Module, nullptr);

    if (this->MainWindow == NULL) {
        return false;
    }

    SetWindowLongA(this->MainWindow, 0, (LONG_PTR)this);

    return true;
}

// 0x0ffb0b50
bool ZSysInterface::HandleWindowMessages(HWND hwnd) {
    if (!this->ProcessingWindowMessages) {
        this->ProcessingWindowMessages = true;

        MSG msg;
        while (PeekMessageA(&msg, hwnd, 0, 0, PM_NOREMOVE)) {
            if (!GetMessageA(&msg, hwnd, 0, 0)) {
                return false;
            }

            TranslateMessage(&msg);
            DispatchMessageA(&msg);
        }

        this->ProcessingWindowMessages = false;
    }

    return true;
}

// 0x0ffb0bf0
void ZSysInterface::Method0x10() {
    this->Unk0x6 = true;

    if (this->ExceptionCount != 0) {
        g_pSysCom->Log("Z:\\Engine\\System\\_Wintel\\Source\\SysInterfaceWintel.cpp", 2156)
            ->LogMessage("Errors occured during execution - an error log has been generated in %s",
                this->LogPath.AsString());
    }
}

// 0x0ffb0c50
void ZSysInterface::SetWindowTitle(const char* title) {
    this->WindowTitle = title;
    SetWindowTextA(this->MainWindow, this->WindowTitle);
}

// 0x0ffb0e20
static LRESULT WINAPI MainWindowHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (g_pSysCom == nullptr) {
        return 0;
    }

    if (uMsg == g_pSysCom->GetWindowMessage()) {
        return g_pSysCom->Method0x1C(uMsg, wParam, lParam);
    }

    switch (uMsg) {
    case WM_DESTROY: {
        if (!g_pSysInterface->Unk0x38F1) {
            PostQuitMessage(EXIT_SUCCESS);
            return 0;
        }
    }
    case WM_SETFOCUS: {
        g_pSysInterface->Method0x9C();
        g_pSysInterface->WindowHasFocus = TRUE;
        break;
    }
    case WM_KILLFOCUS: {
        g_pSysInterface->WindowHasFocus = FALSE;
        break;
    }
    case WM_ACTIVATEAPP: {
        if (wParam) {
            g_pSysInterface->Method0x9C();
            g_pSysInterface->WindowHasFocus = TRUE;

            if (g_pSysInterface->FullScreen) {
                SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
            }
        }
        else {
            g_pSysInterface->WindowHasFocus = FALSE;

            if (g_pSysInterface->FullScreen) {
                SetWindowPos(hWnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
            }

            if (g_pSysInterface->SoundModule != nullptr) {
                if (g_pSysInterface->SoundModule->Unk0x1C != nullptr) {
                    g_pSysInterface->SoundModule->Unk0x1C->Method0x48();
                }
            }
        }

        break;
    }
    case WM_SYSCOMMAND: {
        if (wParam == 0x1000 /* TODO */) {
            MessageBoxA(NULL, "Knep i roeven", "", MB_OK);
        }
        else if (wParam == SC_RESTORE) {
            return 0;
        }
    }
    }

    if (!g_pSysInterface->Unk0x38F1) {
        if (g_pSysInterface->Render != nullptr) {
            return g_pSysInterface->Render->HandleRenderWindowMessages(uMsg, wParam, lParam);
        }
    }

    return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}

// 0x0ffb1140
void ZSysInterface::Method0x9C() {
    this->Unk0x3BCD =
        g_pSysInterface->GetProcessorTicks("Z:\\Engine\\System\\_Wintel\\Source\\SysInterfaceWintel.cpp", 2255);
}

// 0x0ffb17a0
void ZSysInterface::Method0x10C(const char* path, u32 line) {
    // TODO NOT IMPLEMENTED
}

// 0x0ffb1230
void ZSysInterface::Method0xA0() {
    if (this->ProcessorCounter == 0.0) {
        const u64 start =
            g_pSysInterface->GetProcessorTicks("Z:\\Engine\\System\\_Wintel\\Source\\SysInterfaceWintel.cpp", 2264);

        Sleep(1000);

        const u64 end =
            g_pSysInterface->GetProcessorTicks("Z:\\Engine\\System\\_Wintel\\Source\\SysInterfaceWintel.cpp", 2266);

        const f64 cycles = (f64)(end - start);

        if (!g_pSysInterface->Unk0x38F1) {
            g_pSysCom->Log("Z:\\Engine\\System\\_Wintel\\Source\\SysInterfaceWintel.cpp", 2268)
                ->LogMessage("Cycles pr second %f\n", cycles);
        }

        // TODO NOT IMPLEMENTED

        this->ProcessorCounter = cycles;
    }
}

// 0x0ffb1310
void ZSysInterface::Sleep(f32 time) {
    Sleep((u32)time);
}

// 0x0ffb1330
bool ZSysInterface::ReleaseModule(ZModule* ptr) {
    if (ptr == nullptr) {
        return false;
    }

    HMODULE module = ptr->ModuleHandle;

    delete ptr;

    return g_pSysFile->Method0x90(module);
}
