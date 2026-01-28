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

#include "BigIO.hxx"
#include "Globals.hxx"

// 0x0ffc16b0
BigIO::BigIO() {}

// 0x0ffc1670
BigIO::~BigIO() {}

// 0x0ffc16c0
bool BigIO::Exists(const char* path) {
    return g_pSysFile->Exists(path, false);
}

// 0x0ffc16e0
u32 BigIO::Method0xC(u32) {
    return 0;
}

// 0x0ffc16f0
u32 BigIO::Method0x14(u32, u32, u32, u32) {
    return 0;
}

// 0x0ffc1700
void BigIO::Method0x4(u32, u32) {
    // NOT IMPLEMENTED
}

// 0x0ffc74e0
void BigIO::Method0x0() {}

// 0x0ffc74f0
void BigIO::Method0x8(u32) {}
