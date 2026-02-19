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

#include <System/ZSound.hxx>

// 0x0ff50bd0
ZSoundListener::ZSoundListener() {
    this->SpatialBuffer = nullptr;
    this->Listener = nullptr;
}

// 0x0ff359b0
// 0x0ff359f0
ZSoundListener::~ZSoundListener() {
    if (this->SpatialBuffer != nullptr) {
        delete this->SpatialBuffer;
    }

    this->SpatialBuffer = nullptr;

    if (this->Listener != nullptr) {
        this->Listener->Release();
    }

    this->Listener = nullptr;
}

// 0x0ff35bb0
void ZSoundListener::Release() {
    if (this->SpatialBuffer != nullptr) {
        delete this->SpatialBuffer;
    }

    this->SpatialBuffer = nullptr;

    if (this->Listener != nullptr) {
        this->Listener->Release();
    }

    this->Listener = nullptr;
}

// 0x0ff35be0
bool ZSoundListener::Method0x4() {
    void* todo_1 = this->Sound->Method0x17C(); // TODO

    if (todo_1 == nullptr) {
        return false;
    }




    TODO
}

// 0x0ff35d40
bool ZSoundListener::SetRoomSize(f32 size) {
    if (this->SpatialBuffer == nullptr) {
        return false;
    }

    return this->SpatialBuffer->SetRoomSize(size);
}

// 0x0ff35a80
bool ZSoundListener::Initialize() {
    HRESULT hr = DS_OK;
    if (FAILED(hr = this->Buffer->QueryInterface(IID_IDirectSound3DListener, (LPVOID*)&this->Listener))) {
        DirectSoundLogMessage(hr, "Query listener failed");
        return false;
    }

    if (this->Sound->Method0x1CC()) {
        this->SpatialBuffer = new ZSpatialSoundBuffer();

        BlockRefTab* todo_1 = this->Sound->Method0x180(); // TODO

        TODO
    }

    this->SetRoom(EAX_ENVIRONMENT_GENERIC);

    return true;
}

// 0x0ff35d60
void ZSoundListener::Method0x20(u32 todo) {
    ZSoundListenerBase::Method0x20(todo);

    if (this->SpatialBuffer != nullptr) {
        this->SpatialBuffer->Method0x1C(todo);
    }
}

// 0x0ff35b90
void ZSoundListener::SetRoom(DWORD environment) {
    if (this->SpatialBuffer != nullptr) {
        this->SpatialBuffer->SetRoom(environment);
    }
}

// 0x0ff35a70
void ZSoundListener::SetBuffer(LPDIRECTSOUNDBUFFER buffer) {
    this->Buffer = buffer;
}
