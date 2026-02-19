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

#include <System/ZSpatialSoundBuffer.hxx>

// 0x0ff336a0
// 0x0ff509e8
ZSpatialSoundBuffer::ZSpatialSoundBuffer() {
    this->Buffer = nullptr;
}

// 0x0ff336c0
// 0x0ff33700
ZSpatialSoundBuffer::~ZSpatialSoundBuffer() {
    if (this->Buffer != nullptr) {
        this->Buffer->Release();
    }
}

// 0x0ff33770
bool ZSpatialSoundBuffer::Initialize(LPDIRECTSOUND ds, LPDIRECTSOUNDBUFFER buffer) {
    if (buffer != nullptr) {
        WAVEFORMATEX format;
        ZeroMemory(&format, sizeof(WAVEFORMATEX));

        format.wFormatTag = WAVE_FORMAT_PCM;
        format.nChannels = 1;
        format.nSamplesPerSec = 44100;
        format.nAvgBytesPerSec = 88200;
        format.nBlockAlign = 2;
        format.wBitsPerSample = 16;

        DSBUFFERDESC desc;
        ZeroMemory(&desc, sizeof(DSBUFFERDESC));

        desc.dwSize = sizeof(DSBUFFERDESC);
        desc.dwFlags = DSBCAPS_CTRL3D | DSBCAPS_LOCHARDWARE;
        desc.dwBufferBytes = 64;
        desc.lpwfxFormat = &format;

        if (ds->CreateSoundBuffer(&desc, &this->Buffer, nullptr) != DS_OK) {
            g_pSysCom->Log("Z:\\Engine\\Sound\\_Wintel\\Source\\EaxClasses.cpp", 114)
                ->LogMessage("CEAXLISTENER::Init() - Create sound buffer failed");

            return false;
        }

        buffer = this->Buffer;
    }

    if (buffer->QueryInterface(IID_IKsPropertySet, (LPVOID*)&this->PropertySet) != DS_OK) {
        return false;
    }

    ULONG support = 0;
    this->PropertySet->QuerySupport(DSPROPSETID_EAX20_ListenerProperties,
        KSPROPERTY_SUPPORT_GET, &support);

    return true;
}

// 0x0ff33880
void ZSpatialSoundBuffer::Method0x1C(u32 todo) {
    this->Unk0xC = todo;
}

