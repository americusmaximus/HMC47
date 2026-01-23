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
#include "FastLookup.hxx"
#include "ZString.hxx"

#define FASTLOOKUP_REFKEYVALUE_ITEM(X)  ((FastLookupItem*)((size_t)X - 4))

// 0x0ffbe420
FastLookup::FastLookup(u32 count) {
    this->Nodes = new ZValueTree(count);
}

// 0x0ffbe490
FastLookup::~FastLookup() {
    this->Clear();

    if (this->Nodes != nullptr) {
        delete this->Nodes;
    }
}

// 0x0ffbe4d0
// 0x0ffbe5a0
void FastLookup::Clear() {
    RefLink link;
    ZValueTreeNode* node = (ZValueTreeNode*)this->Nodes->Nodes;

    while (node != this->Nodes->Null && node != nullptr) {
        LinkRefTab* items = (LinkRefTab*)node->Value;

        if (items == nullptr) {
            g_pSysCom->Log("Z:\\Engine\\ZStdLib\\Source\\FastLookup.cpp", 340)
                ->LogMessage("INT3 in %s at line %d",
                    "Z:\\Engine\\ZStdLib\\Source\\FastLookup.cpp", 340);

            __asm {int 3}
        }

        items->GetStart(&link);
        FastLookupItem* item = (FastLookupItem*)items->GetNext(&link);

        while (item != nullptr) {
            if (item->Taken) {
                delete item->Item;
            }

            item = (FastLookupItem*)items->GetNext(&link);
        }

        delete items;

        this->Nodes->Remove(node);

        node = (ZValueTreeNode*)this->Nodes->Nodes;
    }
}

// 0x0ffbe5b0
void FastLookup::Insert(const char* value, void* content) {
    const char* input = value == nullptr ? "" : value;

    const u32 length = strlen(input);
    char* string = new char[length + 1];

    strcpy(string, input);

    for (u32 i = 0; i <= length; i++) {
        const char c = string[i];
        if ('A' <= c && c <= 'Z') {
            string[i] = c + ' '; // To Lower
        }
    }

    u32 hash = length;

    {
        const u32 size = length / 4;
        const u32* values = (u32*)string;

        for (u32 i = 0; i < size; i++) {
            hash += values[i];
        }
    }

    FastLookupItem* item = nullptr;
    LinkRefTab* items = (LinkRefTab*)this->Nodes->GetItemValue(hash);

    if (items == nullptr) {
        LinkRefTab* links = new LinkRefTab(128, 3);
        this->Nodes->InsertItem(hash, links);
        item = FASTLOOKUP_REFKEYVALUE_ITEM(links->Insert(0));
    }
    else {
        item = this->Match(items, string, length);

        if (item == nullptr) {
            item = FASTLOOKUP_REFKEYVALUE_ITEM(items->Insert(0));
        }
        else if (item->Taken) {
            delete[] item->Item;
            item->Item = nullptr;
        }
    }

    item->Length = length;
    item->Item = string;
    item->Value = content;
    item->Taken = true;
}

// 0x0ffbe830
FastLookupItem* FastLookup::Match(LinkRefTab* links, const char* value, u32 length) {
    RefLink link;

    if (links != nullptr) {
        links->GetStart(&link);
        FastLookupItem* item = (FastLookupItem*)links->GetNext(&link);

        while (item != nullptr) {
            if (item->Length == length) {
                if (memcmp(item->Value, value, length) == 0) {
                    return item;
                }
            }

            item = (FastLookupItem*)links->GetNext(&link);
        }
    }

    return nullptr;
}

// 0x0ffbeb80
void* FastLookup::Get(const char* value) {
    const char* input = value == nullptr ? "" : value;

    const u32 length = strlen(input);
    char* string = new char[length + 1];

    strcpy(string, input);

    for (u32 i = 0; i <= length; i++) {
        const char c = string[i];
        if ('A' <= c && c <= 'Z') {
            string[i] = c + ' '; // To Lower
        }
    }
    // End TODO

    u32 hash = length;

    {
        const u32 size = length / 4;
        const u32* values = (u32*)string;

        for (u32 i = 0; i < size; i++) {
            hash += values[i];
        }
    }

    FastLookupItem* item = nullptr;
    LinkRefTab* items = (LinkRefTab*)this->Nodes->GetItemValue(hash);

    if (items != nullptr) {
        RefLink link;
        items->GetStart(&link);
        FastLookupItem* item = (FastLookupItem*)items->GetNext(&link);

        while (item != nullptr) {
            if (item->Length == length) {
                if (memcmp(item->Item, string, length) == 0) {
                    return item->Value;
                }
            }


            item = (FastLookupItem*)items->GetNext(&link);
        }
    }

    return nullptr;
}
