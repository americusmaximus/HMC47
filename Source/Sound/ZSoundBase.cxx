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

#include <System/ZSoundBase.hxx>

// 0x0ff3a7e0
// 0x0ff51024
ZSoundBase::ZSoundBase() {
    this->Unk0x149 = 0;
    this->Unk0x10A = 0;
    this->Unk0x12C = 0;
    this->Unk0x145 = 0;
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
    this->Unk0x138 = 0;
    this->Unk0xFA = 0;
    this->Unk0x10C = 0;
    this->Unk0x19 = 0;
    this->Unk0x4 = 0;
    this->Unk0x9 = 0;
    this->Unk0xDF = false;
    this->SfxVolume = 0.5f;
    this->Unk0xEC = 0;
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

// 0x0ff35900
bool ZSoundBase::Method0x1BC() {
    return this->Unk0xF8;
}

// 0x0ff3aad0
bool ZSoundBase::Method0x1B0() {
    return this->Unk0xC9;
}

// 0x0ff3aae0
bool ZSoundBase::HasMusic() {
    return this->Music != nullptr;
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
        // TODO NOT IMPLEMENTED
    }
}

// 0x0ff3c210
void ZSoundBase::SetMusicQuality(s32 value) {
    if (this->Music != nullptr) {
        // TODO NOT IMPLEMENTED
    }
}

// 0x0ff3c250
void ZSoundBase::SetSfxVolume(f32 volume) {
    this->SfxVolume = volume * 0.01f;
}

// 0x0ff3d5b0
BOOL ZSoundBase::EndInit(const char*) {
    return TRUE;
}

// 0x0ff3d5b0
BOOL ZSoundBase::Method0x54(u32) {
    return TRUE; // TODO
}

// 0x0ff3d6a0
bool ZSoundBase::Method0x38() {
    // TODO NOT IMLEMETED
}

// 0x0ff3dbd0
void ZSoundBase::Method0x14C() {
    // TODO NOT IMLEMETED
}