// 0x0ff33890
void ZSpatialSoundBuffer::Method0x8() {
    if (this->PropertySet == nullptr) {
        return;
    }

    void* todo_1 = FUN_0ff33d00(this->Unk0xC);

    if (todo_1 == nullptr) {
        if (g_pSysInterface->SoundModule->GetSound()->Unk0x4 != 0.0f) {
            g_pSysInterface->DisplayDebugString(30, 50, "No room parameters defined for this room");

            for (u32 i = 1; i < 21; i++) { // TODO
                g_pSysInterface->DisplayDebugString(30, 50 + i,
                    "                                                                    ");
            }
        }
    }
    else {
        // TODO NOT IMPLEMENTED




        if (SUCCEEDED(this->PropertySet->Set(DSPROPSETID_EAX20_ListenerProperties,
            DSPROPERTY_EAXLISTENER_DEFERRED | DSPROPERTY_EAXLISTENER_ALLPARAMETERS,
            nullptr, 0, &this->Properties, sizeof(EAXLISTENERPROPERTIES)))
            && g_pSysInterface->SoundModule->GetSound()->Unk0x4 != 0.0f) {
            g_pSysInterface->DisplayDebugString(30, 50, "ENVIRONMENT");

            g_pSysInterface->DisplayDebugString(30, 51, "   1 Preset            %s",
                SoundEnvironments[this->Properties.dwEnvironment]);
            g_pSysInterface->DisplayDebugString(30, 52, "   2 Size                   %5.2f",
                this->Properties.flEnvironmentSize);
            g_pSysInterface->DisplayDebugString(30, 53, "   3 Diffusion              %5.2f",
                this->Properties.flEnvironmentDiffusion);

            g_pSysInterface->DisplayDebugString(30, 54, "ROOM");

            g_pSysInterface->DisplayDebugString(30, 55, "   4 Level                  %5d",
                this->Properties.lRoom);
            g_pSysInterface->DisplayDebugString(30, 56, "   5 HFLevel                %5d",
                this->Properties.lRoomHF);

            g_pSysInterface->DisplayDebugString(30, 57, "REFLECTION");

            g_pSysInterface->DisplayDebugString(30, 58, "   6 Level                  %5d",
                this->Properties.lReflections);
            g_pSysInterface->DisplayDebugString(30, 59, "   7 Delay                  %5.2f",
                this->Properties.flReflectionsDelay);

            g_pSysInterface->DisplayDebugString(30, 60, "REVERB");

            g_pSysInterface->DisplayDebugString(30, 61, "   8 Level                  %5d",
                this->Properties.lReverb);
            g_pSysInterface->DisplayDebugString(30, 62, "   9 Delay                  %5.2f",
                this->Properties.flReverbDelay);

            g_pSysInterface->DisplayDebugString(30, 63, "DECAY");

            g_pSysInterface->DisplayDebugString(30, 64, "  10 Time                   %5.2f",
                this->Properties.flDecayTime);
            g_pSysInterface->DisplayDebugString(30, 65, "  11 HFRatio                %5.2f",
                this->Properties.flDecayHFRatio);

            g_pSysInterface->DisplayDebugString(30, 66, "DISTANCE EFFECT");

            g_pSysInterface->DisplayDebugString(30, 67, "  12 RolloffFactor          %5.2f",
                this->Properties.flRoomRolloffFactor);
            g_pSysInterface->DisplayDebugString(30, 68, "  13 AirAbsorptionHF        %5.2f",
                this->Properties.flAirAbsorptionHF);

            g_pSysInterface->DisplayDebugString(30, 69, "LISTENER FLAGS");

            g_pSysInterface->DisplayDebugString(30, 70, "   Decay Time Scale       %d", 0);
            g_pSysInterface->DisplayDebugString(30, 71, "   Reflection Scale       %d", 0);
            g_pSysInterface->DisplayDebugString(30, 72, "   Reflection Delay Scale %d", 0);
            g_pSysInterface->DisplayDebugString(30, 73, "   Reverb Scale           %d", 0);
            g_pSysInterface->DisplayDebugString(30, 74, "   Reverb Delay Scale     %d", 0);
        }
    }
}

// 0x0ff33d30
bool ZSpatialSoundBuffer::SetRoom(DWORD environment) {
    if (this->PropertySet == nullptr) {
        return false;
    }

    if (FAILED(this->PropertySet->Set(DSPROPSETID_EAX20_ListenerProperties,
        DSPROPERTY_EAXLISTENER_DEFERRED | DSPROPERTY_EAXLISTENER_ENVIRONMENT,
        nullptr, 0, &environment, sizeof(DWORD)))) {
        g_pSysCom->Log("Z:\\Engine\\Sound\\_Wintel\\Source\\EaxClasses.cpp", 215)
            ->LogMessage("SET ROOM FAILED");

        return false;
    }

    ULONG bytes = 0;
    if (FAILED(this->PropertySet->Get(DSPROPSETID_EAX20_ListenerProperties,
        DSPROPERTY_EAXLISTENER_DEFERRED | DSPROPERTY_EAXLISTENER_ALLPARAMETERS,
        nullptr, 0, &this->Properties, sizeof(EAXLISTENERPROPERTIES), &bytes))) {
        g_pSysCom->Log("Z:\\Engine\\Sound\\_Wintel\\Source\\EaxClasses.cpp", 221)
            ->LogMessage("CEAXLISTENER::SetRoom - Get properties failed");

        return false;
    }

    // TODO NOT IMPLEMENTED

    return true;
}

// 0x0ff33e80
bool ZSpatialSoundBuffer::SetRoomSize(f32 size) {
    if (this->PropertySet == nullptr) {
        return false;
    }

    return SUCCEEDED(this->PropertySet->Set(DSPROPSETID_EAX20_ListenerProperties,
        DSPROPERTY_EAXLISTENER_DEFERRED | DSPROPERTY_EAXLISTENER_ENVIRONMENTSIZE,
        nullptr, 0, &size, sizeof(f32)));
}
