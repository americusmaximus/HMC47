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

#pragma pack(push, 1)

template <typename T>
class ZList {
public:
    inline ZList() {
        this->Value = nullptr;
        this->Capacity = 0;
        this->Count = 0;
    }

    inline ~ZList() {
        this->Release();
    }

    inline void Initialize(u32 capacity) {
        this->Release();

        this->Value = new T[capacity];

        if (this->Value == nullptr) {
            printf("ERROR: Cannot allocate array structure");
        }

        this->Count = 0;
        this->Capacity = capacity;
    }

    // 0x0ffc3f60
    inline void Release() {
        if (this->Value != nullptr) {
            delete[] this->Value;

            this->Value = nullptr;
            this->Count = 0;
            this->Capacity = 0;
        }
    }

    inline T* Get(s32 index) {
        return &this->Value[index];
    }

    inline T* Insert() {
        if (this->Capacity <= this->Count) {
            this->Grow();
        }

        return &this->Value[this->Count++];
    }

    inline void Grow() {
        T* items = new T[this->Capacity * 2];

        if (items == nullptr) {
            printf("ERROR: Cannot expand array structure");
        }

        CopyMemory(items, this->Value, this->Count * sizeof(T));

        this->Capacity = 2 * this->Capacity;

        delete[] this->Value;

        this->Value = items;
    }

protected:
public:
    T* Value;                                                                       // 0x0
    u32 Capacity;                                                                   // 0x4
    u32 Count;                                                                      // 0x8
};

#pragma pack(pop)

#if defined(_DEBUG) && !defined(_WIN64)
static_assert(sizeof(ZList<u32>)    == 0xC,     "ZList<u32> size mismatch.");
#endif
