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
#include "ZMusicSegment.hxx"

#include <System/ZMusic.hxx>

#include <math.h>

#define ZMUSIC_MAX_BAND_COUNT   10

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
    this->Quality = ZMUSICQUALITY_NORMAL;
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
                // TODO NOT IMPLEMENTED

                g_pSysCom->Log("Z:\\Engine\\Sound\\_Wintel\\Source\\DMusic.cpp", 145)
                    ->LogMessage("Name %s dir %s chapter id %d segmentid %d",
                        TODO);
            }

            todo = this->Segments->GetNextKey(&link);
        }
    }
}

// 0x0ff31590
bool ZMusic::Initialize(LPDIRECTSOUND ds, HWND window) {
    HRESULT hr = S_OK;

    this->DirectSound = ds;

    CoInitialize(nullptr);

    if (FAILED(hr = CoCreateInstance(CLSID_DirectMusic, nullptr,
        CLSCTX_INPROC_SERVER, IID_IDirectMusic, (LPVOID*)&this->DirectMusic))) {
        DirectMusicLogMessage(hr, "Create Direct music failed");
        return false;
    }

    u32 index = 0;

    DMUS_PORTCAPS caps;
    ZeroMemory(&caps, sizeof(DMUS_PORTCAPS));
    caps.dwSize = sizeof(DMUS_PORTCAPS);

    while (SUCCEEDED(this->DirectMusic->EnumPort(index, &caps))) {
        index++;
    }

    if (FAILED(hr = CoCreateInstance(CLSID_DirectMusicLoader, nullptr,
        CLSCTX_INPROC_HANDLER | CLSCTX_INPROC_SERVER, IID_IDirectMusicLoader, (LPVOID*)&this->DirectMusicLoader))) {
        DirectMusicLogMessage(hr, "Create loader failed");
        return false;
    }

    if (FAILED(hr = this->DirectMusic->SetDirectSound(ds, NULL))) {
        DirectMusicLogMessage(hr, "Set directsound failed");
        return false;
    }

    if (FAILED(hr = CoCreateInstance(CLSID_DirectMusicPerformance, nullptr,
        CLSCTX_INPROC_HANDLER | CLSCTX_INPROC_SERVER, IID_IDirectMusicPerformance, (LPVOID*)&this->DirectMusicPerformance))) {
        DirectMusicLogMessage(hr, "Create performance failed");
        return false;
    }

    if (FAILED(hr = this->DirectMusicPerformance->Init(&this->DirectMusic, this->DirectSound, window))) {
        DirectMusicLogMessage(hr, "Performance init failed");
        return false;
    }

    this->DirectMusicPort = this->CreateDirectMusicPort();

    if (FAILED(hr = this->DirectMusicPerformance->AddPort(this->DirectMusicPort))) {
        DirectMusicLogMessage(hr, "Add Port failed");
        return false;
    }

    if (this->DirectMusicPort != nullptr) {
        if (!this->CreateStreamingBuffer()) {
            this->DirectMusicPort->Release();
            this->DirectMusicPort = nullptr;
        }
        else {
            if (FAILED(hr = this->DirectMusicPort->SetDirectSound(this->DirectSound, this->DirectSoundBuffer))) {
                DirectMusicLogMessage(hr, "Port->SetDirectSound Failed");
                return false;
            }
        }
    }

    if (FAILED(hr = this->DirectMusicPerformance->AssignPChannelBlock(0, this->DirectMusicPort, 1))) {
        DirectMusicLogMessage(hr, "Assign channel block failed");
        return false;
    }

    this->DirectMusicPort->Activate(TRUE);
    this->DirectSoundBuffer->Play(0, 0, DSBPLAY_LOOPING);
    this->DirectSoundBuffer->SetVolume(DSBVOLUME_MAX);

    this->Init = true;

    g_pSysCom->Log("Z:\\Engine\\Sound\\_Wintel\\Source\\DMusic.cpp", 239)
        ->LogMessage("Direct music init ok");

    return true;
}

