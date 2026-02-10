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
#include "ZMalloc.hxx"

#define MAX_BLOCK_SIZE  0x200000

/*
NOTE: The reversed implementation is ugly, but it works. The class definitely needs a face lift.
Additionally, based on the log messages, it looks like there are a few inlined methods,
    which would be great to separate out.
*/

// 0x0ffc7530
// 0x0ffd4408
ZMalloc::ZMalloc() {
    this->Size = 0;
    this->Capacity = 0;

    this->AvailableNodeCount = 0;
    this->AvailableBlockCount = 0;

    this->Lock = false;
}

// 0x0ffc7570
ZMalloc::~ZMalloc() {
    ZBinTreeNode* node = this->Unk0x1FE.Nodes;

    while (node != this->Unk0x1FE.Null && node != nullptr) {
        free(BINTREE_KEY_TO_PTR(node->Key));
        free(this->Unk0x1FE.Remove(node));
    }
}

// 0x0ffc75d0
void* ZMalloc::Allocate(size_t size) {
    if (size == 0) {
        return nullptr;
    }

    const s32 length = (size + 7) & 0xFFFFFFF8;

    if (this->Lock || 0x1FFFF0 < length) {
        s32* value = (s32*)malloc(length + 8);
        value[0] = -length;
        value[1] = -1;
        return (void*)((size_t)value + 8);
    }

    this->Lock = true;

    ZValueTreeNode* node =
        (ZValueTreeNode*)this->Unk0x1E0.GetApproximate(length + 8, nullptr);

    if (node == nullptr) {
        this->Capacity += MAX_BLOCK_SIZE;

        s32* block = nullptr;
        if (this->AvailableBlockCount == 0) {
            block = (s32*)malloc(MAX_BLOCK_SIZE);

            ZBinTreeNode* value = (ZBinTreeNode*)malloc(sizeof(ZBinTreeNode));

            value->Key = BINTREE_PTR_TO_KEY(block);

            this->Unk0x1FE.Insert(value);
        }
        else {
            this->AvailableBlockCount--;
            block = this->AvailableBlocks[this->AvailableBlockCount];
        }

        block[0] = 0x1FFFF0;
        block[1] = 0;

        block[0x7FFFE] = 0;
        block[0x7FFFF] = 0x1FFFF0;

        node = (ZValueTreeNode*)this->Unk0x1E0.GetMatch(block[0], nullptr);

        if (node == nullptr) {
            if (this->AvailableNodeCount == 0) {
                node = (ZValueTreeNode*)malloc(sizeof(ZValueTreeNode));

                ZBinTreeNode* value = (ZBinTreeNode*)malloc(sizeof(ZBinTreeNode));

                value->Key = BINTREE_PTR_TO_KEY(node);

                this->Unk0x1FE.Insert(value);
            }
            else {
                this->AvailableNodeCount--;
                node = this->AvailableNodes[this->AvailableNodeCount];
            }

            node->Key = BINTREE_PTR_TO_KEY(block[0]);
            node->Value = nullptr;

            this->Unk0x1E0.Insert(node);
        }

        block[2] = 0;
        block[3] = (s32)node->Value;

        node->Value = block;

        if (block[3] != 0) {
            *(void**)((size_t)block[3] + 8) = block;
        }
    }

    if (node != nullptr) {
        s32* block = (s32*)node->Value;

        if (length <= block[0]) {
            if (block[0] < length) {
                g_pSysCom->Log("Z:\\Engine\\ZStdLib\\Source\\ZMalloc.cpp", 106)
                    ->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\ZStdLib\\Source\\ZMalloc.cpp", 106);

                __asm { int 3 }
            }

            this->SetValues(block, node);

            const s32* value = (s32*)((size_t)block + 0xC + block[0]);

            if (value[0] != block[0]) {
                g_pSysCom->Log("Z:\\Engine\\ZStdLib\\Source\\ZMalloc.cpp", 118)
                    ->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\ZStdLib\\Source\\ZMalloc.cpp", 118);

                __asm { int 3 }
            }

            if (value[0] < 0) {
                g_pSysCom->Log("Z:\\Engine\\ZStdLib\\Source\\ZMalloc.cpp", 122)
                    ->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\ZStdLib\\Source\\ZMalloc.cpp", 122);

                __asm { int 3 }
            }

            s32 iVar6 = block[0] - length - 8;
            s32 iVar7 = 0;

            if (iVar6 < 8) {
                iVar6 = iVar6 + length + 8;

                if (*(s32*)((size_t)block + iVar6 + 0xC) != iVar6) {
                    g_pSysCom->Log("Z:\\Engine\\ZStdLib\\Source\\ZMalloc.cpp", 156)
                        ->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\ZStdLib\\Source\\ZMalloc.cpp", 156);

                    __asm { int 3 }
                }

                iVar7 = -iVar6;

                *(s32*)((size_t)block + iVar6 + 0xC) = -iVar7;
            }
            else {
                s32* next = (s32*)((size_t)block + length + 8);

                iVar7 = -length;

                next[0] = iVar6;
                next[1] = iVar7;

                node = (ZValueTreeNode*)this->Unk0x1E0.GetMatch(iVar6, nullptr);

                if (node == nullptr) {
                    if (this->AvailableNodeCount == 0) {
                        node = (ZValueTreeNode*)malloc(sizeof(ZValueTreeNode));

                        ZBinTreeNode* value = (ZBinTreeNode*)malloc(sizeof(ZBinTreeNode));

                        value->Key = BINTREE_PTR_TO_KEY(node);

                        this->Unk0x1FE.Insert(value);
                    }
                    else {
                        this->AvailableNodeCount--;
                        node = this->AvailableNodes[this->AvailableNodeCount];
                    }

                    node->Key = BINTREE_PTR_TO_KEY(iVar6);
                    node->Value = nullptr;

                    this->Unk0x1E0.Insert(node);
                }

                next[2] = 0;
                next[3] = *(s32*)((s32)node + 0x11);

                *(s32**)((s32)node + 0x11) = next;

                if (next[3] != 0) {
                    *(s32**)((size_t)next[3] + 8) = next;
                }

                next = (s32*)((size_t)next + (block[0] - length) + 4);

                if (next[0] < 0) {
                    g_pSysCom->Log("Z:\\Engine\\ZStdLib\\Source\\ZMalloc.cpp", 142)
                        ->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\ZStdLib\\Source\\ZMalloc.cpp", 142);

                    __asm { int 3 }
                }

                next[0] = iVar6;
            }

            block[0] = iVar7;

            this->Size -= iVar7;
            this->Lock = false;

            return (void*)((size_t)block + 8);
        }
    }

    g_pSysCom->Log("Z:\\Engine\\ZStdLib\\Source\\ZMalloc.cpp", 367)
        ->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\ZStdLib\\Source\\ZMalloc.cpp", 367);

    __asm { int 3 }
}

