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

#define ZSYSMEM_BUFFER_LENGTH       512

#define ZMEMLINK_CHECKSUM_VALUE     0x87654321
#define ZMEMLINK_SET_MEM_VALUE      0xED

#define ZMEMLINK_CHECKSUM(X, S)     (*(u32*)((size_t)X + (size_t)(S)))

// 0x0ffb1860
ZSysMemBase::ZSysMemBase() {
    this->Init = true;
    this->Alloc = true;
}

// 0x0ffb1880
// 0x0ffb18c0
ZSysMemBase::~ZSysMemBase() {}

// 0x0ffb18d0
void ZSysMem::DisplayStatus(s32 x, s32 y) {
    if (g_pSysInterface == nullptr
        || g_pSysInterface->Unk0x38F1 || g_pSysInterface->DebugOptionsVisibility != 0.0f) {
        u32 allocated, total;
        this->GetProcessStats(&allocated, &total);

        g_pSysInterface->DisplayDebugString(x, y,
            "Process %.3f / %.3f  Internal %.3f / %.3f : %.3f Count %d",
            allocated / 1048576.0f, total / 1048576.0f,
            this->AllocatedSize / 1048576.0f, this->MaxAllocatedSize / 1048576.0f,
            (this->Unk0x1A - this->Unk0x16) / 1048576.0f, this->Allocations + this->LinkAllocations);
    }
}

// 0x0ffb19d0
void ZSysMem::PrintStatus() {
    u32 allocated, total;
    this->GetProcessStats(&allocated, &total);

    g_pSysCom->Log("Z:\\Engine\\System\\_Wintel\\Source\\SysMemWintel.cpp", 36)
        ->LogMessage("Alloc  : Process %.3f/%.3f Internal %.3f/%.3f : Count %.3f",
            allocated / 1048576.0f, total / 1048576.0f,
            this->AllocatedSize / 1048576.0f, this->MaxAllocatedSize / 1048576.0f,
            this->Allocations + this->LinkAllocations);

    g_pSysCom->Log("Z:\\Engine\\System\\_Wintel\\Source\\SysMemWintel.cpp", 37)
        ->LogMessage("Texture: Light %d Texture %d",
            this->Lights->GetCount(), this->Textures->GetCount());

    if (this->Unk0x242 != nullptr) {
        this->Unk0x242->FUN_0ffa3cd0(false);
    }
}

// 0x0ffb1ad0
void ZSysMem::GetProcessStats(u32* allocated, u32* total) {
    *allocated = 0;
    *total = 0;
}

// 0x0ffb1af0
ZSysMem::ZSysMem() {
    this->Unk0x242 = nullptr;

    this->AllocLinks = nullptr;
    this->AllocLinksTail = nullptr;

    this->Allocations = 0;
    this->LinkAllocations = 0;

    this->AllocatedSize = 0;
    this->MaxAllocatedSize = 0;

    this->Unk0x256 = 0;

    g_pSysMem = this;

    this->Textures = new CompareRefTab(256, 0);
    this->Lights = new CompareRefTab(256, 0);
    this->Tab = new AllocRefTab();

    this->Sizes = new u32[this->Tab->GetCapacity() + 1];
    ZeroMemory(this->Sizes, (this->Tab->GetCapacity() + 1) * sizeof(u32));

    this->TabItems = this->Tab->GetItems();

    this->Allocator.Unk0x21B = true;
}

// 0x0ffb1d20
// 0x0ffb1d60
ZSysMem::~ZSysMem() {
    delete[] this->Sizes;
    this->Sizes = nullptr;

    if (this->Tab != nullptr) {
        delete this->Tab;
    }

    this->Tab = nullptr;

    if (this->Textures != nullptr) {
        delete this->Textures;
    }

    this->Textures = nullptr;

    if (this->Lights != nullptr) {
        delete this->Lights;
    }

    this->Lights = nullptr;

    this->AllocCheck();

    g_pSysMem = nullptr;
}

// 0x0ffb1e30
void ZSysMem::Method0x28() {
    // TODO NOT IMPLEMENTED
}

// 0x0ffb1e90
u32 ZSysMem::Method0x4C() {
    // TODO NOT IMPLEMENTED
}

// 0x0ffb1ed0
void ZSysMem::Method0x50() {
    // TODO NOT IMPLEMENTED
}

// 0x0ffb1f00
void ZSysMem::PrintMemoryLink(const char* message, ZMemLink* link) {
    char buffer[ZSYSMEM_BUFFER_LENGTH];

    strcpy(buffer, message);
    strcat(buffer, "File:%-12s Line:%4d, Links: %3d, Size:%6d");

    g_pSysCom->Method0x34(buffer, link->File, link->Line, link->Links, link->Size & ZMEM_SIZE_MASK);
}

// 0x0ffb1fa0
bool ZSysMem::IsMemoryLinkBroken(ZMemLink* link) {
    const u32 current = link->Unk0x10;

    if (link->Size & 0x80000000) { // TODO
        const u32 value = link->Unk0x1C ^ link->Size ^ link->Links ^ link->Line
            ^ (u32)link->File ^ (u32)link->Previous ^ (u32)link->Next;

        link->Unk0x10 = value == 0 ? 1 : value; // TODO
    }

    const bool underrun = link->Unk0x10 != current;

    if (underrun) {
        this->PrintMemoryLink("Memory underrun for ", link);
    }

    const bool overrun =
        ZMEMLINK_CHECKSUM(link->Value, link->Size + ZMEM_SIZE_MASK) != ZMEMLINK_CHECKSUM_VALUE;

    if (overrun) {
        this->PrintMemoryLink("Memory overrun for ", link);
    }

    return underrun || overrun;
}