// 0x0ff31c70
void ZMusic::CreateSegment(const char* directory, const char* file, u32 param_4, u32 param_5) {
    if (!this->Init) {
        return;
    }

    if (this->Method0x28(param_5) != nullptr) { // TODO
        return;
    }

    ZMusicSegment* segment = new ZMusicSegment();

    segment->File = file;
    segment->Unk0xB0 = param_5;
    segment->Unk0xB4 = param_4;
    segment->Directory = directory;

    // TODO NOT IMPLEMENTED

    g_pSysMem->Index(segment);
    this->Segments->Insert(TODO);

    // TODO NOT IMPLEMENTED

    char dir[MAX_PATH];
    GetCurrentDirectoryA(MAX_PATH, dir);

    strcat(dir, "\\");
    strcat(dir, "Music\\");

    if (this->Quality == ZMUSICQUALITY_LOW) {
        strcat(dir, "Low\\");
    }

    strcat(dir, directory);

    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, dir, -1, aWStack_e1c, MAX_PATH);
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, file, -1, aWStack_c14, MAX_PATH);

    this->DirectMusicLoader->SetSearchDirectory(GUID_DirectMusicAllTypes, aWStack_e1c, FALSE);

    DMUS_OBJECTDESC segdesc;
    ZeroMemory(&segdesc, sizeof(DMUS_OBJECTDESC));

    segdesc.dwSize = sizeof(DMUS_OBJECTDESC);
    segdesc.dwValidData = DMUS_OBJ_FILENAME | DMUS_OBJ_CLASS;
    segdesc.guidClass = CLSID_DirectMusicSegment;

    wcscpy(segdesc.wszFileName, aWStack_c14);

    HRESULT hr = DS_OK;
    if (FAILED(hr = this->DirectMusicLoader->GetObject(&segdesc,
        IID_IDirectMusicSegment, (LPVOID*)&segment->Segment))) {
        g_pSysCom->Log("Z:\\Engine\\Sound\\_Wintel\\Source\\DMusic.cpp", 377)
            ->LogMessage("Load segment %s\\%s failed", dir, file);
        DirectMusicLogMessage(hr, "Load segment");
    }

    if (segment->Segment != nullptr) {
        segment->Segment->SetRepeats(1000);
    }

    WCHAR style[MAX_PATH];
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, "style1.sty", -1, style, MAX_PATH);

    DMUS_OBJECTDESC stdesc;
    ZeroMemory(&stdesc, sizeof(DMUS_OBJECTDESC));

    stdesc.dwSize = sizeof(DMUS_OBJECTDESC);
    stdesc.dwValidData = DMUS_OBJ_FILENAME | DMUS_OBJ_CLASS;
    segdesc.guidClass = CLSID_DirectMusicStyle;

    wcscpy(stdesc.wszName, style);
    wcscpy(stdesc.wszFileName, style);

    if (FAILED(hr = this->DirectMusicLoader->GetObject(&stdesc,
        IID_IDirectMusicStyle, (LPVOID*)&segment->Style))) {
        DirectMusicLogMessage(hr, "Failed to load style");
    }

    if (segment->Style != nullptr) {
        WCHAR name[MAX_PATH];

        for (u32 i = 0; i < ZMUSIC_MAX_BAND_COUNT; i++) {
            if (SUCCEEDED(segment->Style->EnumBand(i, name))) {
                IDirectMusicBand* band = nullptr;
                segment->Style->GetBand(name, &band);

                if (band != nullptr) {
                    segment->Bands.Insert(REFTAB_PTR_TO_KEY(band));

                    if (FAILED(hr = band->Download(this->DirectMusicPerformance))) {
                        DirectMusicLogMessage(hr, "Download failed");
                    }
                }
            }
        }
    }
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
