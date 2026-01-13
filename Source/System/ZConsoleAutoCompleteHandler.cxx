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

// 0x0ffd436c
ZConsoleAutoCompleteHandler::ZConsoleAutoCompleteHandler(ZArray<char const*>* items) {
    this->Items = items;
    this->Match = nullptr;
    this->Index = 0;
}

// 0x0ffc5280
ZConsoleAutoCompleteHandler::~ZConsoleAutoCompleteHandler() {
    if (this->Match != nullptr) {
        delete[] this->Match;
    }
}

// 0x0ffc52e0
const char* ZConsoleAutoCompleteHandler::GetMatch(const char* value) {
    if (this->Match != nullptr) {
        delete[] this->Match;
        this->Match = nullptr;
    }

    if (value != nullptr) {
        this->Match = new char[strlen(value) + 1];
        strcpy(this->Match, value);

        for (this->Index = 0; this->Index < this->Items->Count; this->Index++) {
            const char* match = this->Items->Get(this->Index);

            if (_strnicmp(this->Match, match, strlen(this->Match)) == 0) {
                return match;
            }
        }
    }

    return nullptr;
}
