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
#include "ZConsole.hxx"

// 0x0ffc8900
// 0x0ffd44a4
ZConsoleHandler::ZConsoleHandler() {
    this->Help = new ZHelpConsoleCommand("commands", this);
    this->Nodes = new ZConsoleCommandNode(this->Help);
}

// 0x0ffc8b20
ZConsoleHandler::~ZConsoleHandler() {
    if (this->Help != nullptr) {
        delete this->Help;
    }

    ZConsoleCommandNode* node = this->Nodes;

    while (node != nullptr) {
        ZConsoleCommandNode* previous = node->PreviousMatch;

        while (previous != nullptr) {
            node->PreviousMatch = previous->PreviousMatch;

            g_pSysCom->Log("Z:\\Engine\\ZStdLib\\Source\\ConsoleCommand.cpp", 94)
                ->LogMessage("Removing unremoved command '%s'", previous->Command->Command);

            delete previous;

            node = this->Nodes;
            previous = node->PreviousMatch;
        }

        node = this->Nodes;
        this->Nodes = node->Next;

        g_pSysCom->Log("Z:\\Engine\\ZStdLib\\Source\\ConsoleCommand.cpp", 99)
            ->LogMessage("Removing unremoved command '%s'", node->Command->Command);

        delete node;

        node = this->Nodes;
    }
}

// 0x0ffc8c10
void ZConsoleHandler::Register(ZConsoleCommand* command) {
    ZConsoleCommandNode* root = this->Nodes;
    ZConsoleCommandNode* node = new ZConsoleCommandNode(command);

    if (strcmpi(command->Command, root->Command->Command) >= 0) {
        ZConsoleCommandNode* current = root->Next;

        while (current != nullptr
            && strcmpi(command->Command, current->Command->Command) > 0) {
            root = root->Next;
            current = root->Next;
        }

        if (root->Next != nullptr
            && strcmpi(command->Command, root->Next->Command->Command) == 0) {
            root = root->Next;

            if (root->NextMatch != nullptr) {
                root->NextMatch->PreviousMatch = node;
                node->NextMatch = root->NextMatch;
            }

            node->PreviousMatch = root;
            root->NextMatch = node;
        }
        else {
            node->Next = root->Next;
            node->Previous = root;

            if (root->Next != nullptr) {
                root->Next->Previous = node;
            }

            root->Next = node;
        }
    }
    else {
        node->Next = this->Nodes;
        this->Nodes->Previous = node;
        this->Nodes = node;
    }
}

// 0x0ffc8d00
void ZConsoleHandler::Unregister(ZConsoleCommand* command) {
    ZConsoleCommandNode* root = this->Nodes;

    if (root != nullptr) {
        while (strcmpi(root->Command->Command, command->Command) != 0) {
            root = root->Next;

            if (root == nullptr) {
                return;
            }
        }

        for (; root != nullptr; root = root->NextMatch) {
            if (root->Command == command) {
                if (root == this->Nodes) {
                    this->Nodes = root->Next;
                }

                ZConsoleCommandNode* node = root->Previous;

                if (node != nullptr) {
                    if (root->NextMatch == nullptr) {
                        node->Next = root->Next;
                    }
                    else {
                        root->NextMatch->Previous = node;
                        root->Previous->Next = root->NextMatch;
                    }
                }

                node = root->Next;

                if (node != nullptr) {
                    if (root->NextMatch == nullptr) {
                        node->Previous = root->Previous;
                    }
                    else {
                        root->NextMatch->Next = node;
                        root->Next->Previous = root->NextMatch;
                    }
                }

                if (root->PreviousMatch != nullptr) {
                    root->PreviousMatch->NextMatch = root->NextMatch;
                }

                if (root->NextMatch != nullptr) {
                    root->NextMatch->PreviousMatch = root->PreviousMatch;
                }

                delete root;

                return;
            }
        }

        g_pSysCom->Log("Z:\\Engine\\ZStdLib\\Source\\ConsoleCommand.cpp", 215)
            ->LogMessage("Command %s not registered in list!", command->Command);
    }
}

// 0x0ffc8e00
ZConsoleCommandNode* ZConsoleHandler::Find(const char* command,
    bool direction, bool exact, ZConsoleCommandNode* node) {
    ZConsoleCommandNode* result = node == nullptr
        ? this->Nodes
        : (direction ? node->Next : node->Previous);

    const size_t length = strlen(command) + 1;

    while (result != nullptr) {
        const bool match = exact
            ? _strcmpi(command, result->Command->Command) == 0
            : _strnicmp(command, result->Command->Command, length) == 0;

        if (!match) {
            result = direction ? result->Next : result->Previous;
        }
    }

    return result == nullptr ? node : result;
}

// 0x0ffc8ea0
bool ZConsoleHandler::Execute(const char* command, const char* value) {
    ZConsoleCommandNode* node = this->Find(command, true, true, nullptr);

    if (node == nullptr) {
        return false;
    }

    while (node != nullptr) {
        node->Command->Execute(value);
        node = node->NextMatch;
    }

    return true;
}

// 0x0ffc8ee0
void ZConsoleHandler::PrintStatus(const char*) {
    for (ZConsoleCommandNode* node = this->Nodes; node != nullptr; node->Next) {
        g_pSysCom->Log("Z:\\Engine\\ZStdLib\\Source\\ConsoleCommand.cpp", 284)
            ->LogMessage("%s", node->Command->Command);
    }
}

// 0x0ffc8a90
ZConsoleCommandNode* ZConsoleHandler::GetNodes() {
    return this->Nodes;
}
