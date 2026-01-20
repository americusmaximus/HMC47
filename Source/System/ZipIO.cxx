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
#include "ZipIO.hxx"

#include <time.h>

#define MAX_ZIP_STRING_LENGTH           65540
#define MAX_ZIP_UNPACK_BUFFER_LENGTH    1024
#define MAX_ZIP_PACK_BUFFER_LENGTH      0x4000

// 0x0ffc3490
// 0x0ffd4288
ZipIO::ZipIO() {
    this->Name = nullptr;
    this->Mode = ZIPIO_MODE_READ_ONLY;
}

// 0x0ffc1990
void ZipIO::WriteZipFile() {
    this->ZFS.Rune.DirectoryOffset = ftell(this->ZFS.Handle);

    u32 signature = 0;
    char name[MAX_ZIP_STRING_LENGTH];

    for (u32 i = 0; i < this->ZFS.Unk0x46.Count; i++) {
        ZIPCDHV& header = this->ZFS.Unk0x46.Get(i);

        fwrite(&signature, 1, sizeof(u32), this->ZFS.Handle);
        fwrite(&header, 1, sizeof(ZIPCDHV), this->ZFS.Handle);

        const long offset = ftell(this->ZFS.Handle);

        fseek(this->ZFS.Handle, header.LocalHeaderOffset + sizeof(ZIPLFH), SEEK_SET);
        fread(name, 1, header.FileNameLength, this->ZFS.Handle);
        fseek(this->ZFS.Handle, offset, SEEK_SET);
        fwrite(name, 1, header.FileNameLength, this->ZFS.Handle);

        this->ZFS.Rune.EntriesOnDisk++;
    }

    this->ZFS.Rune.TotalEntries = this->ZFS.Rune.EntriesOnDisk;

    {
        const long offset = ftell(this->ZFS.Handle);
        this->ZFS.Rune.DirectorySize = offset - this->ZFS.Rune.DirectoryOffset;
        this->ZFS.Central.DirectoryOffset = offset;
    }

    for (u32 i = 0; i < this->ZFS.Unk0x3A.Count; i++) {
        ZIPCDHV& header = this->ZFS.Unk0x3A.Get(i);

        fwrite(&signature, 1, sizeof(u32), this->ZFS.Handle);
        fwrite(&header, 1, sizeof(ZIPCDHV), this->ZFS.Handle);

        const long offset = ftell(this->ZFS.Handle);

        fseek(this->ZFS.Handle, header.LocalHeaderOffset + sizeof(ZIPLFH), SEEK_SET);
        fread(name, 1, header.FileNameLength, this->ZFS.Handle);
        fseek(this->ZFS.Handle, offset, SEEK_SET);
        fwrite(name, 1, header.FileNameLength, this->ZFS.Handle);

        this->ZFS.Central.EntriesOnDisk++;
    }

    this->ZFS.Central.TotalEntries = this->ZFS.Central.EntriesOnDisk;
    this->ZFS.Central.DirectorySize = ftell(this->ZFS.Handle) - this->ZFS.Central.DirectoryOffset;
}

// 0x0ffc1b70
void ZipIO::Pack(ZIPCDHV* dir, void* value) {
    if (dir->CompressionMethod == Z_NO_COMPRESSION) {
        if (fwrite(value, 1, dir->UncompressedSize, this->ZFS.Handle) < dir->UncompressedSize) {
            printf("ZIPFS: Write error while trying to store file data\n");
        }

        dir->CompressedSize = dir->UncompressedSize;
        return;
    }

    if (dir->CompressionMethod != Z_DEFLATED) {
        printf("ZIPFS: Unrecognized compression method selected");
        return;
    }

    z_stream stream;
    ZeroMemory(&stream, sizeof(z_stream));

    stream.next_in = (byte*)value;
    stream.avail_in = dir->UncompressedSize;

    if (deflateInit(&stream, this->ZFS.Compression) != Z_OK) {
        printf("ZIPFS: Cannot initialize compression library\n");
        return;
    }

    s32 err = Z_OK;
    byte buffer[MAX_ZIP_PACK_BUFFER_LENGTH];
    u32 offset = 2; // Skip the first 2 bytes of zlib deflate stream.

    do {
        stream.next_out = buffer;
        stream.avail_out = MAX_ZIP_PACK_BUFFER_LENGTH;

        err = deflate(&stream, Z_FINISH);

        const u32 size = MAX_ZIP_PACK_BUFFER_LENGTH - stream.avail_out - offset;

        dir->CompressedSize += size;

        if (fwrite(&buffer[offset], 1, size, this->ZFS.Handle) != size) {
            deflateEnd(&stream);
            printf("ZIPFS: Write error: Cannot write to archive\n");
        }

        offset = 0;
    } while (err != Z_STREAM_END);

    deflateEnd(&stream);
}

