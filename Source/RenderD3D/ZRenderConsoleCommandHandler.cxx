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

// 0x0fbb5aa8
ZRenderConsoleCommandHandler::ZRenderConsoleCommandHandler(const char* name, ZRenderBase* instance)
    : ZConsoleCommandHandler(name) {
    this->Instance = instance;
}

// 0x0fba36b0
// 0x0fba36d0
ZRenderConsoleCommandHandler::~ZRenderConsoleCommandHandler() {}

// 0x0fba3630
void ZRenderConsoleCommandHandler::Execute(const char* command) {
    if (_strcmpi(this->Command, "r_font_size") == 0) {
        if (command != nullptr) {
            this->Instance->Method0xBC(this->FUN_0fbb210c());

            return;
        }

        g_pSysCom->Log("Z:\\Engine\\Drawing\\Source\\RenderBase.cpp", 116)
            ->LogMessage("%s = %f", this->Command, this->Instance->Method0xC0());
    }
}
