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

#include <System/ZDynamicLoader.hxx>

// 0x0fbb1830
void ZDynamicLoader::Method0x34() {}

// 0x0fbb2ba0
// 0x0fbbb1a8
ZDynamicLoader::ZDynamicLoader() {
    DynamicLibraryName = nullptr;
    DynamicLibraryModule = NULL;
}

// 0x0fbb1260
// 0x0fbb2be0
// 0x0fbb2c20
ZDynamicLoader::~ZDynamicLoader() {
    if (this->DynamicLibraryName != nullptr) {
        delete[] this->DynamicLibraryName;
    }
}

// 0x0fbb2c60
void ZDynamicLoader::SetDynamicLibrary(HMODULE module, const char* name) {
    if (this->DynamicLibraryName != nullptr) {
        delete[] this->DynamicLibraryName;
    }

    this->DynamicLibraryName = new char[strlen(name) + 1];
    strcpy(this->DynamicLibraryName, name);

    this->DynamicLibraryModule = module;
}

// 0x0fbb2ce0
void ZDynamicLoader::Method0x4() {}

// 0x0fbb2ce0
void ZDynamicLoader::Initialize() {}

// 0x0fbb2ce0
void ZDynamicLoader::Release() {}

// 0x0fbb2ce0
void ZDynamicLoader::Method0x14() {}

// 0x0fbb2ce0
void ZDynamicLoader::Method0x18() {}

// 0x0fbb2ce0
void ZDynamicLoader::Method0x1C() {}

// 0x0fbb2ce0
void ZDynamicLoader::Method0x20() {}

// 0x0fbb2ce0
void ZDynamicLoader::Method0x24() {}

// 0x0fbb2ce0
void ZDynamicLoader::Method0x28() {}

// 0x0fbb2d00
void ZDynamicLoader::Method0x10() {}

// 0x0fbb2e00
FARPROC ZDynamicLoader::GetFunctionAddress(const char* name) {
    return this->DynamicLibraryModule == NULL
        ? NULL : GetProcAddress(this->DynamicLibraryModule, name);
}
