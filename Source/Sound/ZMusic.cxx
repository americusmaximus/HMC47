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

#include "DirectSound.hxx"

#include <System/ZMusic.hxx>

#include <math.h>

// 0x0ff311c0
// 0x0ff5098c
ZMusic::ZMusic() {
    this->Init = false;

    this->DirectMusic = nullptr;
    this->DirectMusicLoader = nullptr;
    this->DirectMusicPerformance = nullptr;
    this->DirectMusicPort = nullptr;
    this->DirectSoundBuffer = nullptr;

    this->Unk0x4 = -1;
    this->Unk0x10 = 0;
    this->Segments = new RefTab(8, 0);
    this->Unk0x15 = false;
}

// 0x0ff31250
bool ZMusic::IsInit() {
    return this->Init;
}

// 0x0ff31260
// 0x0ff31410
ZMusic::~ZMusic() {
    // TODO NOT IMPLEMENTED

    CoUninitialize();
}

// 0x0ff314c0
void ZMusic::PrintStatus() {
    RefLink link;

    g_pSysCom->Log("Z:\\Engine\\Sound\\_Wintel\\Source\\DMusic.cpp", 140)
        ->LogMessage("Current loaded segments:");

    if (this->Segments != nullptr) {
        this->Segments->GetStart(&link);
        u32 todo = this->Segments->GetNextKey(&link);

        while (link.Next != nullptr) {
            segment = FUN_0ff313e0(todo);

            if (segment != 0) { // TODO
                // TODO NT IMPLEMENTED

                g_pSysCom->Log("Z:\\Engine\\Sound\\_Wintel\\Source\\DMusic.cpp", 145)
                    ->LogMessage("Name %s dir %s chapter id %d segmentid %d",
                        TODO);
            }

            todo = this->Segments->GetNextKey(&link);
        }
    }
}

// 0x0ff31590
BOOL ZMusic::Initialize(LPDIRECTSOUND ds, HWND window) {
    this->DirectSound = ds;

    CoInitialize(nullptr);

    HRESULT hr = CoCreateInstance(CLSID_DirectMusic, nullptr,
        CLSCTX_INPROC_SERVER, IID_IDirectMusic, (LPVOID*)&this->DirectMusic);

    if (FAILED(hr)) {
        DirectMusicLogMessage(hr, "Create Direct music failed");
        return FALSE;
    }

    u32 index = 0;

    DMUS_PORTCAPS caps;
    ZeroMemory(&caps, sizeof(DMUS_PORTCAPS));
    caps.dwSize = sizeof(DMUS_PORTCAPS);

    while (SUCCEEDED(this->DirectMusic->EnumPort(index, &caps))) {
        index++;
    }

    hr = CoCreateInstance(CLSID_DirectMusicLoader, nullptr,
        CLSCTX_INPROC_HANDLER | CLSCTX_INPROC_SERVER, IID_IDirectMusicLoader, (LPVOID*)&this->DirectMusicLoader);

    if (FAILED(hr)) {
        DirectMusicLogMessage(hr, "Create loader failed");
        return FALSE;
    }

    hr = this->DirectMusic->SetDirectSound(ds, NULL);

    if (FAILED(hr)) {
        DirectMusicLogMessage(hr, "Set directsound failed");
        return FALSE;
    }

    hr = CoCreateInstance(CLSID_DirectMusicPerformance, nullptr,
        CLSCTX_INPROC_HANDLER | CLSCTX_INPROC_SERVER, IID_IDirectMusicPerformance, (LPVOID*)&this->DirectMusicPerformance);

    if (FAILED(hr)) {
        DirectMusicLogMessage(hr, "Create performance failed");
        return FALSE;
    }

    hr = this->DirectMusicPerformance->Init(&this->DirectMusic, this->DirectSound, window);

    if (FAILED(hr)) {
        DirectMusicLogMessage(hr, "Performance init failed");
        return FALSE;
    }

    this->DirectMusicPort = this->CreateDirectMusicPort();
    hr = this->DirectMusicPerformance->AddPort(this->DirectMusicPort);

    if (FAILED(hr)) {
        DirectMusicLogMessage(hr, "Add Port failed");
        return FALSE;
    }

    if (this->DirectMusicPort != nullptr) {
        if (!this->CreateStreamingBuffer()) {
            this->DirectMusicPort->Release();
            this->DirectMusicPort = nullptr;
        }
        else {
            if (FAILED(hr = this->DirectMusicPort->SetDirectSound(this->DirectSound, this->DirectSoundBuffer))) {
                DirectMusicLogMessage(hr, "Port->SetDirectSound Failed");
                return FALSE;
            }
        }
    }

    if (FAILED(hr = this->DirectMusicPerformance->AssignPChannelBlock(0, this->DirectMusicPort, 1))) {
        DirectMusicLogMessage(hr, "Assign channel block failed");
        return FALSE;
    }

    this->DirectMusicPort->Activate(TRUE);
    this->DirectSoundBuffer->Play(0, 0, DSBPLAY_LOOPING);
    this->DirectSoundBuffer->SetVolume(DSBVOLUME_MAX);

    this->Init = true;

    g_pSysCom->Log("Z:\\Engine\\Sound\\_Wintel\\Source\\DMusic.cpp", 239)
        ->LogMessage("Direct music init ok");

    return TRUE;
}

