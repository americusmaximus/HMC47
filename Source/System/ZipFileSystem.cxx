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

#include "ZipIO.hxx"

#include <zlib.h>

#define GET_ZIPIO()             ((ZipIO*)((size_t)this - sizeof(char*) - sizeof(u32) - sizeof(void*)))
#define GET_ZIPIO_MODE()        (*(u32*)((size_t)this - sizeof(char*) - sizeof(u32)))
#define GET_ZIPIO_FILE_NAME()   ((char*)((size_t)this - sizeof(char*)))

// 0x0ffc3490
// 0x0ffd426c
// 0x0ffd42d0
ZipFileSystem::ZipFileSystem() {
    this->Init = false;

    this->Compression = Z_DEFAULT_COMPRESSION;
    this->Unk0x15 = 0;

    this->Offsets.Initialize(256); // TODO
}

// 0x0ffc27d0
void ZipFileSystem::PrintStatus() {
    if (this->Init) {
        printf("fs: '%s' Offset = %i, status = %i\n",
            GET_ZIPIO_FILE_NAME(), ftell(this->Handle), this->Status);
    }
    else {
        printf("fs not initialized\n");
    }
}

// 0x0ffc2810
bool ZipFileSystem::Copy(const char* src, const char* dst) {
    if (GET_ZIPIO_MODE() != ZIPIO_MODE_READ_WRITE) {
        printf("IOZIP: Cannot add file - filesystem not writable\n");
        return false;
    }

    FILE* f = fopen(src, "rb");

    if (f == nullptr) {
        return false;
    }

    fseek(f, 0, SEEK_END);
    const long size = ftell(f);

    if (size != 0) {
        void* buffer = new byte[size];

        if (buffer == nullptr) {
            printf("ZIPFS: Cannot allocate buffer space for file '%s'\n", src);
            return false;
        }

        fseek(f, 0, SEEK_SET);
        fread(buffer, 1, size, f);

        GET_ZIPIO()->SaveFile(dst, nullptr, buffer, size);

        delete[] buffer;
    }

    return true;
}

// 0x0ffc2920
void ZipFileSystem::Save(const char*) {
    printf("ZIPFS: Save: This method is unsupported by this filesystem\n");
}

// 0x0ffc2930
u32 ZipFileSystem::GetSize(const char* path) {
    ZIPLFHV desc;

    if (GET_ZIPIO()->FindFile(path, &desc, nullptr)) {
        return desc.UncompressedSize;
    }

    return INVALID_FILE_SIZE;
}

// 0x0ffc2ac0
bool ZipFileSystem::Exists(const char* path) {
    ZIPLFHV desc;
    return GET_ZIPIO()->FindFile(path, &desc, nullptr);
}

// 0x0ffc2ae0
u32 ZipFileSystem::Unpack(const char* path, void* value, u32 size) {
    ZIPLFHV desc;

    if (GET_ZIPIO()->FindFile(path, &desc, nullptr)) {
        if (size == 0) {
            size = desc.UncompressedSize;
        }

        if (GET_ZIPIO()->Unpack(&desc, value, nullptr, size) != nullptr) {
            return size;
        }
    }

    return INVALID_FILE_SIZE;
}

// 0x0ffc29a0
void ZipFileSystem::Method0x18( /* TODO */) {
    // TODO NOT IMPLEMENTED
}

// 0x0ffd42d0
FileSystem::FileSystem() {}