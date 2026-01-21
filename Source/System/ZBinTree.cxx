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
#include "ZBinTree.hxx"

// 0x0ffc80d0
ZBinTree::ZBinTree() {
    this->Value.Parent = nullptr;
    this->Value.Left = nullptr;
    this->Value.Right = nullptr;
    this->Value.Key = 0;
    this->Value.Color = false;

    this->Null = &this->Value;
    this->Nodes = this->Null;
}

// 0x0ffc7df0
void ZBinTree::InsertNode(ZBinTreeNode* node) {
    node->Left = this->Null;
    node->Right = this->Null;
    node->Parent = this->Null;

    ZBinTreeNode* parent = this->Null;

    if (this->Nodes != this->Null) {
        ZBinTreeNode* candidate = this->Nodes;

        while (candidate != this->Null) {
            parent = candidate;

            candidate = node->Key < parent->Key
                ? parent->Left : parent->Right;
        }
    }

    node->Parent = parent;

    if (this->Null != parent) {
        if (parent->Key <= node->Key) {
            parent->Right = node;
        }
        else {
            parent->Left = node;
        }
    }
    else {
        this->Nodes = node;
    }
}

// 0x0ffc7e60
ZBinTreeNode* ZBinTree::GetMatch(u32 value, ZBinTreeNode* node) {
    if (node == nullptr) {
        node = this->Nodes;
    }

    while (node != this->Null && value != node->Key) {
        node = value < node->Key ? node->Left : node->Right;
    }

    return node == this->Null ? nullptr : node;
}

// 0x0ffc7ea0
ZBinTreeNode* ZBinTree::GetClose(u32 value, ZBinTreeNode* node) {
    if (node == nullptr) {
        node = this->Nodes;

        if (node == nullptr) {
            return nullptr;
        }
    }

    if (value < node->Key) {
        if (node->Left != nullptr) {
            ZBinTreeNode* result = this->GetClose(value, node->Left);

            if (result != nullptr) {
                if (value <= result->Key) {
                    return result;
                }
            }
        }
    }
    else if (node->Key < value) {
        if (node->Right != nullptr) {
            ZBinTreeNode* result = this->GetClose(value, node->Right);

            if (result != nullptr) {
                if (value <= result->Key) {
                    return result;
                }
            }
        }

        return nullptr;
    }

    return node == this->Null ? nullptr : node;
}

// 0x0ffc7f20
ZBinTreeNode* ZBinTree::GetMinimum(ZBinTreeNode* node) {
    ZBinTreeNode* next = node->Left;

    while (next != this->Null) {
        node = next;
        next = next->Left;
    }

    return node;
}

// 0x0ffc7f40
ZBinTreeNode* ZBinTree::GetMaximum(ZBinTreeNode* node) {
    ZBinTreeNode* next = node->Right;

    while (next != this->Null) {
        node = next;
        next = next->Right;
    }

    return node;
}

// 0x0ffc7f60
ZBinTreeNode* ZBinTree::GetSuccessor(ZBinTreeNode* node) {
    if (node->Right == this->Null) {
        ZBinTreeNode* parent = node->Parent;

        while (parent != this->Null && node == parent->Right) {
            node = parent;
            parent = parent->Parent;
        }

        return parent;
    }

    return this->GetMinimum(node->Right);
}

// 0x0ffc7f90
u32 ZBinTree::GetDepth(ZBinTreeNode* node) {
    if (node == nullptr) {
        node = this->Nodes;
    }

    u32 left = 1;
    u32 right = 1;

    if (node->Left != this->Null) {
        left = this->GetDepth(node->Left);
        left++;
    }

    if (node->Right != this->Null) {
        right = this->GetDepth(node->Right);
        right++;
    }

    return max(left, right);
}

// 0x0ffc7fe0
void ZBinTree::Validate(ZBinTreeNode* node) {
    if (node == nullptr) {
        node = this->Nodes;

        if (node == nullptr) {
            g_pSysCom->Log("Z:\\Engine\\ZStdLib\\Source\\BinaryTrees.cpp", 175)
                ->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\ZStdLib\\Source\\BinaryTrees.cpp", 175);

            __asm { int 3 }
        }
    }

    if (node != this->Null) {
        if (node->Left != this->Null) {
            if (node->Left->Parent != node) {
                g_pSysCom->Log("Z:\\Engine\\ZStdLib\\Source\\BinaryTrees.cpp", 179)
                    ->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\ZStdLib\\Source\\BinaryTrees.cpp", 179);

                __asm { int 3 }
            }

            this->Validate(node->Left);
        }

        if (node->Right != this->Null) {
            if (node->Right->Parent != node) {
                g_pSysCom->Log("Z:\\Engine\\ZStdLib\\Source\\BinaryTrees.cpp", 184)
                    ->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\ZStdLib\\Source\\BinaryTrees.cpp", 184);

                __asm { int 3 }
            }

            this->Validate(node->Right);
        }
    }
}