// 0x0ff31c70
void ZMusic::Method0x18(u32 param_5) {
    if (!this->Init) {
        return;
    }

    if (this->Method0x28(param_5) != nullptr) { // TODO
        return;
    }


    // ZMusicSegment is inherited from BlockRefTab


    TODO
}

// 0x0ff323d0
BOOL ZMusic::CreateStreamingBuffer() {
    WAVEFORMATEX format;
    ZeroMemory(&format, sizeof(WAVEFORMATEX));

    format.wFormatTag = WAVE_FORMAT_PCM;
    format.nChannels = 2;
    format.nSamplesPerSec = 22050;
    format.nAvgBytesPerSec = 88200;
    format.nBlockAlign = 4;
    format.wBitsPerSample = 16;

    DSBUFFERDESC desc;
    ZeroMemory(&desc, sizeof(DSBUFFERDESC));

    desc.dwSize = sizeof(DSBUFFERDESC);
    desc.dwFlags = DSBCAPS_GETCURRENTPOSITION2
        | DSBCAPS_STICKYFOCUS | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY;
    desc.dwBufferBytes = 88200;
    desc.lpwfxFormat = &format;

    HRESULT hr = DS_OK;
    if (FAILED(hr = this->DirectSound->CreateSoundBuffer(&desc, &this->DirectSoundBuffer, nullptr))) {
        DirectSoundLogMessage(hr, "Dmusic create streaming buffer failed");
        return FALSE;
    }

    return TRUE;
}

// 0x0ff322e0
void ZMusic::StopPerformance(u32) {
    if (this->DirectMusicPerformance != nullptr) {
        this->DirectMusicPerformance->Stop(nullptr, nullptr, 0, 0);
    }
}

// 0x0ff32490
IDirectMusicPort* ZMusic::CreateDirectMusicPort() {
    if (this->DirectMusic == nullptr) {
        return nullptr;
    }

    GUID id;
    ZeroMemory(&id, sizeof(GUID));

    LPDIRECTMUSICPORT port = nullptr;

    if (SUCCEEDED(this->DirectMusic->GetDefaultPort(&id))) {
        DMUS_PORTPARAMS params;
        ZeroMemory(&params, sizeof(DMUS_PORTPARAMS));

        params.dwSize = sizeof(DMUS_PORTPARAMS);
        params.dwChannelGroups = 1;
        params.dwValidParams = 2;

        if (SUCCEEDED(this->DirectMusic->CreatePort(id, &params, &port, nullptr))) {
            DMUS_PORTCAPS caps;
            ZeroMemory(&caps, sizeof(DMUS_PORTCAPS));
            caps.dwSize = sizeof(DMUS_PORTCAPS);

            if (SUCCEEDED(port->GetCaps(&caps)) && caps.dwClass == DMUS_PC_OUTPUTCLASS
                && (caps.dwFlags & DMUS_PC_DLS) && (caps.dwFlags & DMUS_PC_DIRECTSOUND)) {
                return port;
            }
            else {
                port->Release();
                port = nullptr;
            }
        }
    }

    u32 index = 0;

    DMUS_PORTCAPS caps;
    ZeroMemory(&caps, sizeof(DMUS_PORTCAPS));
    caps.dwSize = sizeof(DMUS_PORTCAPS);

    HRESULT hr = DS_OK;
    if (SUCCEEDED(hr = this->DirectMusic->EnumPort(0, &caps))) {
        while (hr != S_FALSE) {
            if (caps.dwClass == DMUS_PC_OUTPUTCLASS
                && (caps.dwFlags & DMUS_PC_DLS) && (caps.dwFlags & DMUS_PC_DIRECTSOUND)) {
                DMUS_PORTPARAMS params;
                ZeroMemory(&params, sizeof(DMUS_PORTPARAMS));

                params.dwSize = sizeof(DMUS_PORTPARAMS);
                params.dwChannelGroups = 1;
                params.dwValidParams = 2;

                return SUCCEEDED(this->DirectMusic->CreatePort(caps.guidPort, &params, &port, nullptr))
                    ? port : nullptr;
            }

            ZeroMemory(&caps, sizeof(DMUS_PORTCAPS));
            caps.dwSize = sizeof(DMUS_PORTCAPS);

            index++;

            if (FAILED(hr = this->DirectMusic->EnumPort(index, &caps))) {
                return port;
            }
        }
    }

    return port;
}

// 0x0ff32670
void ZMusic::SetVolume(f32 value) {
    if (this->DirectSoundBuffer != nullptr) {
        f32 volume = (f32)DSBVOLUME_MIN;

        if (value != 0.0f) {
            volume = log10(2.0) * value * 3000.0;
        }

        HRESULT hr = DS_OK;
        if (FAILED(hr = this->DirectSoundBuffer->SetVolume((s32)volume))) {
            DirectSoundLogMessage(hr, "unable to set volume");
        }
    }
}
