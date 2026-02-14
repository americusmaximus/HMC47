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
#include "ZSoundBuffer.hxx"

#include <System/ZSound.hxx>

static HANDLE g_SoundThread;                                                                    // 0x0ff5a738

static LPDIRECTSOUND g_pDirectSound;                                                            // 0x0ff5a74c

// 0x0ff35d80
// 0x0ff50bfc
ZSound::ZSound() {
    // TODO
}

// 0x0ff35e30
// 0x0ff35df0
ZSound::~ZSound() {
    // TODO
}

// 0x0ff36130
BOOL CALLBACK ZSoundEnumerateCallback(LPGUID, LPCSTR, LPCSTR, LPVOID) {
    return TRUE;
}

// 0x0ff36140
bool ZSound::Method0x1CC() {
    return this->Unk0x3F1;
}

// 0x0ff36170
BOOL ZSound::Init(HWND window) {
    this->Unk0x106 = 0;

    if (this->Unk0x10A) {
        return TRUE;
    }

    this->Unk0x10A = true;

    if (DirectSoundEnumerateA(ZSoundEnumerateCallback, nullptr) == DS_OK) {
        HRESULT hr = DS_OK;

        if ((hr = EAXDirectSoundCreate(nullptr, &this->DirectSound, nullptr)) == DS_OK) {
            g_pDirectSound = this->DirectSound;

            if ((hr = this->DirectSound->SetCooperativeLevel(window, DSSCL_PRIORITY | DSSCL_NORMAL)) == DS_OK) {
                this->Caps.dwSize = sizeof(DSCAPS);

                if ((hr = this->DirectSound->GetCaps(&this->Caps)) == DS_OK) {
                    this->SpatialStreamingBuffers = this->Caps.dwMaxHw3DStreamingBuffers;

                    if (this->SpatialStreamingBuffers == 0) {
                        this->Unk0xDA = false;
                    }
                    else {
                        LPDIRECTSOUNDBUFFER dsb =
                            this->InitSoundBuffer(DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_CTRLVOLUME
                                | DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRL3D | DSBCAPS_LOCHARDWARE, "HARDWARE", 0x4000);

                        if (dsb != nullptr) {
                            ZSoundBuffer* buffer = new ZSoundBuffer();

                            if (buffer->Set(dsb)) {
                                if (buffer->Query()) {
                                    this->Unk0xC9 = true;
                                }
                                else {
                                    this->Unk0xC9 = false;
                                    this->Unk0x3F1 = false;
                                }
                            }

                            if (this->Method0x1B0() && !this->Method0x1CC()) {
                                EAXLISTENERPROPERTIES props;
                                ZeroMemory(&props, sizeof(EAXLISTENERPROPERTIES));

                                buffer->GetPropertySet()->Set(DSPROPSETID_EAX20_ListenerProperties,
                                    0, nullptr, 0, &props, sizeof(EAXLISTENERPROPERTIES));
                            }

                            if (buffer != nullptr) {
                                delete buffer;
                            }
                            
                            dsb->Release();
                        }
                    }

                    if (!this->CreatePrimaryBuffer()) {
                        return FALSE;
                    }

                    if (!this->FUN_0ff3d660()) {
                        return FALSE;
                    }

                    g_pSysCom->Log("Z:\\Engine\\Sound\\_Wintel\\Source\\SoundConDS.cpp", 337)
                        ->LogMessage("Direct sound init ok");

                    if (this->Music != nullptr) {
                        if (!this->Music->Initialize(this->DirectSound, window)) {
                            if (this->Music != nullptr) {
                                delete this->Music;
                            }

                            this->Music = nullptr;
                        }
                    }

                    g_SoundThread = FUN_0ff42817(FUN_0ff374e0, 0, this); // TODO
                    SetThreadPriority(g_SoundThread, THREAD_PRIORITY_TIME_CRITICAL);

                    return TRUE;
                }
                else {
                    DirectSoundLogMessage(hr, "Get caps failed");
                }
            }
            else {
                DirectSoundLogMessage(hr, "Set coop level failed");
            }
        }
        else {
            DirectSoundLogMessage(hr, "CDSOUNDCON::Init - Create DS-Object failed");
        }
    }
    else {
        g_pSysCom->Log("Z:\\Engine\\Sound\\_Wintel\\Source\\SoundConDS.cpp", 270)
            ->LogMessage("dsEnumerate failed");
    }

    return FALSE;
}

// 0x0ff36420
BOOL ZSound::EndInit(const char* path) {
    return ZSoundBase::EndInit(path);
}

