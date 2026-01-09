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

// 0x0fbaee60
// 0x0fbb61a4
RefTab::RefTab(u32 count, u32 size = 0) {
    if (count < 1) {
        count = 1;
    }

    this->MaxBlockSize = (size + 1) * count;
    this->Count = 0;

    this->Head = nullptr;
    this->Tail = nullptr;

    this->BlockSize = size + 1;
    this->BlockCount = count;
}

RefTab::RefTab(u32 count, u32 max, u32 size) {
    this->MaxBlockSize = max;
    this->Count = 0;

    this->Head = nullptr;
    this->Tail = nullptr;

    this->BlockSize = size;
    this->BlockCount = count;
}

// 0x0fbaeea0
// 0x0fbaef10
RefTab::~RefTab() {
    this->BlockCount = this->BlockCount | REFTAB_UNLOCK;

    Ref* ref = this->Head;

    while (ref != nullptr) {
        Ref* next = ref->Next;
        this->DeleteRef(ref);
        ref = next;
    }

    this->Count = 0;
    this->Head = nullptr;
}

// 0x0fbaef50
void* RefTab::Insert(u32 key) {
    if (this->Head == nullptr || this->Tail->Size == this->MaxBlockSize) {
        Ref* ref = this->NewRef();
        ref->Next = nullptr;

        if (this->Head == nullptr) {
            this->Head = ref;
            ref->Previous = nullptr;
            ref = this->Head;
        }
        else {
            this->Tail->Next = ref;
            ref->Previous = this->Tail;
        }

        this->Tail = ref;
        ref->Size = 0;
    }

    RefKeyValue* kv = (RefKeyValue*)&this->Tail->Value[this->Tail->Size];

    kv->Key = key;

    this->Tail->Size += this->BlockSize;
    this->Count++;

    return kv->Value;
}

// 0x0fbaefd0
void* RefTab::TryInsert(u32 key) {
    void* result = this->GetByKey(key);

    if (result == nullptr) {
        return this->Insert(key);
    }

    return result;
}

// 0x0fbaeff0
void RefTab::Clear() {
    this->BlockCount = this->BlockCount | REFTAB_UNLOCK;

    Ref* ref = this->Head;

    while (ref != nullptr) {
        Ref* next = ref->Next;

        this->DeleteRef(ref);

        ref = next;
    }

    this->Count = 0;
    this->Head = nullptr;
}

// 0x0fbaeff0
void RefTab::Clear2() {
    this->BlockCount = this->BlockCount | REFTAB_UNLOCK;

    Ref* ref = this->Head;

    while (ref != nullptr) {
        Ref* next = ref->Next;

        this->DeleteRef(ref);

        ref = next;
    }

    this->Count = 0;
    this->Head = nullptr;
}

// 0x0fbaf030
void RefTab::DeleteRef(Ref* ref) {
    delete[] ref;
}

// 0x0fbaf060
Ref* RefTab::NewRef() {
    return (Ref*)new u8[this->MaxBlockSize * sizeof(u32*) + sizeof(Ref)];
}

// 0x0fbaf090
u32 RefTab::GetCount() {
    return this->Count;
}

// 0x0fbaf0a0
void RefTab::RemoveKeyValue(RefKeyValue* kv) {
    RefLink link;

    if (this != nullptr) {
        this->GetStart(&link);
        RefKeyValue* result = this->GetNext(&link);

        if (result != nullptr) {
            while (true) {
                if (result == kv) {
                    break;
                }

                result = this->GetNext(&link);

                if (result == nullptr) {
                    return;
                }
            }

            this->Remove(&link);
            this->BlockCount = this->BlockCount | REFTAB_UNLOCK;
        }
    }
}

// 0x0fbaf100
bool RefTab::ContainsKey(u32 key) {
    RefLink link;

    if (this != nullptr) {
        this->GetStart(&link);

        u32 result = this->GetNextKey(&link);

        while (link.Next != nullptr) {
            if (result == key) {
                return true;
            }

            result = this->GetNextKey(&link);
        }
    }

    return false;
}

// 0x0fbaf160
void* RefTab::GetByKey(u32 key) {
    RefLink link;

    if (this != nullptr) {
        this->GetStart(&link);

        while (RefKeyValue* kv = this->GetNext(&link)) {
            if (kv->Key == key) {
                return kv->Value;
            }
        }
    }

    return nullptr;
}

// 0x0fbaf1b0
u32 RefTab::GetKeyByIndex(u32 i) {
    if (this->Count <= i) {
        return 0;
    }

    Ref* ref = this->Head;

    for (u32 count = this->BlockCount & REFTAB_LOCK; count <= i; i -= count) {
        ref = ref->Next;
    }

    return *(u32*)(&ref->Value[this->BlockSize * i]);
}

// 0x0fbaf1f0
RefKeyValue* RefTab::GetByIndex(u32 i) {
    if (this->Count <= i) {
        return 0;
    }

    u32 count = this->BlockCount & REFTAB_LOCK;
    Ref* ref = this->Head;

    for (u32 count = this->BlockCount & REFTAB_LOCK; count <= i; i -= count) {
        ref = ref->Next;
    }

    return (RefKeyValue*)&ref->Value[this->BlockSize * i];
}

