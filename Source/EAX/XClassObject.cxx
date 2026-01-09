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

#include "XClassObject.hxx"

// 0x10001020
// 0x10006118
XClassObject::XClassObject(LPUNKNOWN handler, XModule* module)
    : Factory((LPUNKNOWN)this, handler, module) {
    this->Module = module;
    this->Count = 0;
    this->Handler = handler;
}

// 0x10001070
HRESULT STDMETHODCALLTYPE XClassObject::QueryInterface(const IID& riid, LPVOID* ppvObj) {
    if (IsEqualIID(IID_IUnknown, riid)) {
        *ppvObj = this;
        this->AddRef();
        return S_OK;
    }

    if (IsEqualIID(IID_IClassFactory, riid)) {
        this->Factory.AddRef();
        *ppvObj = &this->Factory;
        return S_OK;
    }

    return E_NOINTERFACE;
}

// 0x100010d0
ULONG STDMETHODCALLTYPE XClassObject::AddRef() {
    return ++this->Count;
}

// 0x100010e0
ULONG STDMETHODCALLTYPE XClassObject::Release() {
    this->Count--;

    if (this->Count == 0) {
        if (this->Module != nullptr) {
            this->Module->RemoveRef();
        }

        this->Count++;

        if (this != nullptr) {
            delete this;
        }
    }

    return this->Count;
}