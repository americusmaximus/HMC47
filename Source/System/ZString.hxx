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

#pragma once

#include "Common.hxx"

#define MAX_ZSTRING_LENGTH  124

#pragma pack(push, 1)

class ZString {
public:
    inline ZString() {
        this->Ptr = this->Value;
        this->Value[0] = NULL;
    }

    ~ZString();

    inline ZString(const char* value) {
        this->Set(value == nullptr ? "" : value);
    }

    inline char* AsString() {
        return this->Ptr;
    }

    inline ZString& operator=(const ZString& value) {
        if (this != &value) {
            this->Set(value.Ptr);
        }

        return *this;
    }

    inline operator char* () {
        return this->Ptr;
    }

    inline char& operator [](const s32 indx) {
        return this->Ptr[indx];
    }

    inline ZString& operator =(const char* value) {
        this->Set(value == nullptr ? "" : value);

        return *this;
    }

    inline bool operator==(const ZString& value) const {
        return strcmp(this->Ptr, value.Ptr) == 0;
    }

    inline bool operator==(const char* value) const {
        return strcmp(this->Ptr, value == nullptr ? "" : value) == 0;
    }

protected:
    inline void Set(const char* value) {
        if (this->Ptr != this->Value) {
            delete[] this->Ptr;
            this->Ptr = this->Value;
            strcpy(this->Value, "");
        }

        const size_t len = strlen(value);

        if (len <= MAX_ZSTRING_LENGTH) {
            this->Ptr = this->Value;
            strcpy(this->Value, value);
        }
        else {
            this->Ptr = new char[len + 1];
            strcpy(this->Ptr, value);
            strcpy(this->Value, "");
        }
    }

protected:
    char* Ptr;
    char Value[MAX_ZSTRING_LENGTH];
};


#pragma pack(pop)

#if defined(_DEBUG) && !defined(_WIN64)
static_assert(sizeof(ZString)   == 0x80,    "ZString size mismatch.");
#endif