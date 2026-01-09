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

#include "ZTextureManagerD3D.hxx"

// 0x0fb971d0
// 0x0fbb5460
ZTextureManagerD3D::ZTextureManagerD3D() {}

// 0x0fb97200
// 0x0fb97220
ZTextureManagerD3D::~ZTextureManagerD3D() {}

// 0x0fb971e0
bool ZTextureManagerD3D::SupportsCompression() {
    return this->Unk0xC != 0; // TODO
}

// 0x0fb971f0
bool ZTextureManagerD3D::SupportsEMBM() {
    return this->Unk0x144 != 0; // TODO
}