// 0x0ffc7970
void ZMalloc::Release(void* value) {
    s32* block = (s32*)((size_t)value - 8);

    if (0 < block[0]) {
        g_pSysCom->Log("Z:\\Engine\\ZStdLib\\Source\\ZMalloc.cpp", 174)
            ->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\ZStdLib\\Source\\ZMalloc.cpp", 174);

        __asm { int 3 }
    }

    if (block[1] == -1) {
        free(block);
    }
    else {
        s32 iVar1 = block[0];
        s32 iVar7 = -iVar1;

        this->Size += iVar1;

        s32 iVar3 = block[1];

        block[0] = iVar7;

        if (iVar3 < 0) {
            iVar3 = -iVar3;
        }

        s32* local_8 = nullptr;

        if (iVar3 != 0) {
            local_8 = (s32*)((size_t)block - 8 - iVar3);
        }

        s32* local_c = (s32*)((size_t)block + iVar7 + 8);

        if (local_c[1] != iVar1) {
            g_pSysCom->Log("Z:\\Engine\\ZStdLib\\Source\\ZMalloc.cpp", 200)
                ->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\ZStdLib\\Source\\ZMalloc.cpp", 200);

            __asm { int 3 }
        }

        if (local_8 != nullptr && local_8[0] != block[1]) {
            g_pSysCom->Log("Z:\\Engine\\ZStdLib\\Source\\ZMalloc.cpp", 208)
                ->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\ZStdLib\\Source\\ZMalloc.cpp", 208);

            __asm { int 3 }
        }

        if (0 < block[1]) {
            if (local_8 == nullptr) {
                g_pSysCom->Log("Z:\\Engine\\ZStdLib\\Source\\ZMalloc.cpp", 216)
                    ->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\ZStdLib\\Source\\ZMalloc.cpp", 216);

                __asm { int 3 }
            }

            if (local_8[0] != block[1]) {
                g_pSysCom->Log("Z:\\Engine\\ZStdLib\\Source\\ZMalloc.cpp", 221)
                    ->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\ZStdLib\\Source\\ZMalloc.cpp", 221);

                __asm { int 3 }
            }

            this->SetValues(local_8, nullptr);

            iVar7 = iVar7 + 8 + local_8[0];
            local_8[0] = iVar7;
            block = local_8;
        }

        local_c[1] = iVar7;

        if (0 < local_c[0]) {
            if (local_c[0] < 0) {
                g_pSysCom->Log("Z:\\Engine\\ZStdLib\\Source\\ZMalloc.cpp", 238)
                    ->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\ZStdLib\\Source\\ZMalloc.cpp", 238);

                __asm { int 3 }
            }

            this->SetValues(local_c, nullptr);

            iVar7 = iVar7 + 8 + local_c[0];
            local_c = (s32*)((size_t)block + iVar7 + 8);
            local_c[1] = iVar7;
        }

        block[0] = iVar7;

        if (block[1] == 0 && local_c[0] == 0) {
            this->Capacity = this->Capacity + -0x10 - iVar7;

            if (this->AvailableBlockCount != 0x10) {
                this->AvailableBlocks[this->AvailableBlockCount] = block;
                this->AvailableBlockCount++;
                return;
            }

            ZBinTreeNode* iVar7 =
                this->Unk0x1FE.GetMatch(BINTREE_PTR_TO_KEY(block), nullptr);

            free(block);

            if (iVar7 == nullptr) {
                g_pSysCom->Log("Z:\\Engine\\ZStdLib\\Source\\ZMalloc.cpp", 56)
                    ->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\ZStdLib\\Source\\ZMalloc.cpp", 56);

                __asm { int 3 }
            }

            free(this->Unk0x1FE.Remove(iVar7));
            return;
        }

        ZValueTreeNode* puVar5 = (ZValueTreeNode*)this->Unk0x1E0.GetMatch(block[0], nullptr);
        if (puVar5 == nullptr) {
            if (this->AvailableNodeCount == 0) {
                puVar5 = (ZValueTreeNode*)malloc(sizeof(ZValueTreeNode));

                ZBinTreeNode* value = (ZBinTreeNode*)malloc(sizeof(ZBinTreeNode));

                value->Key = BINTREE_PTR_TO_KEY(puVar5);

                this->Unk0x1FE.Insert(value);
            }
            else {
                this->AvailableNodeCount--;
                puVar5 = this->AvailableNodes[this->AvailableNodeCount];
            }

            puVar5->Key = BINTREE_PTR_TO_KEY(block[0]);
            puVar5->Value = nullptr;

            this->Unk0x1E0.Insert(puVar5);
        }

        block[2] = 0;
        block[3] = *(s32*)((size_t)puVar5 + 0x11);

        *(s32**)((size_t)puVar5 + 0x11) = block;

        if (block[3] != 0) {
            *(s32**)(block[3] + 8) = block;
        }
    }
}

