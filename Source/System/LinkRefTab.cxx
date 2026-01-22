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

#include "LinkRefTab.hxx"
#include "Globals.hxx"

#define LINKREFTAB_PREVREFADDR(X)   \
            ((Ref*)((size_t)X + (size_t)(this->BlockSize * 4 - 2 * sizeof(void*))))

#define LINKREFTAB_NEXTREFADDR(X)   \
            ((Ref*)((size_t)X + (size_t)(this->BlockSize * 4 - 1 * sizeof(void*))))

#define LINKREFTAB_KEYADDR(X)       \
            ((RefKeyValue*)((size_t)X + (size_t)(2 * sizeof(void*) - this->BlockSize * 4)))

#define LINKREFTAB_KEYVALUEADDR(X)  \
            ((RefKeyValue*)((size_t)X - sizeof(u32)))

// 0x0ffc0050
// 0x0ffd40a8
LinkRefTab::LinkRefTab(u32 count, u32 size) : RefTab(count, size) {
    if (count < 1) {
        count = 1;
    }

    this->BlockCount = count;
    this->BlockSize = size + 3;
    this->MaxBlockSize = (size + 3) * count;

    this->Links = nullptr;
    this->Next = nullptr;
    this->Previous = nullptr;
}

// 0x0ffc00a0
// 0x0ffc00e0
LinkRefTab::~LinkRefTab() {
    if (this->Links = nullptr) {
        delete this->Links;
    }

    this->Links = nullptr;
    this->Next = nullptr;
    this->Previous = nullptr;
}

// 0x0ffc0170
u32 LinkRefTab::GetKeyByIndex(u32 i) {
    RefLink link;

    if (this != nullptr) {
        this->GetStart(&link);
        u32 key = this->GetNextKey(&link);

        while (link.Next != nullptr) {
            if (i == 0) {
                return key;
            }

            key = this->GetNextKey(&link);
            i--;
        }
    }

    return 0;
}

// 0x0ffc01d0
RefKeyValue* LinkRefTab::GetByIndex(u32 i) {
    RefLink link;

    if (this != nullptr) {
        this->GetStart(&link);
        RefKeyValue* result = this->GetNext(&link);

        if (result != nullptr) {
            if (i == 0) {
                return result;
            }

            result = this->GetNext(&link);
            i--;
        }
    }

    return nullptr;
}

// 0x0ffc0220
u32 LinkRefTab::GetCount() {
    if (this->Links == nullptr) {
        return this->Count;
    }

    return this->Count - this->Links->GetCount();
}

// 0x0ffc0240
void LinkRefTab::Clear() {
    if (this->Links != nullptr) {
        delete this->Links;
    }

    this->Links = nullptr;
    this->Next = nullptr;
    this->Previous = nullptr;

    RefTab::Clear();
}

// 0x0ffc0290
void LinkRefTab::Clear2() {
    if (this->Links != nullptr) {
        delete this->Links;
    }

    this->Links = nullptr;
    this->Next = nullptr;
    this->Previous = nullptr;
}

// 0x0ffc02c0
void* LinkRefTab::InsertBefore(RefKeyValue* point, u32 key) {
    RefKeyValue* kv = nullptr;

    if (this->Links == nullptr) {
        kv = LINKREFTAB_KEYVALUEADDR(RefTab::Insert(key));
    }
    else {
        RefLink link;
        this->Links->GetStart(&link);
        const u32 result = this->Links->GetNextKey(&link);

        kv = (RefKeyValue*)REFTAB_KEY_TO_PTR(result);

        if (link.Next != nullptr) {
            this->Links->Remove(&link);
            kv = LINKREFTAB_KEYADDR(result);
        }

        kv->Key = key;

        if (this->Links->GetCount() == 0) {
            delete this->Links;
            this->Links = nullptr;
        }
    }

    Ref* ref = LINKREFTAB_PREVREFADDR(kv);

    ref->Previous = nullptr;
    ref->Next = nullptr;

    Ref* ptr = LINKREFTAB_PREVREFADDR(point);

    if (ptr == nullptr) {
        ref->Previous = this->Previous;
        ref->Next = nullptr;

        if (this->Previous == nullptr) {
            this->Next = ref;
            this->Previous = ref;
        }
        else {
            this->Previous->Next = ref;
            this->Previous = ref;
        }

        return kv->Value;
    }

    Ref* prev = ptr->Previous;
    ref->Previous = prev;
    ref->Next = ptr;

    if (prev == nullptr) {
        this->Next = ref;
        ptr->Previous = ref;
    }
    else {
        prev->Next = ref;
        ptr->Previous = ref;
    }

    return kv->Value;
}

