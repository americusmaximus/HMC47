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

#include "ZipFileSystem.hxx"

struct ZipNode {
    ZIPLFHV Header;                                                                     // 0x0
    u32 Offset;                                                                         // 0x1A
};

// 0x0ffc2e30
ZipFileSystemCache::ZipFileSystemCache() : Lookup(32) {}

// 0x0ffc30c0
// 0x0ffc3100
ZipFileSystemCache::~ZipFileSystemCache() {
    ZipFileSystemCache::Clear();
}

// 0x0ffc1800
void ZipFileSystemCache::Insert(const char* path, ZIPLFHV* desc, u32 offset) {
    ZipNode* node = new ZipNode();
    CopyMemory(&node->Header, desc, sizeof(ZIPLFHV));
    node->Offset = offset;

    this->Lookup.Insert(path, node);
    this->Items.Insert(REFTAB_PTR_TO_KEY(node));
}

// 0x0ffc1860
bool ZipFileSystemCache::Find(const char* path, ZIPLFHV* desc, u32* offset) {
    ZipNode* node = (ZipNode*)this->Lookup.Get(path);

    if (node != nullptr) {
        *offset = node->Offset;
        CopyMemory(desc, &node->Header, sizeof(ZIPLFHV));
        return true;
    }

    return false;
}

// 0x0ffc18a0
void ZipFileSystemCache::Method0xC() {
    // TODO NOT IMPLEMENTED
}

// 0x0ffc18c0
void ZipFileSystemCache::Clear() {
    RefLink link;

    this->Lookup.Clear();

    this->Items.GetStart(&link);
    ZipNode* node = (ZipNode*)REFTAB_KEY_TO_PTR(this->Items.GetNextKey(&link));

    if (link.Next != nullptr) {
        delete node;
        node = (ZipNode*)REFTAB_KEY_TO_PTR(this->Items.GetNextKey(&link));
    }
}

// 0x0ffd4258
FileSystemCache::FileSystemCache() {}

// 0x0ffc3080
FileSystemCache::~FileSystemCache() {}
