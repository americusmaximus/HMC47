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

#include <System/ZRenderLoader.hxx>

#pragma pack(push, 1)

class ZRenderLoaderD3D : public ZRenderLoader {
public:
    ZRenderLoaderD3D();

public:
    virtual ~ZRenderLoaderD3D();                                                // 0x0
    virtual void Initialize();                                                  // 0x8
    virtual void Method0x28();                                                  // 0x28
    virtual void InitializeRenderer(HWND window);                               // 0x38
    virtual void Method0x3C(u32* todo);                                         // 0x3C
    virtual void Method0x40();                                                  // 0x40
    virtual void Method0x44();                                                  // 0x44
    virtual void Method0x50();                                                  // 0x50

public:
    void* Unk0x70;                                                              // 0x70
    void* Unk0x74;                                                              // 0x74
};

#pragma pack(pop)

#if defined(_DEBUG) && !defined(_WIN64)
static_assert(sizeof(ZRenderLoaderD3D)  == 0x78,    "ZRenderLoaderD3D size mismatch.");
#endif

extern ZRenderLoaderD3D* g_Loader;
