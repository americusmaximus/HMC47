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

#include <stdio.h>
#include <windows.h>

// 0x00401000
int main(int argc, char** argv) {
    if (argc < 2) {
        return printf("For BACKLASH & CLASS releases only.\n");
    }

    HANDLE heap = GetProcessHeap();

    char dir[MAX_PATH];
    char executable[MAX_PATH];

    ZeroMemory(dir, MAX_PATH);

    GetCurrentDirectoryA(MAX_PATH, dir);

    if (FindExecutableA(argv[1], dir, executable) <= (HINSTANCE)0x20) {
        puts("Command not found.");
    }
    else {
        size_t length = strlen(executable);

        for (int i = 2; i < argc; i++) {
            length += strlen(argv[i]);
        }

        char* arguments = (char*)HeapAlloc(heap, HEAP_ZERO_MEMORY, length + argc);

        strcpy(arguments, executable);

        for (int i = 2; i < argc; i++) {
            strcat(arguments, " ");
            strcat(arguments, argv[i]);
        }

        STARTUPINFOA startup;
        ZeroMemory(&startup, sizeof(STARTUPINFOA));

        PROCESS_INFORMATION info;
        ZeroMemory(&info, sizeof(PROCESS_INFORMATION));

        startup.cb = sizeof(STARTUPINFOA);

        if (!CreateProcessA(nullptr, arguments, nullptr, nullptr, FALSE,
            CREATE_NEW_PROCESS_GROUP, nullptr, nullptr, &startup, &info)) {
            puts("Process creation failed.");
        }
        else {
            DWORD code = 0;

            do {
                WaitForSingleObject(info.hProcess, 1000);

                if (!GetExitCodeProcess(info.hProcess, &code)) {
                    code = 0;
                }
            } while (code == STILL_ACTIVE);
        }

        if (info.hProcess != NULL) {
            CloseHandle(info.hProcess);
        }

        if (info.hThread != NULL) {
            CloseHandle(info.hThread);
        }

        if (arguments != nullptr) {
            HeapFree(heap, 0, arguments);
        }
    }
    
    return 0;
}