// 0x0ffc8100
void ZBinTree::Insert(ZBinTreeNode* node) {
    this->InsertNode(node);

    node->Color = true;

    if (this->Nodes == node) {
        this->Nodes->Color = false;
        return;
    }

    while (true) {
        ZBinTreeNode* parent = node->Parent;

        if (!parent->Color) {
            this->Nodes->Color = false;
            return;
        }

        ZBinTreeNode* candidate = parent->Parent->Left;

        if (parent == candidate) {
            candidate = parent->Parent->Right;

            if (candidate->Color) {
                parent->Color = false;
                candidate->Color = false;

                node->Parent->Parent->Color = true;
                node = node->Parent->Parent;

                if (node == this->Nodes) {
                    this->Nodes->Color = false;
                    return;
                }

                continue;
            }

            if (node == parent->Right) {
                candidate = parent->Right;
                parent->Right = candidate->Left;

                if (this->Null != candidate->Left) {
                    candidate->Left->Parent = parent;
                }

                candidate->Parent = parent->Parent;

                if (this->Null == parent->Parent) {
                    this->Nodes = candidate;
                }
                else if (parent == parent->Parent->Left) {
                    parent->Parent->Left = candidate;
                }
                else {
                    parent->Parent->Right = candidate;
                }

                candidate->Left = parent;
                parent->Parent = candidate;
                node = parent;
            }

            node->Parent->Color = false;
            node->Parent->Parent->Color = true;

            parent = node->Parent->Parent;
            candidate = parent->Left;
            parent->Left = candidate->Right;

            if (candidate->Right != this->Null) {
                candidate->Right->Parent = parent;
            }

            candidate->Parent = parent->Parent;

            if (this->Null == parent->Parent) {
                this->Nodes = candidate;
                candidate->Right = parent;
            }
            else if (parent == parent->Parent->Right) {
                parent->Parent->Right = candidate;
                candidate->Right = parent;
            }
            else {
                parent->Parent->Left = candidate;
                candidate->Right = parent;
            }

            parent->Parent = candidate;
        }
        else {
            if (candidate->Color != true) {
                if (node == parent->Left) {
                    candidate = parent->Left;
                    parent->Left = candidate->Right;

                    if (candidate->Right != this->Null) {
                        candidate->Right->Parent = parent;
                    }

                    candidate->Parent = parent->Parent;

                    if (this->Null == parent->Parent) {
                        this->Nodes = candidate;
                    }
                    else if (parent == parent->Parent->Right) {
                        parent->Parent->Right = candidate;
                    }
                    else {
                        parent->Parent->Left = candidate;
                    }

                    candidate->Right = parent;
                    parent->Parent = candidate;
                    node = parent;
                }

                node->Parent->Color = false;
                node->Parent->Parent->Color = true;

                parent = node->Parent->Parent;
                candidate = parent->Right;
                parent->Right = candidate->Left;

                if (candidate->Left != this->Null) {
                    candidate->Left->Parent = parent;
                }

                candidate->Parent = parent->Parent;

                if (this->Null == parent->Parent) {
                    this->Nodes = candidate;
                }
                else if (parent == parent->Parent->Left) {
                    parent->Parent->Left = candidate;
                }
                else {
                    parent->Parent->Right = candidate;
                }

                candidate->Left = parent;
                parent->Parent = candidate;

                if (node == this->Nodes) {
                    this->Nodes->Color = false;
                    return;
                }

                continue;
            }

            parent->Color = false;
            candidate->Color = false;
            node->Parent->Parent->Color = true;
            node = node->Parent->Parent;
        }

        if (node == this->Nodes) {
            this->Nodes->Color = false;
            return;
        }
    }
}