// 0x0ffc03d0
void* LinkRefTab::Insert(u32 key) {
    RefKeyValue* kv = nullptr;

    if (this->Links == nullptr) {
        kv = LINKREFTAB_KEYVALUEADDR(RefTab::Insert(key));
    }
    else {
        RefLink link;
        this->Links->GetStart(&link);
        const u32 result = this->Links->GetNextKey(&link);

        kv = (RefKeyValue*)REFTAB_KEY_TO_PTR(result);

        if (link.Next != nullptr) {
            this->Links->Remove(&link);
            kv = LINKREFTAB_KEYADDR(result);
        }

        kv->Key = key;

        if (this->Links->GetCount() == 0) {
            delete this->Links;
            this->Links = nullptr;
        }
    }

    Ref* ref = LINKREFTAB_PREVREFADDR(kv);

    ref->Previous = this->Previous;
    ref->Next = nullptr;

    if (this->Previous == nullptr) {
        this->Next = ref;
        this->Previous = ref;
    }
    else {
        this->Previous->Next = ref;
        this->Previous = ref;
    }

    return kv->Value;
}

// 0x0ffc04a0
void* LinkRefTab::InsertAtEnd(u32 key) {
    return this->Insert(key);
}

// 0x0ffc04b0
void* LinkRefTab::InsertAtStart(u32 key) {
    RefKeyValue* kv = nullptr;

    if (this->Links == nullptr) {
        kv = LINKREFTAB_KEYVALUEADDR(RefTab::Insert(key));
    }
    else {
        RefLink link;
        this->Links->GetStart(&link);
        const u32 result = this->Links->GetNextKey(&link);

        kv = (RefKeyValue*)REFTAB_KEY_TO_PTR(result);

        if (link.Next != nullptr) {
            this->Links->Remove(&link);
            kv = LINKREFTAB_KEYADDR(result);
        }

        kv->Key = key;

        if (this->Links->GetCount() == 0) {
            delete this->Links;
            this->Links = nullptr;
        }
    }

    Ref* ref = LINKREFTAB_PREVREFADDR(kv);

    ref->Next = this->Next;
    ref->Previous = nullptr;

    if (this->Next == nullptr) {
        this->Previous = ref;
        this->Next = ref;
    }
    else {
        this->Next->Previous = ref;
        this->Next = ref;
    }

    return kv->Value;
}

// 0x0ffc0580
RefKeyValue* LinkRefTab::GetPreviousValue(RefKeyValue* kv) {
    Ref* ref = LINKREFTAB_PREVREFADDR(kv);

    return ref->Previous == nullptr ? nullptr : LINKREFTAB_KEYADDR(ref->Previous);
}

// 0x0ffc05b0
RefKeyValue* LinkRefTab::GetNextValue(RefKeyValue* kv) {
    Ref* ref = LINKREFTAB_NEXTREFADDR(kv);

    return ref->Previous == nullptr ? nullptr : LINKREFTAB_KEYADDR(ref->Previous);
}

// 0x0ffc05e0
void LinkRefTab::GetStart(RefLink* link) {
    link->Size = 0;
    link->Direction = REFTAB_TRAVERSE_FORWARD;
    link->Next = nullptr;

    this->BlockCount = this->BlockCount & REFTAB_UNLOCK;
}

