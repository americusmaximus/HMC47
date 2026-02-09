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
#define ZMEMLINK_SET_INIT_MEM       0xED
#define ZMEMLINK_SET_FREE_MEM       0xD5

#define ZMEMLINK_CHECKSUM(X, S)     (*(u32*)((size_t)X + (size_t)(S)))

#define ZSYSMEM_GET_BLOCK(X)        ((ZMemBlock*)((size_t)X - sizeof(ZMemBlock)))
#define ZSYSMEM_GET_LINK(X)         ((ZMemLink*)((size_t)X - sizeof(ZMemLink) - sizeof(void*)))

#define ZSYSMEM_GET_COUNT(X)        (*(u32*)((size_t)X - 12))
#define ZSYSMEM_GET_INDEX(X)        (*(u32*)((size_t)X - 4))
#define ZSYSMEM_GET_SIZE(X)         (*(u32*)((size_t)X - 8))

#define ZSYSMEM_ALLOCATOR_MASK      0x40000000
#define ZSYSMEM_MEMLINK_MASK        0x80000000

#define ZSYSMEM_IS_ALLOCATOR(X)     (((size_t)X & ZSYSMEM_ALLOCATOR_MASK))
#define ZSYSMEM_IS_MEMLINK(X)       (((size_t)X & ZSYSMEM_MEMLINK_MASK))

#define ZSYSMEM_INVALID_VALUE       ((void*)(-1))

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
            this->AllocatedSize / 1048576.0f,
            this->MaxAllocatedSize / 1048576.0f,
            (this->Allocator.Capacity - this->Allocator.Allocated) / 1048576.0f,
            this->Allocations + this->LinkAllocations);
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
    this->Indx = new AllocRefTab();

    const u32 capacity = this->Indx->GetCapacity() + 1;

    this->Values = new void*[capacity];
    ZeroMemory(this->Values, capacity * sizeof(void*));

    this->Indexes = this->Indx->GetItems();

    this->Allocator.Unk0x21B = true;
}

