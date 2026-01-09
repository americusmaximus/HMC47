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
#include "LinkSortRefTab.hxx"

#define LINKSORTREFTAB_KEYADDR(X)       \
            ((RefKeyValue*)((size_t)X + 2 * sizeof(void*) - this->BlockSize * 4))

#define LINKSORTREFTAB_GETSORTVALUE(X)  \
            (*(f32*)((size_t)X + this->BlockSize * 4 - 2 * sizeof(void*) - 1 * sizeof(f32)))

#define LINKSORTREFTAB_SETSORTVALUE(X)  \
            (*(f32*)((size_t)X + this->BlockSize * 4 - 2 * sizeof(void*) - 1 * sizeof(f32) - 1 * sizeof(u32)))

#define LINKSORTREFTAB_KEYVALUEADDR(X)  \
            ((RefKeyValue*)((size_t)X - sizeof(u32)))

// 0x0ffc0940
// 0x0ffd411c
LinkSortRefTab::LinkSortRefTab(u32 count, u32 size) : LinkRefTab(count, size) {
    if (count < 1) {
        count = 1;
    }

    this->BlockCount = count;
    this->BlockSize = size + 4;
    this->Count = 0;
    this->MaxBlockSize = (size + 4) * count;

    this->Links = nullptr;
    this->Next = nullptr;
    this->Previous = nullptr;

    this->Current = nullptr;
}

// 0x0ffc0990
// 0x0ffc09d0
LinkSortRefTab::~LinkSortRefTab() { }

// 0x0ffc0a60
void LinkSortRefTab::RemoveKeyValue(RefKeyValue* kv) {
    if (this->Current == kv) {
        this->Current = nullptr;
    }

    LinkRefTab::RemoveKeyValue(kv);
}

// 0x0ffc0a80
void LinkSortRefTab::Remove(RefLink* link) {
    if (this->Current == this->GetCurrent(link)) {
        this->Current = nullptr;
    }

    LinkRefTab::Remove(link);
}