// 0x0ffc0600
void LinkRefTab::GetEnd(RefLink* link) {
    link->Size = 0;
    link->Direction = REFTAB_TRAVERSE_BACKWARD;
    link->Next = nullptr;

    this->BlockCount = this->BlockCount & REFTAB_UNLOCK;
}

// 0x0ffc0620
RefKeyValue* LinkRefTab::GetNext(RefLink* link) {
    if (this->BlockCount & REFTAB_LOCK) {
        g_pSysCom->Log("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 1007)
            ->LogMessage("ERROR: Illegal operation inside LINKREFTAB loop");
    }

    if (link->Next == nullptr) {
        link->Next = this->Next;
    }
    else {
        link->Next = link->Next->Next;
    }

    return link->Next == nullptr ? nullptr : LINKREFTAB_KEYADDR(link->Next);
}

// 0x0ffc0690
RefKeyValue* LinkRefTab::GetPrevious(RefLink* link) {
    if (this->BlockCount & REFTAB_LOCK) {
        g_pSysCom->Log("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 1025)
            ->LogMessage("ERROR: Illegal operation inside LINKREFTAB loop");
    }

    if (link->Next == nullptr) {
        link->Next = this->Previous;
    }
    else {
        link->Next = link->Next->Previous;
    }

    return link->Next == nullptr ? nullptr : LINKREFTAB_KEYADDR(link->Next);
}

// 0x0ffc06f0
void LinkRefTab::RemoveKeyValue(RefKeyValue* kv) {
    Ref* ref = LINKREFTAB_PREVREFADDR(kv);

    if (this->Links == nullptr) {
        this->Links = new RefTab(8, 8, 1);
    }

    this->Links->Insert(REFTAB_PTR_TO_KEY(ref));

    if (ref->Previous == nullptr) {
        this->Next = ref->Next;
    }
    else {
        ref->Previous->Next = ref->Next;
    }

    if (ref->Next == nullptr) {
        this->Previous = ref->Previous;
    }
    else {
        this->Next->Previous = ref->Previous;
    }

    ref->Next = nullptr;
    ref->Previous = nullptr;

    this->BlockCount = this->BlockCount | REFTAB_LOCK;
}

// 0x0ffc07a0
void LinkRefTab::Remove(RefLink* link) {
    Ref* ref = link->Next;

    if (ref == nullptr) {
        g_pSysCom->LogFatal("Dit Svin\n");
    }

    if (this->Links == nullptr) {
        this->Links = new RefTab(8, 8, 1);
    }

    this->Links->Insert(REFTAB_PTR_TO_KEY(ref));

    if (link->Direction < REFTAB_TRAVERSE_FORWARD) {
        link->Next = ref->Next;
    }
    else {
        link->Next = ref->Previous;
    }

    if (ref->Previous == nullptr) {
        this->Next = ref->Next;
    }
    else {
        ref->Previous->Next = ref->Next;
    }

    if (ref->Next != nullptr) {
        ref->Next->Previous = ref->Previous;
    }
    else {
        this->Previous = ref->Previous;
    }

    ref->Next = nullptr;
    ref->Previous = nullptr;
}

// 0x0ffc0870
RefKeyValue* LinkRefTab::GetCurrent(RefLink* link) {
    return link->Next == nullptr ? nullptr : LINKREFTAB_KEYADDR(link->Next);
}

// 0x0ffc0890
void LinkRefTab::PrintStatus() {
    g_pSysCom->Log("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 1070)
        ->LogMessage("Nr Ele %d NrLinks %d\n", this->Count, this->GetCount());

    if (this != nullptr) {
        RefLink link;

        this->GetStart(&link);
        u32 key = this->GetNextKey(&link);

        while (link.Next != nullptr) {
            g_pSysCom->Log("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 1074)
                ->LogMessage("Ref %d\n", key);

            key = this->GetNextKey(&link);
        }
    }
}
