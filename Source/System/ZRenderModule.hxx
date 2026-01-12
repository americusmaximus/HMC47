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

#pragma once

#include "LinkRefTab.hxx"
#include "ZTextureManager.hxx"
#include "ZModule.hxx"

#pragma pack(push, 1)

class ZRenderModule : public ZModule {
public:
    ZRenderModule();

public:
    virtual ~ZRenderModule();                                                   // 0x0
    virtual void Method0x10();                                                  // 0x10
    virtual void Method0x14();                                                  // 0x14
    virtual void InitializeRenderer(HWND window);                               // 0x38
    virtual void Method0x3C(u32* todo);                                         // 0x3C
    virtual void Method0x40();                                                  // 0x40
    virtual void Method0x44();                                                  // 0x44
    virtual void Method0x48(s32 count);                                         // 0x48

public:
    u32 Unk0xC;                                                                 // 0xC
    ZTextureManager* Manager;                                                   // 0x10
    s32 Count;                                                                  // 0x14
    RefTab* Unk0x18;                                                            // 0x18
    LinkRefTab* Unk0x1C;                                                        // 0x1C
    u32 Unk0x20[20 /* TODO */];                                                 // 0x20
};

#pragma pack(pop)

#if defined(_DEBUG) && !defined(_WIN64)
static_assert(sizeof(ZRenderModule)     == 0x70,     "ZRenderModule size mismatch.");
#endif
