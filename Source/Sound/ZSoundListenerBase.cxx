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

#include <System/ZSoundListener.hxx>

// 0x0ff50e40
ZSoundListenerBaseBase::ZSoundListenerBaseBase() {
    this->Unk0x1C = 0;
    this->Unk0x10 = 0;
    this->Unk0x14 = 0;
    this->Unk0x18 = 0;
    this->Unk0x4 = 0;
    this->Unk0x8 = 0;
    this->Unk0xC = 0;
}

// 0x0ff38f40
ZSoundListenerBaseBase::~ZSoundListenerBaseBase() {}

// 0x0ff38ec0
// 0x0ff50e50
ZSoundListenerBase::ZSoundListenerBase() {
    this->Unk0x24 = 0; // TODO
}

// 0x0ff35932
bool ZSoundListenerBase::SetRoomSize(f32) {
    return true;
}

// 0x0ff35930
void ZSoundListenerBase::SetRoom(DWORD) {}

// 0x0ff35940
u32 ZSoundListenerBase::Method0x24() {
    return this->Unk0x24;
}

// 0x0ff38ef0
ZSoundListenerBase::~ZSoundListenerBase() {}

// 0x0ff38f30
void ZSoundListenerBase::Method0x20(u32 todo) {
    this->Unk0x24 = todo;
}
