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

#define STATICREFTAB_REF(X)     \
            (*(Ref**)((size_t)X + (this->BlockSize - 1) * 4))

#define STATICREFTAB_NEXTREF(X) \
            ((void*)((size_t)X + this->BlockSize * 4))

#define STATICREFTAB_COUNT(X)   (*(u32*)((size_t)((void**)X)[this->BlockSize - 1] + 8))
#define STATICREFTAB_KEY(X)     (*(u32*)X)
#define STATICREFTAB_NEXT(X)    ((Ref*)((size_t)X + 4))

// 0x0ff85220
// 0x0ff90e48
StaticRefTab::StaticRefTab(u32 count, u32 size) : RefTab(count, size + 1) {
    this->Items =
        new RefTab(this->BlockCount & REFTAB_UNLOCK, this->BlockCount & REFTAB_UNLOCK, 1);
}

// 0x0ff852f0
// 0x0ff85330
StaticRefTab::~StaticRefTab() {
    if (this->Items != nullptr) {
        delete this->Items;
    }
}

// 0x0ff853c0
void* StaticRefTab::Insert(u32 key) {
    this->Count++;

    if (this->Items->GetCount() == 0) {
        Ref* ref = this->NewRef();

        ref->Previous = nullptr;
        ref->Next = this->Head;
        ref->Size = 0;

        if (this->Head != nullptr) {
            this->Head->Previous = ref;
        }

        this->Head = ref;
        void* value = ref->Value;

        for (u32 i = 0; i < (this->BlockCount & REFTAB_UNLOCK); i++) {
            this->Items->Insert(REFTAB_PTR_TO_KEY(value));
            STATICREFTAB_REF(value) = ref;
            value = STATICREFTAB_NEXTREF(value);
        }
    }

    RefLink link;
    void* result = REFTAB_KEY_TO_PTR(key);

    if (this->Items != nullptr) {
        this->Items->GetStart(&link);
        void* value = REFTAB_KEY_TO_PTR(this->Items->GetNextKey(&link));

        if (link.Next != nullptr) {
            this->Items->Remove(&link);
            result = value;
        }
    }

    STATICREFTAB_COUNT(result) = STATICREFTAB_COUNT(result) + 1;
    STATICREFTAB_KEY(result) = key;

    return STATICREFTAB_NEXT(result);
}

// 0x0ff854a0
void* StaticRefTab::TryInsert(u32) {
    g_pSysCom->Log("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 616)
        ->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 616);

    __asm { int 3 }
}

// 0x0ff854e0
void StaticRefTab::Clear() {
    if (this->Items != nullptr) {
        delete this->Items;
        this->Items = nullptr;
    }

    RefTab::Clear();
}

// 0x0ff85530
void StaticRefTab::Clear2() {
    if (this->Items != nullptr) {
        delete this->Items;
        this->Items = nullptr;
    }
}

// 0x0ff85560
void StaticRefTab::RemoveKeyValue(RefKeyValue* kv) {
    this->Count--;

    STATICREFTAB_COUNT(kv) = STATICREFTAB_COUNT(kv) - 1;

    Ref* ref = STATICREFTAB_REF(kv);

    if (STATICREFTAB_COUNT(kv) == 0) {
        if (ref->Previous == nullptr) {
            this->Head = ref->Next;
        }
        else {
            ref->Previous->Next = ref->Next;
        }

        if (ref->Next != nullptr) {
            ref->Next = ref->Previous;
        }

        if (this->Items != nullptr) {
            RefLink link;

            this->Items->GetStart(&link);
            void* result = REFTAB_KEY_TO_PTR(this->Items->GetNextKey(&link));

            while (link.Next != nullptr) {
                if (ref == STATICREFTAB_REF(result)) {
                    this->Items->Remove(&link);
                }

                result = REFTAB_KEY_TO_PTR(this->Items->GetNextKey(&link));
            }
        }

        this->DeleteRef(STATICREFTAB_REF(kv));
    }
    else {
        this->Items->Insert(REFTAB_PTR_TO_KEY(kv));
    }
}

// 0x0ff85620
bool StaticRefTab::ContainsKey(u32) {
    g_pSysCom->Log("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 668)
        ->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 668);

    __asm { int 3 }
}

// 0x0ff85660
void* StaticRefTab::GetByKey(u32) {
    g_pSysCom->Log("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 673)
        ->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 673);

    __asm { int 3 }
}

// 0x0ff856a0
u32 StaticRefTab::GetKeyByIndex(u32 i) {
    g_pSysCom->Log("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 678)
        ->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 678);

    __asm { int 3 }
}

// 0x0ff856e0
RefKeyValue* StaticRefTab::GetByIndex(u32) {
    g_pSysCom->Log("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 683)
        ->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 683);

    __asm { int 3 }
}

// 0x0ff85720
void StaticRefTab::PrintStatus() {
    g_pSysCom->Log("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 688)
        ->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 688);

    __asm { int 3 }
}

// 0x0ff85750
void StaticRefTab::RemoveByKey(u32) {
    g_pSysCom->Log("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 692)
        ->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 692);

    __asm { int 3 }
}

// 0x0ff85790
bool StaticRefTab::TryRemoveByKey(u32) {
    g_pSysCom->Log("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 696)
        ->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 696);

    __asm { int 3 }
}

// 0x0ff857d0
void StaticRefTab::Remove(RefLink*) {
    g_pSysCom->Log("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 701)
        ->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 701);

    __asm { int 3 }
}

// 0x0ff85810
void StaticRefTab::GetStart(RefLink*) {
    g_pSysCom->Log("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 705)
        ->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 705);

    __asm { int 3 }
}

// 0x0ff85850
void StaticRefTab::GetEnd(RefLink*) {
    g_pSysCom->Log("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 709)
        ->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 709);

    __asm { int 3 }
}

// 0x0ff85890
u32 StaticRefTab::GetNextKey(RefLink*) {
    g_pSysCom->Log("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 713)
        ->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 713);

    __asm { int 3 }
}

// 0x0ff858d0
RefKeyValue* StaticRefTab::GetNext(RefLink*) {
    g_pSysCom->Log("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 718)
        ->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 718);

    __asm { int 3 }
}

// 0x0ff85910
u32 StaticRefTab::GetPreviousKey(RefLink*) {
    g_pSysCom->Log("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 723)
        ->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 723);

    __asm { int 3 }
}

// 0x0ff85950
RefKeyValue* StaticRefTab::GetPrevious(RefLink*) {
    g_pSysCom->Log("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 728)
        ->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 728);

    __asm { int 3 }
}