// 0x0ffb2020
void ZSysMem::AllocCheck() {
    // TODO NOT IMPLEMENTED
}

// 0x0ffb2260
void* ZSysMem::Allocate(size_t size) {
    if (!this->Init) {
        g_pSysCom->Log("Z:\\Engine\\System\\_Wintel\\Source\\SysMemWintel.cpp", 234)
            ->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\System\\_Wintel\\Source\\SysMemWintel.cpp", 234);

        __asm { int 3 }
    }

    ZMemBlock* block = nullptr;

    if (this->Alloc) {
        block = (ZMemBlock*)this->Allocator.Allocate(size + sizeof(ZMemBlock));
        block->Size = size | 0x40000000; // TODO
    }
    else {
        block = (ZMemBlock*)malloc(size + sizeof(ZMemBlock));
        block->Size = size;
    }

    if (block == nullptr) {
        g_pSysCom->Log("Z:\\Engine\\System\\_Wintel\\Source\\SysMemWintel.cpp", 244)
            ->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\System\\_Wintel\\Source\\SysMemWintel.cpp", 244);

        __asm { int 3 }
    }

    block->Unk0x0 = 1; // TODO
    block->Unk0x8 = 0; // TODO

    this->Allocations++;
    this->AllocatedSize += size + sizeof(ZMemBlock);

    if (this->MaxAllocatedSize < this->AllocatedSize) {
        this->MaxAllocatedSize = this->AllocatedSize;
    }

    return block->Value;
}

// 0x0ffb2360
void* ZSysMem::AllocateLinked(size_t size) {
    if (!this->Init) {
        g_pSysCom->Log("Z:\\Engine\\System\\_Wintel\\Source\\SysMemWintel.cpp", 234)
            ->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\System\\_Wintel\\Source\\SysMemWintel.cpp", 234);

        __asm { int 3 }
    }

    ZMemLink* link = nullptr;

    if (this->Alloc) {
        link = (ZMemLink*)this->Allocator.Allocate(size + sizeof(ZMemLink));
        link->Size = size | 0xC0000000; // TODO
    }
    else {
        link = (ZMemLink*)malloc(size + sizeof(ZMemLink));
        link->Size = size | 0x80000000; // TODO
    }

    if (link == nullptr) {
        g_pSysCom->Log("Z:\\Engine\\System\\_Wintel\\Source\\SysMemWintel.cpp", 264)
            ->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\System\\_Wintel\\Source\\SysMemWintel.cpp", 264);

        __asm { int 3 }
    }

    link->Next = nullptr;
    link->Previous = this->AllocLinksTail;

    if (this->AllocLinksTail == nullptr) {
        this->AllocLinks = link;
    }
    else {
        this->AllocLinksTail->Next = link;
    }

    this->AllocLinksTail = link;

    memset(&link->Value, ZMEMLINK_SET_MEM_VALUE, size);
    ZMEMLINK_CHECKSUM(link->Value, size) = ZMEMLINK_CHECKSUM_VALUE;

    link->Line = 0;
    link->File = nullptr;

    this->LinkAllocations++;
    this->AllocatedSize = this->AllocatedSize + size + sizeof(ZMemLink) - sizeof(void*);

    if (this->MaxAllocatedSize < this->AllocatedSize) {
        this->MaxAllocatedSize = this->AllocatedSize;
    }

    link->Links = 1;
    link->Unk0x1C = 0;

    if (link->Size & 0x80000000) { // TODO
        const u32 value = link->Unk0x1C ^ link->Size ^ link->Links
            ^ link->Line ^ (u32)link->File ^ (u32)link->Previous ^ (u32)link->Next;

        link->Unk0x10 = value == 0 ? 1 : value; // TODO
    }

    ZMemLink* previous = link->Previous;

    if (previous != nullptr && (previous->Size & 0x80000000)) { // TODO
        const u32 value = previous->Unk0x1C ^ previous->Size ^ previous->Links
            ^ previous->Line ^ (u32)previous->File ^ (u32)previous->Previous ^ (u32)previous->Next;

        previous->Unk0x10 = value == 0 ? 1 : value; // TODO
    }

    return &link->Value;
}

// 0x0ffb2cb0
void ZSysMem::Method0x18() {
    while (this->AllocLinks != nullptr) {
        this->Delete(&this->AllocLinks->Value);
    }
}

// 0x0ffb2f10
u32 ZSysMem::Method0x54() {
    u32 result = 0;

    RefLink link;
    if (this->Lights != nullptr) {
        this->Lights->GetStart(&link);

        u32 item = this->Lights->GetNextKey(&link); // TODO

        while (link.Next != nullptr) {
            // TODO NOT IMPLEMENTED

            item = this->Lights->GetNextKey(&link);
        }
    }

    return result;
}

// 0x0ffb2e50
u32 ZSysMem::Method0x54() {
    u32 result = 0;

    RefLink link;
    if (this->Textures != nullptr) {
        this->Textures->GetStart(&link);

        u32 item = this->Textures->GetNextKey(&link); // TODO

        while (link.Next != nullptr) {
            // TODO NOT IMPLEMENTED

            item = this->Textures->GetNextKey(&link);
        }
    }

    return result;
}

// 0x0ffc74e0
void ZSysMemBase::Method0x1C() {}

// 0x0ffc74e0
void ZSysMemBase::Method0x20() {}