inline void GetLocalTime(u16* date, u16* time) {
    time_t* ts;
    ::time(ts);
    tm* lt = localtime(ts);

    if (date != nullptr) {
        *date = ((lt->tm_year + 48) << 9) | ((lt->tm_mon + 1) << 5) | (lt->tm_mday);
    }

    if (time != nullptr) {
        *time = (lt->tm_hour << 11) | (lt->tm_min << 5) | (lt->tm_sec / 2);
    }
}

// 0x0ffc1cd0
void ZipIO::SaveFile(const char* path, LPFILETIME time, void* value, u32 size) {
    char name[MAX_ZIP_STRING_LENGTH];

    for (u32 i = 0; path[i] != NULL; i++) {
        name[i] = path[i] == '\\' ? '/' : path[i];
    }

    ZIPCDHV& dir = this->ZFS.Unk0x15
        ? this->ZFS.Unk0x46.Insert() : this->ZFS.Unk0x3A.Insert();

    ZeroMemory(&dir, sizeof(ZIPCDHV));

    dir.UncompressedSize = size;
    dir.VersionNeeded = ZIP_VERSION_2_0;
    dir.CompressionMethod = Z_DEFLATED;
    dir.FileNameLength = strlen(name);

    if (time == nullptr) {
        GetLocalTime(&dir.LastModifiedDate, &dir.LastModifiedDate);
    }
    else {
        if (!FileTimeToDosDateTime(time, &dir.LastModifiedDate, &dir.LastModifiedTime)) {
            GetLocalTime(&dir.LastModifiedDate, &dir.LastModifiedDate);
        }
    }

    dir.CRC32 = crc32(0, nullptr, 0);
    dir.CRC32 = crc32(dir.CRC32, (byte*)value, size);

    const u32 start = ftell(this->ZFS.Handle);

    const u32 signature = ZIP_SIGNATURE;
    fwrite(&signature, 1, sizeof(u32), this->ZFS.Handle);
    fwrite(&dir.VersionNeeded, 1, sizeof(ZIPLFHV), this->ZFS.Handle);
    fwrite(name, 1, dir.FileNameLength, this->ZFS.Handle);

    this->Pack(&dir, value);

    if (dir.UncompressedSize <= dir.CompressedSize) {
        dir.CompressedSize = 0;
        dir.CompressionMethod = Z_NO_COMPRESSION;

        fseek(this->ZFS.Handle, start + sizeof(u32), SEEK_SET);
        fwrite(&dir.VersionNeeded, 1, sizeof(ZIPLFHV), this->ZFS.Handle);
        fwrite(name, 1, dir.FileNameLength, this->ZFS.Handle);

        this->Pack(&dir, value);
    }

    const u32 end = ftell(this->ZFS.Handle);

    fseek(this->ZFS.Handle, start + sizeof(u32), SEEK_SET);
    fwrite(&dir.VersionNeeded, 1, sizeof(ZIPLFHV), this->ZFS.Handle);
    fseek(this->ZFS.Handle, end, SEEK_SET);

    dir.LocalHeaderOffset = start;
    dir.VersionCreated = ZIP_VERSION_CREATED;
}

