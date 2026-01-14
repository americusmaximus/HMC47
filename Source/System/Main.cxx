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

// 0x0ffe0518
static s32 g_LibraryCallCount = 0;

// 0x0ffaa7b0
void __cdecl EditPrint(const char* value) {
    g_pSysCom->LogMessage("%s", value);
}

// 0x0ffaa7d0
u32 __cdecl RunEngine(u32 code) {
    g_pSysInterface->Execute(code);

    return EXIT_SUCCESS;
}

// 0x0ffb3070
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        if (g_LibraryCallCount == 0) {
            new ZSysCom();
            new ZSysMem();
            new ZSysFile();
            new ZSysInterface(hModule);

            g_pSysCom->Initialize(g_pSysInterface->MainWindow);
            g_pSysInterface->Initialize();
        }

        g_LibraryCallCount++;
    }
    else if (ul_reason_for_call == DLL_PROCESS_DETACH) {
        if (g_LibraryCallCount == 1) {
            if (g_pSysInterface != nullptr) {
                delete g_pSysInterface;
            }

            if (g_pSysFile != nullptr) {
                delete g_pSysFile;
            }

            g_pSysCom->Release();

            if (g_pSysMem != nullptr) {
                delete g_pSysMem;
            }

            if (g_pSysCom != nullptr) {
                delete g_pSysCom;
            }
        }

        g_LibraryCallCount--;
    }

    return TRUE;
}