// 0x0ff36430
bool ZSound::CreatePrimaryBuffer() {
    HRESULT hr = DS_OK;

    DSBUFFERDESC desc;
    ZeroMemory(&desc, sizeof(DSBUFFERDESC));

    desc.dwSize = sizeof(DSBUFFERDESC);
    desc.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRL3D | DSBCAPS_PRIMARYBUFFER;

    if ((hr = this->DirectSound->CreateSoundBuffer(&desc, &this->DirectSoundBuffer, nullptr)) != DS_OK) {
        DirectSoundLogMessage(hr, "create primary buffer failed");
        return false;
    }

    WAVEFORMATEX format;

    format.wFormatTag = WAVE_FORMAT_PCM;
    format.nChannels = 2;
    format.nSamplesPerSec = 22050;
    format.nAvgBytesPerSec = 22050 * 4;
    format.nBlockAlign = 4;
    format.wBitsPerSample = 16;
    format.cbSize = sizeof(WAVEFORMATEX);

    if ((hr = this->DirectSoundBuffer->SetFormat(&format)) != DS_OK) {
        DirectSoundLogMessage(hr, "Set format failed");
        return false;
    }

    if ((hr = this->DirectSoundBuffer->Play(0, 0, DSBPLAY_LOOPING)) != DS_OK) {
        DirectSoundLogMessage(hr, "CDSOUNDCON::CreatePBuffer - play failed");
        return false;
    }

    return true;
}

// 0x0ff36b20
BOOL ZSound::RenderFrame() {
    // TODO NOT IMPLEMENTED
}

// 0x0ff36d30
bool ZSound::Method0xC() {
    // TODO NOT IMPLEMENTED
}

// 0x0ff370d0
LPDIRECTSOUNDBUFFER ZSound::InitSoundBuffer(u32 flags, const char* name, u32 size) {
    DSBUFFERDESC desc;
    ZeroMemory(&desc, sizeof(DSBUFFERDESC));

    WAVEFORMATEX format;
    ZeroMemory(&format, sizeof(WAVEFORMATEX));

    format.wFormatTag = WAVE_FORMAT_PCM;
    format.nChannels = 1;
    format.nSamplesPerSec = 22050;
    format.nAvgBytesPerSec = 22050 * 2;
    format.nBlockAlign = 2;
    format.wBitsPerSample = 16;

    desc.dwSize = sizeof(DSBUFFERDESC);
    desc.dwFlags = flags;
    desc.dwBufferBytes = size;
    desc.lpwfxFormat = &format;

    if (g_pSysInterface->DebugOptionsVisibility != 0.0f) {
        if (g_pSysInterface->TimersVisibility != 0.0f) {
            g_pSysInterface->SetRenderNameCounterValue(name, __rdtsc());
        }
    }

    LPDIRECTSOUNDBUFFER buffer = nullptr;
    HRESULT hr = this->DirectSound->CreateSoundBuffer(&desc, &buffer, nullptr);

    if (g_pSysInterface->DebugOptionsVisibility != 0.0f) {
        if (g_pSysInterface->TimersVisibility != 0.0f) {
            g_pSysInterface->SetRenderCounterValue(__rdtsc());
        }
    }

    if (hr != DS_OK) {
        DirectSoundLogMessage(hr, "Create test buffer failed");

        return nullptr;
    }

    if (g_pSysInterface->DebugOptionsVisibility != 0.0f) {
        if (g_pSysInterface->TimersVisibility != 0.0f) {
            g_pSysInterface->SetRenderNameCounterValue("lock", __rdtsc());
        }
    }

    void* audio1 = nullptr;
    void* audio2 = nullptr;
    DWORD audio1size = 0, audio2size = 0;

    hr = buffer->Lock(0, size, &audio1, &audio1size, &audio2, &audio2size, 0);

    if (hr != DS_OK) {
        DirectSoundLogMessage(hr, "lock on buffer failed");

        return nullptr;
    }

    if (g_pSysInterface->DebugOptionsVisibility != 0.0f) {
        if (g_pSysInterface->TimersVisibility != 0.0f) {
            g_pSysInterface->SetRenderCounterValue(__rdtsc());
        }
    }

    if (g_pSysInterface->DebugOptionsVisibility != 0.0f) {
        if (g_pSysInterface->TimersVisibility != 0.0f) {
            g_pSysInterface->SetRenderNameCounterValue("CopyWaveData", __rdtsc());
        }
    }

    ZeroMemory(audio1, size);

    if (g_pSysInterface->DebugOptionsVisibility != 0.0f) {
        if (g_pSysInterface->TimersVisibility != 0.0f) {
            g_pSysInterface->SetRenderCounterValue(__rdtsc());
        }
    }

    if (g_pSysInterface->DebugOptionsVisibility != 0.0f) {
        if (g_pSysInterface->TimersVisibility != 0.0f) {
            g_pSysInterface->SetRenderNameCounterValue("Unlock", __rdtsc());
        }
    }

    hr = buffer->Unlock(audio1, audio1size, audio2, audio2size);

    if (hr != DS_OK) {
        DirectSoundLogMessage(hr, "unlock failed");

        return nullptr;
    }

    if (g_pSysInterface->DebugOptionsVisibility != 0.0f) {
        if (g_pSysInterface->TimersVisibility != 0.0f) {
            g_pSysInterface->SetRenderCounterValue(__rdtsc());
        }
    }

    buffer->SetVolume(DSBVOLUME_MIN);

    return buffer;
}

// 0x0ff3d660
BOOL ZSound::FUN_0ff3d660() {
    this->Unk0x102 = 0;
    this->Unk0x106 = 0;
    this->Unk0xFE = 0;
    this->Unk0x10A = true;

    this->Method0xC();
    this->Method0x38();

    this->Unk0xDF = true;

    return TRUE;
}