// 0x0ffc2130
void* ZipIO::Unpack(ZIPLFHV* desc, void* value, void*, u32 size) {
    if (desc->CompressionMethod == Z_NO_COMPRESSION) {
        fread(value, 1, size, this->ZFS.Handle);
    }
    else {
        if (desc->CompressionMethod != Z_DEFLATED) {
            printf("ZIPFS: unsupported compression method\n");
            return nullptr;
        }

        u32 bytes = 0;
        byte buffer[MAX_ZIP_UNPACK_BUFFER_LENGTH];

        z_stream stream;
        ZeroMemory(&stream, sizeof(z_stream));

        stream.next_out = (byte*)value;
        stream.avail_out = size;

        this->ZFS.Status = inflateInit2(&stream, -MAX_WBITS);

        if (this->ZFS.Status != Z_OK) {
            return nullptr;
        }

        for (u32 i = 0; i < desc->CompressedSize; i += bytes) {
            stream.next_in = buffer;

            bytes = MAX_ZIP_UNPACK_BUFFER_LENGTH;
            if (desc->CompressedSize - i < MAX_ZIP_UNPACK_BUFFER_LENGTH) {
                bytes = desc->CompressedSize - i;
            }

            stream.avail_in = fread(buffer, 1, bytes, this->ZFS.Handle);
            this->ZFS.Status = inflate(&stream,
                bytes < desc->CompressedSize - i ? Z_PARTIAL_FLUSH : Z_FINISH);
        }

        inflateEnd(&stream);

        if (this->ZFS.Status != Z_STREAM_END
            && (this->ZFS.Status != Z_BUF_ERROR || stream.avail_out != 0)) {
            return nullptr;
        }
    }

    return value;
}

// 0x0ffc2270
void ZipIO::ReadZipFile() {
    char name[MAX_ZIP_STRING_LENGTH];
    char extras[MAX_ZIP_STRING_LENGTH];

    fseek(this->ZFS.Handle,
        this->ZFS.Rune.DirectorySize == 0
        ? this->ZFS.Central.DirectoryOffset : this->ZFS.Rune.DirectoryOffset, SEEK_SET);

    while (feof(this->ZFS.Handle) == 0) {
        bool cache = false;
        u32 signature = 0;
        fread(&signature, 1, sizeof(u32), this->ZFS.Handle);

        if (signature == ZIP_SIGNATURE_CDH) {
            ZIPCDHV file;
            const long offset = ftell(this->ZFS.Handle);
            fread(&file, 1, sizeof(ZIPCDHV), this->ZFS.Handle);

            ZIPLFHV desc;
            if (file.FileNameLength != 0) {
                fread(name, 1, file.FileNameLength, this->ZFS.Handle);
                name[file.FileNameLength] = NULL;

                bool found = false;

                for (u32 i = 0; i < this->ZFS.Offsets.Count; i++) {
                    if (offset == this->ZFS.Offsets.Get(i)) {
                        found = true;
                        break;
                    }
                }

                if (!found) {
                    CopyMemory(&desc, &file.VersionNeeded, sizeof(ZIPLFHV));
                    cache = true;
                }
            }

            if (file.ExtraFieldLength != 0) {
                fread(extras, 1, file.ExtraFieldLength, this->ZFS.Handle);
            }

            if (file.CommentLength != 0) {
                fread(extras, 1, file.CommentLength, this->ZFS.Handle);
            }

            if (cache) {
                this->ZFS.Cache->Insert(name, &desc,
                    file.FileNameLength + sizeof(ZIPLFH) + file.LocalHeaderOffset);
            }
        }
        else {
            if (signature == ZIP_SIGNATURE_EOCD || signature == ZIP_SIGNATURE_RUNE) {
                return;
            }

            g_pSysCom->Log("Z:\\Engine\\ZStdLib\\Source\\IOZip.cpp", 491)
                ->LogMessage("ZIPFS: Broken .zip archive\n");
        }
    }
}

