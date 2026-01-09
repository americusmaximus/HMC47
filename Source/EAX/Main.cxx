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

#define USERNAME_LENGTH 256

static const GUID CLSID_EAXDirectSound =
    { 0x4ff53b81, 0x1ce0, 0x11d3, { 0xaa, 0xb8, 0x0, 0xa0, 0xc9, 0x59, 0x49, 0xd5 } };      // 0x100062a8

static XModule* g_Module;                                                                   // 0x10007550

void CompactWideString(char* value, int length);

// 0x10001000
extern "C"
HRESULT WINAPI EAXDirectSoundCreate(LPCGUID pcGuidDevice, LPDIRECTSOUND * ppDS, LPUNKNOWN pUnkOuter) {
    return DirectSoundCreate(pcGuidDevice, ppDS, pUnkOuter);
}

// 0x100012c0
BOOL CheckUserName() {
    char name[USERNAME_LENGTH];
    DWORD length = USERNAME_LENGTH;

    if (!GetUserNameA(name, &length)) {
        return GetLastError() != ERROR_CALL_NOT_IMPLEMENTED;
    }

    return TRUE;
}

// 0x10001310
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID) {
    if (ul_reason_for_call == DLL_PROCESS_DETACH) {
        CoUninitialize();

        if (g_Module != nullptr) {
            delete g_Module;
            g_Module = nullptr;
        }
    }
    else if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        if (CheckUserName()) {
            g_Module = new XModule();

            if (g_Module != nullptr) {
                g_Module->Module = hModule;
                return TRUE;
            }
        }

        return FALSE;
    }

    return TRUE;
}

// 0x100013f0
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID FAR* ppv) {
    HRESULT hr = CLASS_E_CLASSNOTAVAILABLE;

    if (IsEqualCLSID(rclsid, CLSID_EAXDirectSound)) {
        hr = E_OUTOFMEMORY;

        XClassObject* object = new XClassObject(nullptr, g_Module);

        if (object != nullptr) {
            g_Module->AddRef();

            if (FAILED(hr = object->QueryInterface(riid, ppv))) {
                g_Module->RemoveRef();

                delete object;
            }
        }
    }

    return hr;
}

// 0x10001470
STDAPI DllCanUnloadNow() {
    return g_Module->Refs == 0 && g_Module->Locks == 0 ? S_OK : S_FALSE;
}

// 0x10001490
DWORD SetRegistryValue(const char* path, const char* name, const char* value) {
    char key[256];
    DWORD result = FALSE;

    lstrcpyA(key, path);

    if (name != nullptr) {
        lstrcatA(key, "\\");
        lstrcatA(key, name);
    }

    HKEY rk;
    LSTATUS status =
        RegCreateKeyExA(HKEY_CLASSES_ROOT, key, 0, nullptr, 0, KEY_ALL_ACCESS, nullptr, &rk, nullptr);

    if (value != nullptr && status == ERROR_SUCCESS) {
        result = RegSetValueExA(rk, nullptr, 0, REG_SZ, (BYTE*)value, lstrlenA(value) + 1) == ERROR_SUCCESS;
        RegCloseKey(rk);
    }

    return result;
}

// 0x10001540
DWORD SetRegistryValues(const char* path,
    const char* name, const char* value, const char* subname, const char* subvalue) {
    char key[256];
    DWORD result = FALSE;

    lstrcpyA(key, path);

    if (name != nullptr) {
        lstrcatA(key, "\\");
        lstrcatA(key, name);
    }

    HKEY rk;
    LSTATUS status =
        RegCreateKeyExA(HKEY_CLASSES_ROOT, key, 0, nullptr, 0, KEY_ALL_ACCESS, nullptr, &rk, nullptr);

    if (value == nullptr || status != ERROR_SUCCESS) {
        return FALSE;
    }

    status = RegSetValueExA(rk, nullptr, 0, REG_SZ, (BYTE*)value, lstrlenA(value) + 1);

    if (subname == nullptr || subvalue == nullptr) {
        result = TRUE;

        if (status == ERROR_SUCCESS) {
            RegCloseKey(rk);

            return result;
        }
    }
    else if (status == ERROR_SUCCESS) {
        status = RegSetValueExA(rk, subname, 0, REG_SZ, (BYTE*)subvalue, lstrlenA(subvalue) + 1);

        result = TRUE;

        if (status == ERROR_SUCCESS) {
            RegCloseKey(rk);
            return result;
        }
    }

    RegCloseKey(rk);

    return result;
}

// 0x10001640
extern "C"
HRESULT WINAPI DllRegisterServer() {
    OLECHAR id[128];
    CHAR value[132];

    StringFromGUID2(CLSID_EAXDirectSound, id, 128);

    CompactWideString((char*)id, 64);

    lstrcpyA(value, "CLSID\\");
    lstrcatA(value, (LPCSTR)id);

    SetRegistryValue("EAX", nullptr, "EAX Object");
    SetRegistryValue("EAX", "CLSID", (LPCSTR)id);
    SetRegistryValue(value, nullptr, "EAX Object");
    SetRegistryValues(value, "InprocServer32", "EAX.dll", "ThreadingModel", "Apartment");

    return S_OK;
}

// 0x10001700
extern "C"
HRESULT WINAPI DllUnregisterServer() {
    OLECHAR id[128];
    CHAR key[132];
    CHAR value[132];

    StringFromGUID2(CLSID_EAXDirectSound, id, 128);

    CompactWideString((char*)id, 64);

    lstrcpyA(key, "CLSID\\");
    lstrcatA(key, key);

    RegDeleteKeyA(HKEY_CLASSES_ROOT, "EAX\\CLSID");
    RegDeleteKeyA(HKEY_CLASSES_ROOT, "EAX");

    wsprintfA(value, "%s\\%s", key, "InprocServer32");

    RegDeleteKeyA(HKEY_CLASSES_ROOT, value);
    RegDeleteKeyA(HKEY_CLASSES_ROOT, key);

    return S_OK;
}

// 0x100017c0
void CompactWideString(char* value, int length) {
    if (length < 2) {
        value[length] = NULL;
        return;
    }

    for (int i = 1, p = 2; i < length; i++, p += 2) {
        value[i] = value[p];
    }

    value[length] = NULL;
}

// 0x10001800
extern "C"
HRESULT GetCurrentVersion(DWORD * dwMajor, DWORD * dwMinor) {
    *dwMajor = 0;
    *dwMinor = 0;

    return S_OK;
}