// 0x0ffc7cd0
void ZMalloc::SetValues(s32* block, ZValueTreeNode* node) {
    if (block[0] < 0) {
        g_pSysCom->Log("Z:\\Engine\\ZStdLib\\Source\\ZMalloc.cpp", 274)
            ->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\ZStdLib\\Source\\ZMalloc.cpp", 274);

        __asm { int 3 }
    }

    if (node == nullptr) {
        node = (ZValueTreeNode*)this->Unk0x1E0.GetMatch(block[0], nullptr);
    }

    if (block[2] == 0) {
        node->Value = (void*)block[3];
    }
    else {
        *(s32*)(block[2] + 0xC) = block[3];
    }

    if (block[3] != 0) {
        *(s32*)(block[3] + 8) = block[2];
    }

    if (node->Value == nullptr) {
        ZValueTreeNode* available = (ZValueTreeNode*)this->Unk0x1E0.Remove(node);

        if (this->AvailableNodeCount == MAX_ZMALLOC_AVAILABLE_BUCKET_COUNT) {
            ZBinTreeNode* match =
                this->Unk0x1FE.GetMatch(BINTREE_PTR_TO_KEY(available), nullptr);

            free(available);

            if (match == nullptr) {
                g_pSysCom->Log("Z:\\Engine\\ZStdLib\\Source\\ZMalloc.cpp", 56)
                    ->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\ZStdLib\\Source\\ZMalloc.cpp", 56);

                __asm { int 3 }
            }

            free(this->Unk0x1FE.Remove(match));

            return;
        }

        this->AvailableNodes[this->AvailableNodeCount] = available;
        this->AvailableNodeCount++;
    }
}