// 0x0ffc2710
u32 ZipIO::GetDirectoryOffset() {
    fseek(this->ZFS.Handle, 0, SEEK_END);

    long offset = fseek(this->ZFS.Handle, offset, SEEK_SET) - sizeof(ZIPEOCD);

    while (offset >= 0) {
        u32 signature = 0;
        fread(&signature, 1, sizeof(u32), this->ZFS.Handle);

        if (signature == ZIP_SIGNATURE_RUNE) {
            fread(&this->ZFS.Rune, 1, sizeof(ZIPEOCDV), this->ZFS.Handle);
            fseek(this->ZFS.Handle, -sizeof(ZIPEOCDV), SEEK_CUR);
        }
        else if (signature == ZIP_SIGNATURE_EOCD) {
            this->ZFS.Offset = ftell(this->ZFS.Handle);
            fread(&this->ZFS.Central, 1, sizeof(ZIPEOCDV), this->ZFS.Handle);
            return this->ZFS.Offset;
        }

        offset--;

        fseek(this->ZFS.Handle, -5, SEEK_CUR);
    }

    return INVALID_SET_FILE_POINTER;
}

// 0x0ffc2960
bool ZipIO::GetDateTime(const char* path, LPFILETIME time) {
    ZIPLFHV desc;

    if (this->FindFile(path, &desc, nullptr)) {
        DosDateTimeToFileTime(desc.LastModifiedDate, desc.LastModifiedTime, time);
        return true;
    }

    return false;
}

// 0x0ffc2b40
ZipIOContext* ZipIO::OpenFile(const char* path, u32 mode) {
    ZipIOContext* result = new ZipIOContext();

    if (result != nullptr) {
        result->CurrentOffset = 0;
        result->Mode = mode;

        if (mode == 3 /* TODO */) {
            if (this->FindFile(path, &result->Header, nullptr)) {
                const u32 offset = ftell(this->ZFS.Handle);

                result->CurrentOffset = offset;
                result->Unk0x22 = offset;
                
                result->Size = result->Header.CompressedSize;

                ZeroMemory(&result->Status, sizeof(z_stream));
                
                result->Stream.next_in = result->Value;
                result->Status = inflateInit2(&result->Stream, -MAX_WBITS);

                if (result->Status == Z_OK) {
                    return result;
                }
            }
        }

        delete result;
    }

    return nullptr;
}

// 0x0ffc2c30
u32 ZipIO::ReadFile(ZipIOContext* context, void* value, u32 size) {
    fseek(this->ZFS.Handle, context->CurrentOffset, SEEK_SET);

    if (context->Header.CompressionMethod == Z_NO_COMPRESSION) {
        fseek(this->ZFS.Handle, context->CurrentOffset, SEEK_SET);
        return fread(value, 1, size, this->ZFS.Handle);
    }

    if (context->Header.CompressionMethod != Z_DEFLATED) {
        printf("ZIPFS: unsupported compression method\n");
        return size - context->Stream.avail_out;
    }

    if (context->Status != Z_OK) {
        return context->Status != Z_STREAM_END ? INVALID_FILE_SIZE : 0;
    }

    context->Stream.next_out = (byte*)value;
    context->Stream.avail_out = size;

    if (size != 0) {
        do {
            if (context->Status != Z_OK) {
                break;
            }

            u32 offset =
                ZIPIO_CONTEXT_BUFFER_LENGTH - context->Stream.avail_in;

            CopyMemory(context->Value, &context->Value[offset], ZIPIO_CONTEXT_BUFFER_LENGTH - offset);

            const u32 length = context->Header.CompressedSize - context->CurrentOffset;

            if (length < offset) {
                offset = length;
            }

            if (offset != 0) {
                context->Stream.avail_in = fread(context->Value, 1, offset, this->ZFS.Handle);
            }

            context->Stream.next_in = context->Value;
            context->Status = inflate(&context->Stream, Z_FULL_FLUSH);

            if (context->Status != Z_OK && context->Status != Z_STREAM_END) {
                printf("dut");
            }
        } while (context->Stream.avail_out != 0);
    }

    context->CurrentOffset = ftell(this->ZFS.Handle);

    return size - context->Stream.avail_out;
}

