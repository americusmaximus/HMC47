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

#include "ZModule.hxx"

// 0x0ffc73d0
// 0x0ffd43d0
ZModule::ZModule() {
    this->ModuleName = nullptr;
    this->ModuleHandle = NULL;
}

// 0x0ffc73f0
ZModule::~ZModule() {
    if (this->ModuleName != nullptr) {
        delete[] this->ModuleName;
    }
}

// 0x0ffc7450
void ZModule::SetModule(HMODULE module, const char* name) {
    if (this->ModuleName != nullptr) {
        delete[] this->ModuleName;
    }

    this->ModuleName = new char[strlen(name) + 1];
    strcpy(this->ModuleName, name);

    this->ModuleHandle = module;
}

// 0x0ffc74e0
void ZModule::Method0x4() {}

// 0x0ffc74e0
void ZModule::Initialize() {}

// 0x0ffc74e0
void ZModule::Release() {}

// 0x0ffc74e0
void ZModule::Method0x14() {}

// 0x0ffc74e0
void ZModule::BeginInit() {}

// 0x0ffc74e0
void ZModule::EndInit() {}

// 0x0ffc74e0
void ZModule::Method0x20() {}

// 0x0ffc74e0
void ZModule::Method0x24() {}

// 0x0ffc74e0
void ZModule::Method0x28() {}

// 0x0ffc74f0
void ZModule::Method0x10(const char*) {}

// 0x0ffc7500
FARPROC ZModule::GetFunctionAddress(const char* name) {
    return this->ModuleHandle == NULL
        ? nullptr : GetProcAddress(this->ModuleHandle, name);
}

// 0x0ffc7520
void ZModule::Method0x34() {}