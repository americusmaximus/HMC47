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

#include <System/ZDirectPlay.hxx>

static const GUID DirectPlayInstance =
{ 0x26df80b0, 0x3de8, 0x11d3, { 0x81, 0x29, 0x00, 0xe0, 0x18, 0x90, 0x22, 0x7d } };      // 0x100070f8

BOOL PASCAL DirectPlayEnumerateCallback(LPGUID lpguidSP, LPSTR lpSPName,
    DWORD dwMajorVersion, DWORD dwMinorVersion, LPVOID lpContext);
BOOL PASCAL DirectPlayEnumerateSessionsCallback(LPCDPSESSIONDESC2 lpThisSD,
    LPDWORD lpdwTimeOut, DWORD dwFlags, LPVOID lpContext);

// 0x10001240
// 0x10007140
ZDirectPlay::ZDirectPlay() {
    this->Instance = nullptr;
    this->ID = nullptr;

    this->SessionName = new char[strlen("lortesession") + 1];
    strcpy(this->SessionName, "lortesession");

    this->Unk0x9 = new char[strlen("lorteliv") + 1];
    strcpy(this->Unk0x9, "lorteliv");

    this->Unk0xD = new char[strlen("lorte liv") + 1];
    strcpy(this->Unk0xD, "lorte liv");

    ZDirectPlay::Enumerate();
    ZDirectPlay::Initialize();
}

// 0x10001180
BOOL PASCAL DirectPlayEnumerateCallback(LPGUID lpguidSP, LPSTR lpSPName,
    DWORD dwMajorVersion, DWORD dwMinorVersion, LPVOID lpContext) {
    if (strstr(lpSPName, "TCP") != nullptr) {
        *(GUID**)((LPVOID*)lpContext) = new GUID;
        CopyMemory(*(GUID**)((LPVOID*)lpContext), lpguidSP, sizeof(GUID));

        return FALSE;
    }

    return TRUE;
}

// 0x100011f0
BOOL PASCAL DirectPlayEnumerateSessionsCallback(LPCDPSESSIONDESC2 lpThisSD,
    LPDWORD lpdwTimeOut, DWORD dwFlags, LPVOID lpContext) {
    if (dwFlags & DPESC_TIMEDOUT) {
        *(bool*)lpContext = false;
        return FALSE;
    }

    g_pSysCom->Log("Z:\\Engine\\Network\\_Wintel\\DirectPlay\\Source\\DirectPlay.cpp", 223)
        ->LogMessage("Found Session: %s", lpThisSD->lpszSessionNameA);

    *(bool*)lpContext = true;
    return FALSE;
}

// 0x100013f0
ZDirectPlay::~ZDirectPlay() {
    ZDirectPlay::Release();
}

// 0x100014d0
void ZDirectPlay::Initialize() {
    LPDIRECTPLAY dp = nullptr;
    DirectPlayCreate(this->ID, &dp, nullptr);

    dp->QueryInterface(IID_IDirectPlay2, (LPVOID*)&this->Instance);
    dp->Close();
    dp->Release();

    if (this->HasSession()) {
        this->Active = false;
        this->OpenUntitledSession();
    }
    else {
        this->Active = true;
        this->OpenSession(10);
    }
}

// 0x10001540
void ZDirectPlay::Release() {
    if (this->Instance != nullptr) {
        this->Instance->Close();
        this->Instance->Release();
    }

    delete[] this->SessionName;
    delete[] this->Unk0x9;
    delete[] this->Unk0xD;

    delete this->ID;
}

// 0x100015f0
void ZDirectPlay::Enumerate() {
    DirectPlayEnumerateA(DirectPlayEnumerateCallback, &this->ID);
}

// 0x10001600
bool ZDirectPlay::HasSession() {
    DPSESSIONDESC2 desc;
    ZeroMemory(&desc, sizeof(DPSESSIONDESC2));

    desc.dwSize = sizeof(DPSESSIONDESC2);
    CopyMemory(&desc.guidInstance, &DirectPlayInstance, sizeof(GUID));

    bool result = false;
    this->Instance->EnumSessions(&desc, 1000, DirectPlayEnumerateSessionsCallback, &result, 0);

    return result;
}

// 0x10001670
void ZDirectPlay::OpenSession(u32 count) {
    DPSESSIONDESC2 desc;
    ZeroMemory(&desc, sizeof(DPSESSIONDESC2));

    desc.dwSize = sizeof(DPSESSIONDESC2);
    desc.dwFlags = 2; // TODO
    CopyMemory(&desc.guidInstance, &DirectPlayInstance, sizeof(GUID));
    desc.dwMaxPlayers = count;
    desc.lpszSessionNameA = this->SessionName;
    desc.dwUser1 = 2;

    this->Instance->Open(&desc, 0);
}

// 0x100016f0
void ZDirectPlay::OpenUntitledSession() {
    DPSESSIONDESC2 desc;
    ZeroMemory(&desc, sizeof(DPSESSIONDESC2));

    desc.dwSize = sizeof(DPSESSIONDESC2);
    desc.dwFlags = DPSESSION_NEWPLAYERSDISABLED;
    CopyMemory(&desc.guidInstance, &DirectPlayInstance, sizeof(GUID));
    desc.dwUser1 = 2;

    this->Instance->Open(&desc, 0);
}
