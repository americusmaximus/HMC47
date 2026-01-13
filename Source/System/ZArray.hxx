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
struct ZArrayNode {
    ZArrayNode* Next;
    T* Value;
};

template <typename T>
class ZArray {
public:
    class RangeError {};

public:
    ZArray() {
        this->First = nullptr;
        this->Last = nullptr;
        this->Capacity = 32;
        this->Count = 0;
    }

    ~ZArray() {
        this->Clear();
    }

    void Clear() {
        ZArrayNode<T>* node = this->First;

        while (node != nullptr) {
            this->First = node->Next;

            delete[] node->Value;
            delete node;
        }

        this->Last = nullptr;
        this->Count = 0;
    }

    T Get(s32 index) {
        ZArrayNode<T>* node = this->First;

        while (node != nullptr) {
            if (index < this->Capacity) {
                if (node != nullptr) {
                    return node->Value[index];
                }

                break;
            }

            node = node->Next;
            index -= this->Capacity;
        }

        throw RangeError();
    }

    void Insert(T value) {
        const s32 index = this->Count % this->Capacity;

        if (index == 0) {
            ZArrayNode<T>* n = new ZArrayNode<T>();
            n->Next = nullptr;
            n->Value = new T[this->Capacity];

            if (this->Last == nullptr) {
                this->First = n;
                this->Last = n;
            }
            else {
                this->Last->Next = n;
                this->Last = this->Last->Next;
            }
        }

        this->Count++;
        this->Last->Value[index] = value;
    }

protected:
    ZArrayNode<T>* First;                                                           // 0x0
    ZArrayNode<T>* Last;                                                            // 0x4

public:
    s32 Capacity;                                                                   // 0x8
    s32 Count;                                                                      // 0xC
};

#pragma pack(pop)

#if defined(_DEBUG) && !defined(_WIN64)
static_assert(sizeof(ZArray<char>) == 0x10, "ZArray<char> size mismatch.");
static_assert(sizeof(ZArray<char const*>) == 0x10, "ZArray<char const *> size mismatch.");
#endif
