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
#include "StringRefTab.hxx"

s32 __cdecl StringRefTabStringCompareCallback(void const* a, void const* b);

// 0x0ffc0d30
// 0x0ffd419c
StringRefTab::StringRefTab(u32 count, u32 size) : RefTab(count, size) {}

// 0x0ffc0d70
// 0x0ffc0db0
StringRefTab::~StringRefTab() {
    RefLink link;

    if (this != nullptr) {
        this->GetStart(&link);
        char* key = (char*)REFTAB_KEY_TO_PTR(this->GetNextKey(&link));

        while (link.Next != nullptr) {
            delete[] key;

            key = (char*)REFTAB_KEY_TO_PTR(this->GetNextKey(&link));
        }
    }
}

// 0x0ffc0e80
void StringRefTab::Clear() {
    this->Clear2();
    RefTab::Clear();
}

// 0x0ffc0ec0
void StringRefTab::Clear2() {
    RefLink link;

    if (this != nullptr) {
        this->GetStart(&link);
        char* key = (char*)REFTAB_KEY_TO_PTR(this->GetNextKey(&link));

        while (link.Next != nullptr) {
            delete[] key;

            key = (char*)REFTAB_KEY_TO_PTR(this->GetNextKey(&link));
        }
    }

    this->BlockCount = this->BlockCount | REFTAB_LOCK;
}

// 0x0ffc0f30
char* StringRefTab::InsertString(const char* value) {
    char* str = new char[strlen(value) + 1];
    strcpy(str, value);

    return (char*)RefTab::Insert(REFTAB_PTR_TO_KEY(str));
}

// 0x0ffc0fa0
char* StringRefTab::TryInsertString(const char* value) {
    if (!this->ContainsString(value)) {
        return this->InsertString(value);
    }

    return nullptr;
}

// 0x0ffc0fd0
void StringRefTab::RemoveString(const char* value) {
    RefLink link;

    if (this != nullptr) {
        this->GetStart(&link);
        char* key = (char*)REFTAB_KEY_TO_PTR(this->GetNextKey(&link));

        while (link.Next != nullptr) {
            if (_strcmpi(key, value) == 0) {
                this->Remove(&link);
                this->BlockCount = this->BlockCount | REFTAB_LOCK;
                return;
            }

            key = (char*)REFTAB_KEY_TO_PTR(this->GetNextKey(&link));
        }
    }

    g_pSysCom->Log("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 1267)
        ->LogMessage("WARNING: unable to remove string %s\n", value);
}

// 0x0ffc1070
RefKeyValue* StringRefTab::GetString(const char* value) {
    RefLink link;

    if (this != nullptr) {
        this->GetStart(&link);
        RefKeyValue* kv = this->GetNext(&link);

        while (kv != nullptr) {
            char* key = (char*)kv->Key;

            if (_strcmpi(key, value) == 0) {
                return kv;
            }

            kv = this->GetNext(&link);
        }
    }

    return nullptr;
}

// 0x0ffc10e0
void StringRefTab::Remove(RefLink* link) {
    char* value = nullptr;

    if (link->Direction < REFTAB_TRAVERSE_FORWARD) {
        value = (char*)REFTAB_KEY_TO_PTR(this->GetCurrent(link)->Key);
    }
    else {
        if (link->Size < this->BlockSize) {
            value = (char*)link->Next->Previous->Value[this->MaxBlockSize - this->BlockSize];
        }
        else {
            value = (char*)link->Next->Value[link->Size - this->BlockSize];
        }
    }

    delete[] value;

    RefTab::Remove(link);
}

// 0x0ffc1150
bool StringRefTab::ContainsString(const char* value) {
    RefLink link;

    if (this != nullptr) {
        this->GetStart(&link);
        char* key = (char*)REFTAB_KEY_TO_PTR(this->GetNextKey(&link));

        while (link.Next != nullptr) {
            if (_strcmpi(key, value) == 0) {
                return true;
            }

            key = (char*)REFTAB_KEY_TO_PTR(this->GetNextKey(&link));
        }
    }

    return false;
}

// 0x0ffc11c0
void StringRefTab::PrintStatus() {
    RefLink link;

    if (this != nullptr) {
        this->GetStart(&link);
        char* key = (char*)REFTAB_KEY_TO_PTR(this->GetNextKey(&link));

        while (link.Next != nullptr) {
            g_pSysCom->Log("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 1320)
                ->LogMessage("String %s\n", key);

            key = (char*)REFTAB_KEY_TO_PTR(this->GetNextKey(&link));
        }
    }
}

// 0x0ffc1230
void StringRefTab::SortStrings() {
    RefLink link;

    u32 count = 0;
    char** values = new char* [this->GetCount()];

    if (this != nullptr) {
        this->GetStart(&link);
        char* key = (char*)REFTAB_KEY_TO_PTR(this->GetNextKey(&link));

        while (link.Next != nullptr) {
            values[count++] = key;
            key = (char*)REFTAB_KEY_TO_PTR(this->GetNextKey(&link));
        }
    }

    qsort(values, count, sizeof(char*), StringRefTabStringCompareCallback);

    this->BlockCount = this->BlockCount | REFTAB_LOCK;

    {
        Ref* ref = this->Head;

        while (ref != nullptr) {
            Ref* next = ref->Next;
            this->DeleteRef(ref);
            ref = next;
        }

        this->Count = 0;
        this->Head = nullptr;
    }

    for (u32 i = 0; i < count; i++) {
        this->InsertString(values[i]);
        delete[] values[i];
    }

    delete[] values;
}

// 0x0ffc8850
bool StringRefTabStringComparator(const char* a, const char* b) {
    if (_strcmpi(a, b) == 0) {
        return false;
    }

    const size_t la = strlen(a) + 1;
    const size_t lb = strlen(b) + 1;

    const size_t m = min(la, lb);

    if (m == 0) {
        return b[0] == NULL;
    }

    for (u32 i = 0; i < m; i++) {
        char ca = a[i];
        char cb = b[i];

        if ('A' <= ca && ca <= 'Z') {
            ca += ' ';
        }

        if ('A' <= cb && cb <= 'Z') {
            cb += ' ';
        }

        if (cb < ca) {
            return true;
        }

        if (ca != cb) {
            return b[i] == NULL;
        }
    }

    return b[m] == NULL;
}

// 0x0ffc88e0
s32 __cdecl StringRefTabStringCompareCallback(void const* a, void const* b) {
    return StringRefTabStringComparator(*(const char**)a, *(const char**)b) ? 1 : -1;
}
