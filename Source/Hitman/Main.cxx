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

#include <signal.h>

#include <System/ZSysInterface.hxx>

__declspec(dllimport) ZSysInterface* g_pSysInterface;

#define INI_VALUE   ".ini"
#define INI_LENGTH  4

// 0x00401010
void __CRTDECL SignalHandler(int signal) {
    if (g_pSysInterface->SoundLoader != nullptr) {
        g_pSysInterface->SoundLoader->Method0x28();
    }

    if (g_pSysInterface->RenderLoader != nullptr) {
        g_pSysInterface->RenderLoader->Method0x28();
    }
}

// 0x00401040
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpCmdLine, int) {
    CreateMutexA(NULL, FALSE, "Hitboy");

    if (GetLastError() != ERROR_ALREADY_EXISTS) {

        signal(SIGFPE, SignalHandler);
        signal(SIGABRT, SignalHandler);
        signal(SIGILL, SignalHandler);
        signal(SIGSEGV, SignalHandler);

        HMODULE system = LoadLibraryA("System.dll");

        if (system == NULL) {
            return EXIT_FAILURE;
        }

        char exe[MAX_PATH];
        char ini[MAX_PATH];

        const char* cfg = nullptr;

        if (strlen(lpCmdLine) < 2) {
            GetModuleFileNameA(hInstance, exe, MAX_PATH);

            strcpy(&exe[strlen(exe) - INI_LENGTH], INI_VALUE);

            HANDLE file = CreateFileA(exe, GENERIC_READ,
                FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

            if (file == INVALID_HANDLE_VALUE) {
                cfg = "@main.ini";
            }
            else {
                CloseHandle(file);

                strcpy(ini, "@");
                strcat(ini, exe);

                cfg = ini;
            }
        }

        g_pSysInterface->ExecuteWithArgs(cfg);

        FreeLibrary(system);
    }

    return EXIT_SUCCESS;
}
