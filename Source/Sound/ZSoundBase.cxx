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

#include <System/ZSoundBase.hxx>

// 0x0ff3a7e0
// 0x0ff51024
ZSoundBase::ZSoundBase() {
    this->Unk0x149 = 0;
    this->Unk0x10A = 0;
    this->Unk0x12C = 0;
    this->Unk0x145 = nullptr;
    this->Unk0x124 = 0;
    this->Unk0x118 = 0;
    this->Unk0x11C = 0;
    this->Unk0x120 = 0;
    this->Unk0x110 = 0;
    this->Unk0xCE = nullptr;

    this->Unk0x128 = new LinkRefTab(8, 0);
    this->Music = new ZMusic();

    this->Unk0x10B = false;
    this->Unk0x15 = 0;
    this->Unk0x10A = false;
    this->Unk0x138 = nullptr;
    this->Unk0xFA = 0;
    this->Unk0x10C = 0;
    this->Unk0x19 = 0.0f;
    this->Unk0x4 = 0.0f;
    this->Unk0x9 = 0;
    this->Unk0xDF = false;
    this->SfxVolume = 0.5f;
    this->Unk0xEC = 0.0f;
    this->Unk0xE4 = 0.2f;
    this->MusicVolume = 0.5f;
    this->SpeechVolume = 0.1f;
    this->Unk0x8 = true;
    this->UseStreaming = true;
    this->Unk0xDB = 0;

    this->Unk0xCE = new BlockRefTab();

    this->Unk0xD2 = 0;
    this->Unk0x14D = 0;
    this->Unk0xE0 = 8;
    this->Unk0xD6 = 16384;
    this->Unk0xDA = false;
    this->SpatialStreamingBuffers = 0;
    this->Unk0xC9 = false;
    this->Unk0xF8 = false;
    this->Unk0x13C = 0;

    this->Unk0xCA = new LinkRefTab(16, 0);
}

// 0x0ff3aa70
// 0x0ff3aaf0
ZSoundBase::~ZSoundBase() {}

// 0x0ff35860
bool ZSoundBase::Method0x30() {
    return this->Unk0x10A;
}

// 0x0ff358e0
BlockRefTab* ZSoundBase::Method0x180() {
    return this->Unk0xCE;
}

// 0x0ff358f0
bool ZSoundBase::Method0x184() {
    return this->Unk0xDA;
}

// 0x0ff35900
bool ZSoundBase::Method0x1BC() {
    return this->Unk0xF8;
}

// 0x0ff3ab80
void ZSoundBase::Method0x1A0(u32 todo) {
    this->Unk0xE0 = todo;
}

// 0x0ff3ab90
void ZSoundBase::Method0x1A4(u32 todo) {
    this->Unk0xD6 = todo;
}

// 0x0ff3aba0
void ZSoundBase::Method0x1B8(u32 todo) {
    this->Unk0x14D = todo;

    void* todo_1 = FUN_0ff3a690(g_pSysInterface->SoundModule->Unk0x14);

    if (todo_1 != nullptr) {
        todo_1->Unk0x38 = this->Unk0x14D;
    }
}

// 0x0ff3abe0
void ZSoundBase::Method0x1A8(bool todo) {
    this->Unk0x140 = todo;
}

// 0x0ff3aab0
s32 ZSoundBase::Method0x1AC() {
    if (this->SpatialStreamingBuffers != 0) {
        return this->SpatialStreamingBuffers - 2;
    }

    return 0;
}

// 0x0ff3aad0
bool ZSoundBase::Method0x1B0() {
    return this->Unk0xC9;
}

// 0x0ff3aae0
bool ZSoundBase::HasMusic() {
    return this->Music != nullptr;
}

// 0x0ff3adc0
void ZSoundBase::Method0x178(u32 todo) {
    this->Unk0xDB = todo;
}

// 0x0ff3add0
void* ZSoundBase::Method0x17C() {
    void* todo_1 = FUN_0ff3a690(this->Unk0xDB);

    if (todo_1 != nullptr) {
        return todo_1;
    }

    return FUN_0ff3a690(g_pSysInterface->Render->Method0x7C(0)); // TODO
}

// 0x0ff3ae10
ZSoundListener* ZSoundBase::GetListener() {
    return this->Unk0x138;
}

// 0x0ff3ae20
void ZSoundBase::SetStreaming(bool value) {
    this->UseStreaming = value;
}

// 0x0ff3c180
void ZSoundBase::SetSpeechVolume(f32 volume, bool todo) {
    this->Unk0xF8 = todo;
    this->SpeechVolume = volume * 0.01f;
}

// 0x0ff3c1a0
void ZSoundBase::SetMusicVolume(f32 volume) {
    this->MusicVolume = volume * 0.01f;

    if (this->Music != nullptr) {
        this->Music->SetVolume(this->MusicVolume * (1.0f - this->Unk0xEC));
    }
}

// 0x0ff3c210
void ZSoundBase::SetMusicQuality(s32 value) {
    if (this->Music != nullptr) {
        this->Music->SetQuality(value == 0 ? ZMUSICQUALITY_LOW : ZMUSICQUALITY_NORMAL);
    }
}

// 0x0ff3c250
void ZSoundBase::SetSfxVolume(f32 volume) {
    this->SfxVolume = volume * 0.01f;
}

// 0x0ff3d5b0
bool ZSoundBase::EndInit(const char*) {
    return true;
}

// 0x0ff3d5b0
bool ZSoundBase::Method0x54(u32) {
    return true;
}

// 0x0ff3d6a0
bool ZSoundBase::Method0x38() {
    if (this->Unk0x140) {
        // TODO NOT IMLEMETED
    }

    return true;
}

// 0x0ff3dbd0
void ZSoundBase::Method0x14C() {
    // TODO NOT IMLEMETED
}

// 0x0ff3e550
void ZSoundBase::PrintStatus() {
    g_pSysCom->Log("Z:\\Engine\\Sound\\Source\\ZSoundCon.cpp", 2050)
        ->LogMessage("SoundDll Status:");

    g_pSysCom->Log("Z:\\Engine\\Sound\\Source\\ZSoundCon.cpp", 2051)
        ->LogMessage("---------------------------");

    g_pSysCom->Log("Z:\\Engine\\Sound\\Source\\ZSoundCon.cpp", 2052)
        ->LogMessage("Wave data size %d", this->WaveDataSize / 1024);
}

// 0x0ff3e6a0
void ZSoundBase::Method0x1C0() {
    switch (param_3) {
    case 1: { // TODO
        this->Method0x1C4(&this->Unk0x151, param_2, 1);
        break;
    }
    case 2: { // TODO
        this->Method0x1C4(&this->Unk0x1F9, param_2, 1);
        break;
    }
    case 3: { // TODO
        this->Method0x1C4(&this->Unk0x2A1, param_2, 1);
        break;
    }
    case 4: { // TODO
        this->Method0x1C4(&this->Unk0x349, param_2, 1);
        break;
    }
    }
}

// 0x0ff3ed30
void ZSoundBase::Method0x18C() {
    if (this->Unk0x145 != nullptr) {
        this->Unk0x145->Method0x1C(0);// TODO
    }
}