// 0x0ffc82a0
void ZBinTree::BalanceNode(ZBinTreeNode* node) {
    if (node == this->Nodes) {
        node->Color = false;
        return;
    }

    while (node != this->Nodes) {
        if (node->Color != false) {
            break;
        }

        ZBinTreeNode* parent = node->Parent;
        ZBinTreeNode* candidate = parent->Left;

        if (node == candidate) {
            candidate = parent->Right;

            if (candidate->Color) {
                candidate->Color = false;
                node->Parent->Color = true;

                parent = node->Parent;
                candidate = parent->Right;
                parent->Right = candidate->Left;

                if (candidate->Left != this->Null) {
                    candidate->Left->Parent = parent;
                }

                candidate->Parent = parent->Parent;

                if (parent->Parent == this->Null) {
                    this->Nodes = candidate;
                }
                else if (parent == parent->Parent->Left) {
                    parent->Parent->Left = candidate;
                }
                else {
                    parent->Parent->Right = candidate;
                }

                candidate->Left = parent;
                parent->Parent = candidate;
                parent = node->Parent;
                candidate = parent->Right;
            }

            if (!candidate->Left->Color && !candidate->Right->Color) {
                candidate->Color = true;
                node = node->Parent;
            }
            else {
                if (!candidate->Right->Color) {
                    candidate->Left->Color = false;

                    parent = candidate->Left;
                    candidate->Color = true;
                    candidate->Left = parent->Right;

                    if (parent->Right != this->Null) {
                        parent->Right->Parent = candidate;
                    }

                    parent->Parent = candidate->Parent;

                    if (candidate->Parent == this->Null) {
                        this->Nodes = parent;
                    }
                    else if (candidate == candidate->Parent->Right) {
                        candidate->Parent->Right = parent;
                    }
                    else {
                        candidate->Parent->Left = parent;
                    }

                    parent->Right = candidate;
                    candidate->Parent = parent;
                    parent = node->Parent;
                    candidate = parent->Right;
                }

                candidate->Color = parent->Color;

                node->Parent->Color = false;
                candidate->Right->Color = false;

                parent = node->Parent;
                candidate = parent->Right;
                parent->Right = candidate->Left;

                if (candidate->Left != this->Null) {
                    candidate->Left->Parent = parent;
                }

                candidate->Parent = parent->Parent;

                if (parent->Parent == this->Null) {
                    this->Nodes = candidate;
                    candidate->Left = parent;
                }
                else if (parent == parent->Parent->Left) {
                    parent->Parent->Left = candidate;
                    candidate->Left = parent;
                }
                else {
                    parent->Parent->Right = candidate;
                    candidate->Left = parent;
                }

                parent->Parent = candidate;
                node = this->Nodes;
            }
        }
        else {
            if (candidate->Color) {
                candidate->Color = false;
                node->Parent->Color = true;

                parent = node->Parent;
                candidate = parent->Left;
                parent->Left = candidate->Right;

                if (candidate->Right != this->Null) {
                    candidate->Right->Parent = parent;
                }

                candidate->Parent = parent->Parent;

                if (parent->Parent == this->Null) {
                    this->Nodes = candidate;
                }
                else if (parent == parent->Parent->Right) {
                    parent->Parent->Right = candidate;
                }
                else {
                    parent->Parent->Left = candidate;
                }

                candidate->Right = parent;
                parent->Parent = candidate;
                parent = node->Parent;
                candidate = parent->Left;
            }

            if (candidate->Right->Color || candidate->Left->Color) {
                if (!candidate->Left->Color) {
                    candidate->Right->Color = false;

                    parent = candidate->Right;
                    candidate->Color = true;
                    candidate->Right = parent->Left;

                    if (parent->Left != this->Null) {
                        parent->Left->Parent = candidate;
                    }

                    parent->Parent = candidate->Parent;

                    if (candidate->Parent == this->Null) {
                        this->Nodes = parent;
                    }
                    else if (candidate == candidate->Parent->Left) {
                        candidate->Parent->Left = parent;
                    }
                    else {
                        candidate->Parent->Right = parent;
                    }

                    parent->Left = candidate;
                    candidate->Parent = parent;
                    parent = node->Parent;
                    candidate = parent->Left;
                }

                candidate->Color = parent->Color;

                node->Parent->Color = false;
                candidate->Left->Color = false;

                parent = node->Parent;
                candidate = parent->Left;
                parent->Left = candidate->Right;

                if (candidate->Right != this->Null) {
                    candidate->Right->Parent = parent;
                }

                candidate->Parent = parent->Parent;

                if (parent->Parent == this->Null) {
                    this->Nodes = candidate;
                }
                else if (parent == parent->Parent->Right) {
                    parent->Parent->Right = candidate;
                }
                else {
                    parent->Parent->Left = candidate;
                }

                candidate->Right = parent;
                parent->Parent = candidate;
                node = this->Nodes;

                continue;
            }

            candidate->Color = true;
            node = node->Parent;
        }
    }

    node->Color = false;
}

// 0x0ffc84f0
ZBinTreeNode* ZBinTree::Remove(ZBinTreeNode* node) {
    ZBinTreeNode* successor = node;

    if (node->Left != this->Null && node->Right != this->Null) {
        successor = this->GetSuccessor(node);
    }

    ZBinTreeNode* candidate = successor->Left;

    if (candidate == this->Null) {
        candidate = successor->Right;
    }

    candidate->Parent = successor->Parent;

    if (successor->Parent == this->Null) {
        this->Nodes = candidate;
    }
    else if (successor == successor->Parent->Left) {
        successor->Parent->Left = candidate;
    }
    else {
        successor->Parent->Right = candidate;
    }

    if (successor != node) {
        node->Key = successor->Key;
        this->CopyValue(node, successor);
    }

    if (!successor->Color) {
        this->BalanceNode(candidate);
    }

    return successor == this->Null ? nullptr : successor;
}

// 0x0ffb1720
void ZBinTree::CopyValue(ZBinTreeNode*, ZBinTreeNode*) {}
