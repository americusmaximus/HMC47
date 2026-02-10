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

#include "ZSoundBuffer.hxx"

// 0x0ff33ed0
// 0x0ff50a08
ZSoundBuffer::ZSoundBuffer() {
    ZeroMemory(&this->Properties, sizeof(EAXBUFFERPROPERTIES));

    this->Properties.flOcclusionLFRatio = 0.25f;
    this->Properties.flOcclusionRoomRatio = 1.5f;
}

// 0x0ff33f10
// 0x0ff33eb0
ZSoundBuffer::~ZSoundBuffer() {}

// 0x0ff33f50
void ZSoundBuffer::Method0x8() {}
