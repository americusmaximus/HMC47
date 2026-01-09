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

#include "XModule.hxx"

#pragma pack(push, 1)

class XClassFactory {
public:
    XClassFactory(LPUNKNOWN object, LPUNKNOWN handler, XModule* module);

public:
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(const IID& riid, LPVOID* ppvObj);
    virtual ULONG STDMETHODCALLTYPE AddRef();
    virtual ULONG STDMETHODCALLTYPE Release();
    virtual HRESULT STDMETHODCALLTYPE  CreateInstance(LPUNKNOWN pUnkOuter, const IID& riid, LPVOID* ppvObject);
    virtual HRESULT STDMETHODCALLTYPE LockServer(BOOL fLock);

protected:
    ULONG Count;                                                                        // 0x4
    LPUNKNOWN Object;                                                                   // 0x8
    LPUNKNOWN Handler;                                                                  // 0xC
    XModule* Module;                                                                    // 0x10
};

#pragma pack(pop)

#if defined(_DEBUG) && !defined(_WIN64)
static_assert(sizeof(XClassFactory)     == 0x14,    "XClassFactory size mismatch.");
#endif
