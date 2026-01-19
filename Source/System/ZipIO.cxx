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

#define MAX_ZIP_STRING_LENGTH       65540

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
        CDFHV& header = this->ZFS.Unk0x46.Get(i);

        fwrite(&signature, 1, sizeof(u32), this->ZFS.Handle);
        fwrite(&header, 1, sizeof(CDFHV), this->ZFS.Handle);

        const long offset = ftell(this->ZFS.Handle);

        fseek(this->ZFS.Handle, header.LocalHeaderOffset + sizeof(LFH), SEEK_SET);
        fread(name, 1, header.FileNameLength, this->ZFS.Handle);
        fseek(this->ZFS.Handle, offset, SEEK_SET);
        fwrite(name, 1, header.FileNameLength, this->ZFS.Handle);

        this->ZFS.Rune.EntriesOnDisk++;
    }

    this->ZFS.Rune.TotalEntries = this->ZFS.Rune.EntriesOnDisk;

    {
        const long offset = ftell(this->ZFS.Handle);
        this->ZFS.Rune.DirectorySize = offset - this->ZFS.Rune.DirectoryOffset;
        this->ZFS.Directory.DirectoryOffset = offset;
    }

    for (u32 i = 0; i < this->ZFS.Unk0x3A.Count; i++) {
        CDFHV& header = this->ZFS.Unk0x3A.Get(i);

        fwrite(&signature, 1, sizeof(u32), this->ZFS.Handle);
        fwrite(&header, 1, sizeof(CDFHV), this->ZFS.Handle);

        const long offset = ftell(this->ZFS.Handle);

        fseek(this->ZFS.Handle, header.LocalHeaderOffset + sizeof(LFH), SEEK_SET);
        fread(name, 1, header.FileNameLength, this->ZFS.Handle);
        fseek(this->ZFS.Handle, offset, SEEK_SET);
        fwrite(name, 1, header.FileNameLength, this->ZFS.Handle);

        this->ZFS.Directory.EntriesOnDisk++;
    }

    this->ZFS.Directory.TotalEntries = this->ZFS.Directory.EntriesOnDisk;
    this->ZFS.Directory.DirectorySize = ftell(this->ZFS.Handle) - this->ZFS.Directory.DirectoryOffset;
}

// 0x0ffc2270
void ZipIO::ReadZipFile() {
    char name[MAX_ZIP_STRING_LENGTH];
    char extras[MAX_ZIP_STRING_LENGTH];

    fseek(this->ZFS.Handle,
        this->ZFS.Rune.DirectorySize == 0
        ? this->ZFS.Directory.DirectoryOffset : this->ZFS.Rune.DirectoryOffset, SEEK_SET);

    while (feof(this->ZFS.Handle) == 0) {
        bool cache = false;
        u32 signature = 0;
        fread(&signature, 1, sizeof(u32), this->ZFS.Handle);

        if (signature == ZIP_SIGNATURE_CDFH) {
            CDFHV file;

            const long offset = ftell(this->ZFS.Handle);
            fread(&file, 1, sizeof(CDFHV), this->ZFS.Handle);

            LFHV desc;
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
                    CopyMemory(&desc, &file.VersionNeeded, sizeof(LFHV));
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
                this->ZFS.Cache->Method0x4(name, &desc,
                    file.FileNameLength + sizeof(LFH) + file.LocalHeaderOffset);
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

    long offset = fseek(this->ZFS.Handle, offset, SEEK_SET) - sizeof(EOCD);

    while (offset >= 0) {
        u32 signature = 0;
        fread(&signature, 1, sizeof(u32), this->ZFS.Handle);

        if (signature == ZIP_SIGNATURE_RUNE) {
            fread(&this->ZFS.Rune, 1, sizeof(EOCDV), this->ZFS.Handle);
            fseek(this->ZFS.Handle, -sizeof(EOCDV), SEEK_CUR);
        }
        else if (signature == ZIP_SIGNATURE_EOCD) {
            this->ZFS.Offset = ftell(this->ZFS.Handle);
            fread(&this->ZFS.Directory, 1, sizeof(EOCDV), this->ZFS.Handle);
            return this->ZFS.Offset;
        }

        offset--;

        fseek(this->ZFS.Handle, -5, SEEK_CUR);
    }

    return INVALID_SET_FILE_POINTER;
}

// 0x0ffc2960
bool ZipIO::GetDateTime(const char* path, LPFILETIME time) {
    LFHV desc;

    if (this->FindFile(path, &desc, nullptr)) {
        DosDateTimeToFileTime(desc.LastModifiedDate, desc.LastModifiedTime, time);
        return true;
    }

    return false;
}

// 0x0ffc2e30
bool ZipIO::Open(const char* path, u32 mode) {
    if (this->Name != nullptr) {
        this->Save();
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

    ZeroMemory(&this->ZFS.Directory, sizeof(EOCDV));
    ZeroMemory(&this->ZFS.Rune, sizeof(EOCDV));

    this->GetDirectoryOffset();

    this->ZFS.Cache = new ZipFileSystemCache();

    if (mode != ZIPIO_MODE_READ_WRITE) {
        this->ReadZipFile();
    }

    return true;
}

// 0x0ffc31d0
void ZipIO::Save() {
    if (this->Name != nullptr) {
        this->WriteZipFile();

        u32 signature = ZIP_SIGNATURE_EOCD;
        fwrite(&signature, 1, sizeof(u32), this->ZFS.Handle);
        fwrite(&this->ZFS.Directory, 1, sizeof(EOCDV), this->ZFS.Handle);

        signature = ZIP_SIGNATURE_RUNE;
        fwrite(&signature, 1, sizeof(u32), this->ZFS.Handle);
        fwrite(&this->ZFS.Rune, 1, sizeof(EOCDV), this->ZFS.Handle);

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

// 0x0ffc3820
bool ZipIO::Reopen() {
    char name[MAX_PATH + 1];

    strncpy(name, this->Name, MAX_PATH);

    delete[] this->Name;
    this->Name = nullptr;

    return this->Open(name, this->Mode);
}
