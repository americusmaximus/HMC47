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

#include "ZPropertySet.hxx"

// 0x0ff335d0
ZPropertySet::~ZPropertySet() {
    if (this->PropertySet != nullptr) {
        this->PropertySet->Release();
    }
}

// 0x0ff33680
bool ZPropertySet::Set(LPDIRECTSOUNDBUFFER buffer) {
    return buffer->QueryInterface(IID_IKsPropertySet, (LPVOID*)&this->PropertySet);
}

// 0x0ff33f60
void ZPropertySet::Method0x8() {}

// 0x0ff33620
bool ZPropertySet::Query() {
    if (this->PropertySet == nullptr) {
        g_pSysCom->Log("Z:\\Engine\\Sound\\_Wintel\\Source\\EaxClasses.cpp", 65)
            ->LogMessage("Querying uninitialized buffer");

        return false;
    }

    ULONG support = 0;
    HRESULT hr = this->PropertySet->QuerySupport(DSPROPSETID_EAX20_ListenerProperties,
        DSPROPERTY_EAXLISTENER_ALLPARAMETERS, &support);

    return SUCCEEDED(hr)
        && (support & (KSPROPERTY_SUPPORT_SET | KSPROPERTY_SUPPORT_GET)) == (KSPROPERTY_SUPPORT_SET | KSPROPERTY_SUPPORT_GET);
}