// 0x0ffb1d20
// 0x0ffb1d60
ZSysMem::~ZSysMem() {
    delete[] this->Values;
    this->Values = nullptr;

    if (this->Indx != nullptr) {
        delete this->Indx;
    }

    this->Indx = nullptr;

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
void ZSysMem::Index(void* value) {
    const u32 index = this->Indx->Create();
    this->Values[this->Indx->GetCapacity() & index] = value;

    ZMemBlock* block = ZSYSMEM_GET_BLOCK(value);

    block->Index = index;

    if (ZSYSMEM_IS_MEMLINK(block->Size)) {
        ZMemLink* link = ZSYSMEM_GET_LINK(value);

        const u32 value = link->Index ^ link->Size ^ link->Count ^ link->Line
            ^ (u32)link->File ^ (u32)link->Previous ^ (u32)link->Next;

        link->Unk0x10 = value == 0 ? 1 : value; // TODO
    }
}

// 0x0ffb1e90
void* ZSysMem::GetByIndex(u32 index) {
    if (index == 0) {
        return nullptr;
    }

    if (!this->Indx->IsAvailable(index)) {
        return nullptr;
    }

    return this->Values[this->Indx->GetCapacity() & index];
}

// 0x0ffb1ed0
void ZSysMem::Method0x50(void* value) {
    if (value != nullptr) {
        g_pSysInterface->EngineData->Method0x110(value);
    }
}

// 0x0ffb1f00
void ZSysMem::PrintMemoryLink(const char* message, ZMemLink* link) {
    char buffer[ZSYSMEM_BUFFER_LENGTH];

    strcpy(buffer, message);
    strcat(buffer, "File:%-12s Line:%4d, Links: %3d, Size:%6d");

    g_pSysCom->Method0x34(buffer, link->File, link->Line, link->Count, link->Size & ZMEM_SIZE_MASK);
}

// 0x0ffb1fa0
bool ZSysMem::IsMemoryLinkBroken(ZMemLink* link) {
    const u32 current = link->Unk0x10;

    if (ZSYSMEM_IS_MEMLINK(link->Size)) {
        const u32 value = link->Index ^ link->Size ^ link->Count ^ link->Line
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
    ZMemLink* link = this->AllocLinks;

    // TODO NOT IMPLEMENTED
}

// 0x0ffb21c0
void* ZSysMem::SetMemoryLinkDetails(void* value, const char* path, u32 line) {
    const char* name = strrchr(path, '\\');
    name = name == nullptr ? path : &name[1];

    char* str = (char*)malloc(strlen(name) + 1);
    strcpy(str, name);

    ZMemLink* link = ZSYSMEM_GET_LINK(value);

    link->File = str;
    link->Line = line;

    if (ZSYSMEM_IS_ALLOCATOR(link->Size)) {
        const u32 value = link->Index ^ link->Size ^ link->Count
            ^ link->Line ^ (u32)link->File ^ (u32)link->Previous ^ (u32)link->Next;

        link->Unk0x10 = value == 0 ? 1 : value; // TODO
    }

    return value;
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
        block->Size = size | ZSYSMEM_ALLOCATOR_MASK;
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

    block->Count = 1;
    block->Index = 0;

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
        link->Size = size | ZSYSMEM_MEMLINK_MASK | ZSYSMEM_ALLOCATOR_MASK;
    }
    else {
        link = (ZMemLink*)malloc(size + sizeof(ZMemLink));
        link->Size = size | ZSYSMEM_MEMLINK_MASK;
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

    memset(&link->Value, ZMEMLINK_SET_INIT_MEM, size);
    ZMEMLINK_CHECKSUM(link->Value, size) = ZMEMLINK_CHECKSUM_VALUE;

    link->Line = 0;
    link->File = nullptr;

    this->LinkAllocations++;
    this->AllocatedSize = this->AllocatedSize + size + sizeof(ZMemLink) - sizeof(void*);

    if (this->MaxAllocatedSize < this->AllocatedSize) {
        this->MaxAllocatedSize = this->AllocatedSize;
    }

    link->Count = 1;
    link->Index = 0;

    if (ZSYSMEM_IS_MEMLINK(link->Size)) {
        const u32 value = link->Index ^ link->Size ^ link->Count
            ^ link->Line ^ (u32)link->File ^ (u32)link->Previous ^ (u32)link->Next;

        link->Unk0x10 = value == 0 ? 1 : value; // TODO
    }

    ZMemLink* previous = link->Previous;

    if (previous != nullptr && ZSYSMEM_IS_MEMLINK(previous->Size)) {
        const u32 value = previous->Index ^ previous->Size ^ previous->Count
            ^ previous->Line ^ (u32)previous->File ^ (u32)previous->Previous ^ (u32)previous->Next;

        previous->Unk0x10 = value == 0 ? 1 : value; // TODO
    }

    return &link->Value;
}

// 0x0ffb25d0
void ZSysMem::Method0x38(void* value, const char* path, u32 line) {

    // TODO NOT IMPLEMENTED

    // TODO ZMemLink* link = ZSYSMEM_GET_LINK(value); ZSYSMEM_GET_COUNT ???

    if (link->Unk0x10 != 1) { // TODO
        void* pointer = this->AllocateLinked(link->Size & ZMEM_SIZE_MASK);
        this->SetMemoryLinkDetails(pointer, path, line);

        // TODO NOT IMPLEMENTED
    }
}

// 0x0ffb26c0
bool ZSysMem::Delete(void* value) {
    if (!this->Init) {
        g_pSysCom->Log("Z:\\Engine\\System\\_Wintel\\Source\\SysMemWintel.cpp", 304)
            ->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\System\\_Wintel\\Source\\SysMemWintel.cpp", 304);

        __asm { int 3 }
    }

    if (value == nullptr) {
        g_pSysCom->Log("Z:\\Engine\\System\\_Wintel\\Source\\SysMemWintel.cpp", 307)
            ->LogMessage("Tried to Delete 0 Pointer");

        return false;
    }

    if (g_pSysInterface != nullptr) {
        if (g_pSysInterface->EngineData != nullptr) {
            // TODO NOT IMPLEMENTED

            g_pSysCom->Log("Z:\\Engine\\System\\_Wintel\\Source\\SysMemWintel.cpp", 312)
                ->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\System\\_Wintel\\Source\\SysMemWintel.cpp", 312);

            __asm { int 3 }
        }
    }

    bool check = false;

    if (ZSYSMEM_IS_MEMLINK(ZSYSMEM_GET_SIZE(value))) {
        ZMemLink* link = ZSYSMEM_GET_LINK(value);

        check = this->IsMemoryLinkBroken(link);

        if (link->Count >= 1) {
            link->Count--;

            if (ZSYSMEM_IS_MEMLINK(link->Size)) {
                const u32 value = link->Index ^ link->Size ^ link->Count
                    ^ link->Line ^ (u32)link->File ^ (u32)link->Previous ^ (u32)link->Next;

                link->Unk0x10 = value == 0 ? 1 : value; // TODO
            }

            return false;
        }

        ZMemLink* next = link->Next;
        ZMemLink* previous = link->Previous;

        if (previous == nullptr) {
            this->AllocLinks = next;
        }
        else {
            previous->Next = next;
        }

        if (next == nullptr) {
            this->AllocLinksTail = previous;
        }
        else {
            next->Previous = previous;
        }

        link->Next = nullptr;
        link->Previous = nullptr;

        if (previous != nullptr && ZSYSMEM_IS_MEMLINK(previous->Size)) {
            const u32 value = previous->Index ^ previous->Size ^ previous->Count
                ^ previous->Line ^ (u32)previous->File ^ (u32)previous->Previous ^ (u32)previous->Next;

            previous->Unk0x10 = value == 0 ? 1 : value; // TODO
        }

        if (next != nullptr && ZSYSMEM_IS_MEMLINK(next->Size)) {
            const u32 value = next->Index ^ next->Size ^ next->Count
                ^ next->Line ^ (u32)next->File ^ (u32)next->Previous ^ (u32)next->Next;

            next->Unk0x10 = value == 0 ? 1 : value; // TODO
        }

        if (link->Index != 0) {
            if (link->Index == 0) {
                g_pSysMem->Index(value);
            }

            if (this->Indx != nullptr) {
                if (this->Indx->Release(link->Index)) {
                    this->Values[this->Indx->GetCapacity() & link->Index] = ZSYSMEM_INVALID_VALUE;
                }
                else {
                    this->PrintMemoryLink("Couldn't free Ref for ", link);
                }
            }
        }

        memset(value, ZMEMLINK_SET_FREE_MEM, link->Size);

        this->LinkAllocations--;

        if (this->LinkAllocations < 0) {
            g_pSysCom->Log("Z:\\Engine\\System\\_Wintel\\Source\\SysMemWintel.cpp", 342)
                ->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\System\\_Wintel\\Source\\SysMemWintel.cpp", 342);

            __asm { int 3 }
        }

        if (this->Unk0x242 != nullptr) {
            this->Unk0x242->FUN_0ffa3b60(link->File);
        }

        this->AllocatedSize -= link->Size & ZMEM_SIZE_MASK - sizeof(ZMemLink) - sizeof(void*);

        if (link->File != nullptr) {
            free(link->File);
        }

        if (ZSYSMEM_IS_ALLOCATOR(link->Size)) {
            this->Allocator.Release(link);
        }
        else {
            free(link);
        }
    }
    else {
        ZMemBlock* block = ZSYSMEM_GET_BLOCK(value);

        block->Count--;

        if (block->Count != 0) {
            return false;
        }

        if (block->Index != 0) {
            if (block->Index == 0) {
                g_pSysMem->Index(value);
            }

            if (this->Indx != nullptr) {
                if (this->Indx->Release(block->Index)) {
                    this->Values[this->Indx->GetCapacity() & block->Index] = ZSYSMEM_INVALID_VALUE;
                }
                else {
                    check = true;
                }
            }
        }

        this->Allocations--;

        if (this->Allocations < 0) {
            g_pSysCom->Log("Z:\\Engine\\System\\_Wintel\\Source\\SysMemWintel.cpp", 366)
                ->LogMessage("NrAllocs is less than 0");

            check = true;
            this->Allocations = 0;
        }

        this->AllocatedSize -= (block->Size & ZMEM_SIZE_MASK) + sizeof(ZMemBlock);

        if (ZSYSMEM_IS_ALLOCATOR(block->Size)) {
            this->Allocator.Release(block);
        }
        else {
            free(block);
        }
    }

    if (this->AllocatedSize < 0) {
        g_pSysCom->Log("Z:\\Engine\\System\\_Wintel\\Source\\SysMemWintel.cpp", 380)
            ->LogMessage("Error: Total no of bytes used:%d", this->AllocatedSize);

        this->AllocatedSize = 0;
        this->AllocCheck();

        return true;
    }

    if (check) {
        this->AllocCheck();
    }

    return true;
}

// 0x0ffb2cb0
void ZSysMem::DeleteLinks() {
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
