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
#include "StaticRefTab.hxx"
#include "ZBinTree.hxx" // TODO

#define STATICREFTAB_REF(X)     \
            (*(Ref**)((size_t)X + (this->BlockSize - 1) * 4))

#define STATICREFTAB_NEXTREF(X) \
            ((void*)((size_t)X + this->BlockSize * 4))

#define STATICREFTAB_COUNT(X)   (*(u32*)((size_t)((void**)X)[this->BlockSize - 1] + 8))
#define STATICREFTAB_KEY(X)     (*(u32*)X)
#define STATICREFTAB_NEXT(X)    ((void*)((size_t)X + 4))

#define STATICREFTAB_NODE(X)    \
            ((ZBinTreeNode*)((size_t)X + (this->BlockSize - 1) * 4 - 4))

// 0x0ffbf710
// 0x0ffd3fe0
StaticRefTab::StaticRefTab(u32 count, u32 size) : RefTab(count, size + 1) {
    this->Items =
        new RefTab(this->BlockCount & REFTAB_UNLOCK, this->BlockCount & REFTAB_UNLOCK, 1);
}

// 0x0ffbf7e0
// 0x0ffbf820
StaticRefTab::~StaticRefTab() {
    if (this->Items != nullptr) {
        delete this->Items;
    }
}

// 0x0ffbf8b0
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

// 0x0ffbf990
void* StaticRefTab::TryInsert(u32) {
    g_pSysCom->Log("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 616)
        ->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 616);

    __asm { int 3 }
}

// 0x0ffbf9d0
void StaticRefTab::Clear() {
    if (this->Items != nullptr) {
        delete this->Items;
        this->Items = nullptr;
    }

    RefTab::Clear();
}

// 0x0ffbfa20
void StaticRefTab::Clear2() {
    if (this->Items != nullptr) {
        delete this->Items;
        this->Items = nullptr;
    }
}

// 0x0ffbfa50
void StaticRefTab::RemoveKeyValue(RefKeyValue* kv) {
    this->Count--;

    // TODO NOT TESTED

    ZBinTreeNode* node = STATICREFTAB_NODE(kv);

    node->Key--;

    if (node->Key == 0) {
        if (node->Left == nullptr) {
            this->Head = (Ref*)node->Right;
        }
        else {
            node->Left = node->Right;
        }

        if (node->Right != nullptr) {
            node->Right = node->Left;
        }

        if (this->Items != nullptr) {
            RefLink link;

            this->Items->GetStart(&link);
            ZBinTreeNode* result = STATICREFTAB_NODE(this->Items->GetNextKey(&link));

            while (link.Next != nullptr) {
                if (result == node) {
                    this->Items->Remove(&link);
                }

                result = STATICREFTAB_NODE(this->Items->GetNextKey(&link));
            }
        }

        this->DeleteRef(STATICREFTAB_REF(kv));
    }
    else {
        this->Items->Insert(REFTAB_PTR_TO_KEY(kv));
    }
}

// 0x0ffbfb10
bool StaticRefTab::ContainsKey(u32) {
    g_pSysCom->Log("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 668)
        ->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 668);

    __asm { int 3 }
}

// 0x0ffbfb50
void* StaticRefTab::GetByKey(u32) {
    g_pSysCom->Log("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 673)
        ->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 673);

    __asm { int 3 }
}

// 0x0ffbfb90
u32 StaticRefTab::GetKeyByIndex(u32 i) {
    //g_pSysCom->Log("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 678)
//    ->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 678);

    __asm { int 3 }
}

// 0x0ffbfbd0
RefKeyValue* StaticRefTab::GetByIndex(u32) {
    g_pSysCom->Log("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 683)
        ->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 683);

    __asm { int 3 }
}

// 0x0ffbfc10
void StaticRefTab::PrintStatus() {
    g_pSysCom->Log("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 688)
        ->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 688);

    __asm { int 3 }
}

// 0x0ffbfc40
void StaticRefTab::RemoveByKey(u32) {
    g_pSysCom->Log("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 692)
        ->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 692);

    __asm { int 3 }
}

// 0x0ffbfc80
bool StaticRefTab::TryRemoveByKey(u32) {
    g_pSysCom->Log("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 696)
        ->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 696);

    __asm { int 3 }
}

// 0x0ffbfcc0
void StaticRefTab::Remove(RefLink*) {
    g_pSysCom->Log("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 701)
        ->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 701);

    __asm { int 3 }
}

// 0x0ffbfd00
void StaticRefTab::GetStart(RefLink*) {
    g_pSysCom->Log("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 705)
        ->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 705);

    __asm { int 3 }
}

// 0x0ffbfd40
void StaticRefTab::GetEnd(RefLink*) {
    g_pSysCom->Log("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 709)
        ->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 709);

    __asm { int 3 }
}

// 0x0ffbfd80
u32 StaticRefTab::GetNextKey(RefLink*) {
    g_pSysCom->Log("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 713)
        ->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 713);

    __asm { int 3 }
}

// 0x0ffbfdc0
RefKeyValue* StaticRefTab::GetNext(RefLink*) {
    g_pSysCom->Log("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 718)
        ->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 718);

    __asm { int 3 }
}

// 0x0ffbfe00
u32 StaticRefTab::GetPreviousKey(RefLink*) {
    g_pSysCom->Log("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 723)
        ->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 723);

    __asm { int 3 }
}

// 0x0ffbfe40
RefKeyValue* StaticRefTab::GetPrevious(RefLink*) {
    g_pSysCom->Log("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 728)
        ->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 728);

    __asm { int 3 }
}