// 0x0fbaf230
void RefTab::RemoveByKey(u32 key) {
    if (!this->TryRemoveByKey(key)) {
        g_pSysCom->Log("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 290)
            ->LogMessage("WARNING: unable to remove REF %d\n", key);
    }
}

// 0x0fbaf270
bool RefTab::TryRemoveByKey(u32 key) {
    RefLink link;

    if (this != nullptr) {
        this->GetStart(&link);
        u32 value = this->GetNextKey(&link);

        while (link.Next != nullptr) {
            if (value == key) {
                this->Remove(&link);
                this->BlockCount = this->BlockCount | REFTAB_UNLOCK;
                return true;
            }

            value = this->GetNextKey(&link);
        }
    }

    return false;
}

// 0x0fbaf2e0
void RefTab::GetStart(RefLink* link) {
    link->Size = 0;
    link->Direction = REFTAB_TRAVERSE_FORWARD;
    link->Next = this->Head;

    this->BlockCount = this->BlockCount & REFTAB_LOCK;
}

// 0x0fbaf310
void RefTab::GetEnd(RefLink* link) {
    link->Direction = REFTAB_TRAVERSE_BACKWARD;
    link->Next = this->Tail;

    if (this->Tail != nullptr) {
        link->Size = this->Tail->Size;
    }

    this->BlockCount = this->BlockCount & REFTAB_LOCK;
}

// 0x0fbaf340
u32 RefTab::GetNextKey(RefLink* link) {
    RefKeyValue* kv = this->GetNext(link);

    return kv != nullptr ? kv->Key : 0;
}

// 0x0fbaf360
RefKeyValue* RefTab::GetNext(RefLink* link) {
    if (this->BlockCount & REFTAB_UNLOCK) {
        g_pSysCom->Log("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 373)
            ->LogMessage("ERROR: Illegal operation inside REFTAB loop");

        g_pSysCom->Log("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 374)
            ->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 374);

        __asm { int 3 }
    }

    if (link->Size == this->MaxBlockSize) {
        link->Size = 0;
        link->Next = link->Next->Next;
    }

    Ref* next = link->Next;

    if (next == nullptr) {
        return nullptr;
    }

    const u32 size = link->Size;

    if (size == this->Tail->Size && next->Next == nullptr) {
        link->Next = nullptr;
        return nullptr;
    }

    link->Size = this->BlockSize + size;

    return (RefKeyValue*)&link->Next->Value[size];
}

// 0x0fbaf420
u32 RefTab::GetPreviousKey(RefLink* link) {
    RefKeyValue* kv = this->GetPrevious(link);

    return kv != nullptr ? kv->Key : 0;
}

// 0x0fbaf440
RefKeyValue* RefTab::GetPrevious(RefLink* link) {
    if (this->BlockCount & REFTAB_UNLOCK) {
        g_pSysCom->Log("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 424)
            ->LogMessage("ERROR: Illegal operation inside REFTAB loop");

        g_pSysCom->Log("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 425)
            ->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 425);

        __asm { int 3 }
    }

    if (link->Next == nullptr) {
        return nullptr;
    }

    if (link->Size < this->BlockSize) {
        link->Next = link->Next->Previous;
        if (link->Next == nullptr) {
            return nullptr;
        }

        link->Size = this->MaxBlockSize - this->BlockSize;
    }
    else {
        link->Size -= this->BlockSize;
    }

    return (RefKeyValue*)&link->Next->Value[link->Size];
}

// 0x0fbaf4f0
void RefTab::Remove(RefLink* link) {
    this->Count--;

    this->Tail->Size -= this->BlockSize;

    if (link->Direction >= REFTAB_TRAVERSE_FORWARD) {
        if (link->Size < this->BlockSize) {
            link->Size = this->MaxBlockSize - this->BlockSize;
            link->Next = link->Next->Previous;
        }
        else {
            link->Size -= this->BlockSize;
        }
    }

    if (this->Count != 0) {
        void* src = &this->Tail->Value[this->Tail->Size];
        void* dst = &link->Next->Value[link->Size];

        CopyMemory(dst, src, this->BlockSize & REFTAB_COPY);
    }

    Ref* tail = this->Tail;

    if (tail->Size == 0) {
        Ref* previous = tail->Previous;
        this->Tail = previous;

        if (previous != nullptr) {
            if (tail == link->Next) {
                link->Next = previous;
                link->Size = this->MaxBlockSize;
            }

            this->Tail->Next = nullptr;
            this->DeleteRef(tail);

            return;
        }

        this->Head = nullptr;
        link->Next = nullptr;

        if (this->Count != 0) {
            g_pSysCom->LogFatal("RefPik\n");
        }

        this->DeleteRef(tail);
    }
}

// 0x0fbaf5c0
RefKeyValue* RefTab::GetCurrent(RefLink* link) {
    return (RefKeyValue*)&link->Next->Value[link->Size];
}

// 0x0fbaf5d0
void RefTab::PrintStatus() {
    char* buffer = new char[1024 /* 256 */];

    u32 count = 0;
    for (Ref* ref = this->Head; ref != nullptr; ref = ref->Next) {
        count++;
    }

    g_pSysCom->Log("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 518)
        ->LogMessage("REFTAB::PrintStatus Nr %d\n", count);

    for (Ref* ref = this->Head; ref != nullptr; ref = ref->Next) {
        u32* key = (u32*)ref->Value;
        char* ptr = buffer;

        if (ref->Size != 0) {
            for (u32 i = 0; i < ref->Size; i += this->BlockSize) {
                _itoa(*key, ptr, 10);

                const size_t length = strlen(ptr);
                if (length != 0) {
                    ptr[length] = ' ';
                    ptr += length + 1;
                }

                ptr[0] = NULL;

                key = (u32*)((size_t)key + this->BlockSize * sizeof(u32*));
            }

        }

        ptr = buffer;

        g_pSysCom->Log("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 534)
            ->LogMessage("RefBlk %d -> %s\n", ref->Size, buffer);
    }

    delete[] buffer;
}
