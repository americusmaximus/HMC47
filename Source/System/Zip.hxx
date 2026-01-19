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

#pragma once

#include "Common.hxx"

#define ZIP_SIGNATURE           0x04034B50
#define ZIP_SIGNATURE_CDH       0x02014B50
#define ZIP_SIGNATURE_EOCD      0x06054B50
#define ZIP_SIGNATURE_RUNE      0x52756E65

#define ZIP_VERSION_2_0         0x14
#define ZIP_VERSION_CREATED     0x616

#pragma pack(push, 1)

struct ZIPLFHV {
    u16 VersionNeeded;
    u16 Flags;
    u16 CompressionMethod;
    u16 LastModifiedTime;
    u16 LastModifiedDate;
    u32 CRC32;
    u32 CompressedSize;
    u32 UncompressedSize;
    u16 FileNameLength;
    u16 ExtraFieldLength;
    // char FileName[FileNameLength]
    // u8 ExtraField[ExtraFieldLength]
};

struct ZIPLFH {
    u32 Signature;
    ZIPLFHV Value;
};

struct ZIPCDHV {
    u16 VersionCreated;
    u16 VersionNeeded;
    u16 Flags;
    u16 CompressionMethod;
    u16 LastModifiedTime;
    u16 LastModifiedDate;
    u32 CRC32;
    u32 CompressedSize;
    u32 UncompressedSize;
    u16 FileNameLength;
    u16 ExtraFieldLength;
    u16 CommentLength;
    u16 DiskNumberStart;
    u16 InternalAttribute;
    u32 ExternalAttribute;
    u32 LocalHeaderOffset;
    // char FileName[FileNameLength]
    // u8 ExtraField[ExtraFieldLength]
    // char Comment[CommentLength]
};

struct ZIPCDH {
    u32 Signature;
    ZIPCDHV Value;
};

struct ZIPEOCDV {
    u16 DiskNumber;
    u16 StartDiskNumber;
    u16 EntriesOnDisk;
    u16 TotalEntries;
    u32 DirectorySize;
    u32 DirectoryOffset;
    u16 CommentLength;
    //char Comment[CommentLength];
};

struct ZIPEOCD {
    u32 Signature;
    ZIPEOCDV Value;
};

#pragma pack(pop)

#if defined(_DEBUG) && !defined(_WIN64)
static_assert(sizeof(ZIPCDH)    == 0x2E,    "ZIPCDH size mismatch.");
static_assert(sizeof(ZIPCDHV)   == 0x2A,    "ZIPCDHV size mismatch.");
static_assert(sizeof(ZIPEOCD)   == 0x16,    "ZIPEOCD size mismatch.");
static_assert(sizeof(ZIPLFH)    == 0x1E,    "ZIPLFH size mismatch.");
static_assert(sizeof(ZIPLFHV)   == 0x1A,    "ZIPLFHV size mismatch.");
#endif