// 0x0ffc2db0
u32 ZipIO::WriteFile(ZipIOContext*, void*, u32) {
    printf("ZIPFS: Stream write not implemented yet\n");

    return INVALID_FILE_SIZE;
}

// 0x0ffc2dd0
void ZipIO::CloseFile(ZipIOContext* context) {
    inflateEnd(&context->Stream);
    delete context;
}

// 0x0ffc2e30
bool ZipIO::Initialize(const char* path, u32 mode) {
    if (this->Name != nullptr) {
        this->Release();
    }

    this->ZFS.Handle = fopen(path, mode != ZIPIO_MODE_READ_WRITE ? "rb" : "wb+");

    if (this->ZFS.Handle == nullptr) {
        printf("ZIPFS: Cannot initialize archive\n");
        this->Mode = ZIPIO_MODE_READ_ONLY;
        return false;
    }

    this->Name = new char[strlen(path) + 1];
    strcpy(this->Name, path);

    this->ZFS.Init = true;
    this->Mode = mode;

    this->ZFS.Unk0x3A.Initialize(256); // TODO
    this->ZFS.Unk0x46.Initialize(8); // TODO

    ZeroMemory(&this->ZFS.Central, sizeof(ZIPEOCDV));
    ZeroMemory(&this->ZFS.Rune, sizeof(ZIPEOCDV));

    this->GetDirectoryOffset();

    this->ZFS.Cache = new ZipFileSystemCache();

    if (mode != ZIPIO_MODE_READ_WRITE) {
        this->ReadZipFile();
    }

    return true;
}

// 0x0ffc31d0
void ZipIO::Release() {
    if (this->Name != nullptr) {
        this->WriteZipFile();

        u32 signature = ZIP_SIGNATURE_EOCD;
        fwrite(&signature, 1, sizeof(u32), this->ZFS.Handle);
        fwrite(&this->ZFS.Central, 1, sizeof(ZIPEOCDV), this->ZFS.Handle);

        signature = ZIP_SIGNATURE_RUNE;
        fwrite(&signature, 1, sizeof(u32), this->ZFS.Handle);
        fwrite(&this->ZFS.Rune, 1, sizeof(ZIPEOCDV), this->ZFS.Handle);

        this->ZFS.Unk0x3A.Release();
        this->ZFS.Unk0x46.Release();

        delete[] this->Name;
        this->Name = nullptr;

        fclose(this->ZFS.Handle);
        
        this->ZFS.Init = false;
        this->Mode = ZIPIO_MODE_READ_ONLY;

        if (this->ZFS.Cache != nullptr) {
            delete this->ZFS.Cache;
            this->ZFS.Cache = nullptr;
        }
    }
}

// 0x0ffc32f0
bool ZipIO::Compare(const char* a, const char* b) {
    ZIPLFHV da, db;

    if (this->FindFile(a, &da, nullptr)) {
        if (this->FindFile(b, &da, nullptr)) {
            if (da.CompressedSize == db.CompressedSize) {
                return da.CRC32 == db.CRC32;
            }
        }
    }

    return false;
}

// 0x0ffc3580
void ZipIO::Append(const char* path, LPFILETIME time, void* value, u32 size) {
    this->SaveFile(path, time, value, size);
}

// 0x0ffc3760
void ZipIO::SetCompression(u32 level) {
    this->ZFS.Compression = level;
}

// 0x0ffc3770
void ZipIO::Close() {
    this->ZFS.Unk0x3A.Release();
    this->ZFS.Unk0x46.Release();
    
    this->ZFS.Offsets.Release();

    fclose(this->ZFS.Handle);

    this->ZFS.Init = false;

    if (this->ZFS.Cache != nullptr) {
        delete this->ZFS.Cache;
        this->ZFS.Cache = nullptr;
    }
}

// 0x0ffc3820
bool ZipIO::ReInitialize() {
    char name[MAX_PATH + 1];

    strncpy(name, this->Name, MAX_PATH);

    delete[] this->Name;
    this->Name = nullptr;

    return this->Initialize(name, this->Mode);
}