// 0x0ffc0ab0
void* LinkSortRefTab::InsertSort(u32 key, f32 order, LinkSortRefTabInsert mode) {
    RefKeyValue* current = this->Current;

    if (current == nullptr) {
        if (this->Next == nullptr) {
            // The collection is empty.

            // -----------------
            // |   |   |   |   |
            // -----------------

            // Insert the key at the first (last possition).

            // -----------------
            // | X |   |   |   |
            // -----------------

            void* result = this->InsertAtEnd(key);
            LINKSORTREFTAB_SETSORTVALUE(result) = order;
            this->Current = LINKSORTREFTAB_KEYVALUEADDR(result);
            return result;
        }

        // When the collection has elements, but the current pointer is not set,
        // then iterate through the collection and find the element with sort order greater or equal
        // to the sort order needed.

        for (current = LINKSORTREFTAB_KEYADDR(this->Next);
            current != nullptr; current = this->GetNextValue(current)) {
            if (order <= LINKSORTREFTAB_GETSORTVALUE(current)) {
                break;
            }
        }

        // The search can result in two cases:

        // Case 1. The current sort order is less or equal to one present in the collection:

        // -----------------------
        // | .1 | .5 | 2.0 | 3.0 |
        // -----------------------
        //              ^
        //              | <- 2.0

        // When the element found then the current pointer points to the equal or greater sort value element.
        // Then it is needed to rocess inser mode, se below...

        // Case 2. The current sort order is the greatest in the collection:

        // -------------------
        // | .1 | .5 |   |   |
        // -------------------
        //             ^
        //             | <- 2.0

        // In this case the element has to be added as the last element to the collection.
        if (current == nullptr) {
            void* result = this->InsertAtEnd(key);
            LINKSORTREFTAB_SETSORTVALUE(result) = order;
            this->Current = LINKSORTREFTAB_KEYVALUEADDR(result);
            return result;
        }
    }

    // At this point the current pointer can either point to the equal or greater sort value element
    // found above, or to the last used element inserted into the collection, which is sorted.

    // Given that the collection is sorted, the current pointer can be used right away to determine
    // whether to iterate left (to the beginning), or to the right (end), based on its own sort value.

    if (order <= LINKSORTREFTAB_GETSORTVALUE(current)) {
        // Iterate to the left if the current cursor sort value is greater or equal to the sort order.

        // -----------------------------------------
        // | .1 | .5 | 2.0 | 2.0 | 3.0 | 3.5 | 3.6 |
        // -----------------------------------------
        //                          ^
        //                          | <- 2.0 <<<

        for (; current != nullptr; current = this->GetPreviousValue(current)) {
            if (mode == LINKSORTREFTAB_AFTER) {
                if (LINKSORTREFTAB_GETSORTVALUE(current) <= order) {
                    break;
                }
            }
            else {
                if (LINKSORTREFTAB_GETSORTVALUE(current) < order) {
                    break;
                }
            }
        }

        // So that the last element matching element is found, or the beginning of the list.

        // -----------------------------------------
        // | .1 | .5 | 2.0 | 2.0 | 3.0 | 3.5 | 3.6 |
        // -----------------------------------------
        //                    ^
        //                    | <- 2.0

        // Insert the element at the start if the start of the collection reached.

        if (current == nullptr) {
            void* result = this->InsertAtStart(key);
            LINKSORTREFTAB_SETSORTVALUE(result) = order;
            this->Current = LINKSORTREFTAB_KEYVALUEADDR(result);
            return result;
        }
    }

    if (LINKSORTREFTAB_GETSORTVALUE(current) <= order) {
        // Iterate to the right if the current cursor sort value is less than the sort order.

        // -----------------------------------------
        // | .1 | .5 | 2.0 | 2.0 | 3.0 | 3.5 | 3.6 |
        // -----------------------------------------
        //    ^
        //    | <- 2.0 >>>

        for (; current != nullptr; current = this->GetNextValue(current)) {
            if (order <= LINKSORTREFTAB_GETSORTVALUE(current)) {
                break;
            }
        }

        // So that the first element matching element is found, or the end of the list.

        // -----------------------------------------
        // | .1 | .5 | 2.0 | 2.0 | 3.0 | 3.5 | 3.6 |
        // -----------------------------------------
        //              ^
        //              | <- 2.0

        // Insert the element at the end if the end of the collection reached.

        if (current == nullptr) {
            void* result = this->InsertAtEnd(key);
            LINKSORTREFTAB_SETSORTVALUE(result) = order;
            this->Current = LINKSORTREFTAB_KEYVALUEADDR(result);
            return result;
        }
    }

    if (mode == LINKSORTREFTAB_AFTER) {
        // In this mode insertion of new values with the same order
        // happens at the end of the sequence with the same value.

        // Iterate through the collection until the different sort value or the end of collection.

        for (; current != nullptr; current = this->GetNextValue(current)) {
            if (order < LINKSORTREFTAB_GETSORTVALUE(current)) {
                break;
            }
        }

        // -----------------------------------------
        // | .1 | .5 | 2.0 | 2.0 | 3.0 | 3.5 | 3.6 |
        // -----------------------------------------
        //                          ^
        //                          | <- 2.0

        if (current == nullptr) {
            void* result = this->InsertAtEnd(key);
            LINKSORTREFTAB_SETSORTVALUE(result) = order;
            this->Current = LINKSORTREFTAB_KEYVALUEADDR(result);
            return result;
        }
    }

    void* result = this->InsertBefore(current, key);
    LINKSORTREFTAB_SETSORTVALUE(result) = order;
    this->Current = LINKSORTREFTAB_KEYVALUEADDR(result);
    return result;
}

// 0x0ffc0be0
f32 LinkSortRefTab::GetSortValue(RefKeyValue* kv) {
    return LINKSORTREFTAB_GETSORTVALUE(kv);
}

// 0x0ffc0bf0
void LinkSortRefTab::ReplaceAt(RefKeyValue* kv, f32 order) {
    RefKeyValue* result =
        LINKSORTREFTAB_KEYVALUEADDR(this->InsertSort(kv->Key, order, LINKSORTREFTAB_INSERT));

    CopyMemory(result, kv, (this->BlockSize - 1) * 4);

    this->RemoveKeyValue(kv);
}

// 0x0ffc0c40
void LinkSortRefTab::Clear() {
    this->Current = nullptr;

    LinkRefTab::Clear();
}

// 0x0ffc0c90
void LinkSortRefTab::Clear2() {
    this->Current = nullptr;
}

// 0x0ffc0ca0
void LinkSortRefTab::PrintStatus() {
    RefLink link;

    if (this != nullptr) {
        this->GetStart(&link);
        RefKeyValue* kv = this->GetNext(&link);

        while (kv != nullptr) {
            const f32 order = LINKSORTREFTAB_GETSORTVALUE(kv);

            g_pSysCom->Log("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 1169)
                ->LogMessage("Ref %x Sort Value %f", kv->Key, order);

            kv = this->GetNext(&link);
        }
    }
}
