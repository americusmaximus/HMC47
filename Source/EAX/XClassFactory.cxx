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

#include "XClassFactory.hxx"

// 0x10001120
// 0x10006124
XClassFactory::XClassFactory(LPUNKNOWN object, LPUNKNOWN handler, XModule* module) {
    this->Count = 0;
    this->Object = object;
    this->Module = module;
    this->Handler = handler == nullptr ? object : handler;
}

// 0x10001170
HRESULT STDMETHODCALLTYPE XClassFactory::QueryInterface(const IID& riid, LPVOID* ppvObj) {
    return this->Handler->QueryInterface(riid, ppvObj);
}

// 0x10001190
ULONG STDMETHODCALLTYPE XClassFactory::AddRef() {
    this->Count++;
    return this->Handler->AddRef();
}

// 0x100011b0
ULONG STDMETHODCALLTYPE XClassFactory::Release() {
    this->Count--;
    return this->Handler->Release();
}

// 0x100011d0
HRESULT STDMETHODCALLTYPE XClassFactory::CreateInstance(LPUNKNOWN pUnkOuter, const IID& riid, LPVOID* ppvObject) {
    if (pUnkOuter != nullptr) {
        if (!IsEqualIID(IID_IUnknown, riid)) {
            return CLASS_E_NOAGGREGATION;
        }
    }

    if (IsEqualIID(IID_IUnknown, riid)) {
        return S_OK;
    }

    if (IsEqualIID(IID_IDirectSound, riid)) {
        HRESULT hr = CoInitialize(NULL);

        if (FAILED(hr)) {
            return E_FAIL;
        }

        hr = CoCreateInstance(CLSID_DirectSound, nullptr, CLSCTX_INPROC_SERVER, IID_IDirectSound, ppvObject);

        if (hr == S_OK) {
            this->Module->AddRef();
            return S_OK;
        }

        return E_OUTOFMEMORY;
    }

    return E_FAIL;
}

// 0x10001290
HRESULT STDMETHODCALLTYPE XClassFactory::LockServer(BOOL fLock) {
    if (fLock) {
        this->Module->Lock();
    }
    else {
        this->Module->Unlock();
    }

    return S_OK;
}
