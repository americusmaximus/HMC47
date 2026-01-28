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
#include "ZipIO.hxx"

#include <stdio.h>

// 0x0ffa5290
ZSysFileBase::ZSysFileBase() {
    this->Blocked = nullptr;
}

// 0x0ffa52a0
// 0x0ffa52f0
ZSysFileBase::~ZSysFileBase() {
    if (this->Blocked != nullptr) {
        delete this->Blocked;
    }
}

// 0x0ffa5310
ZSysFile::ZSysFile() {
    this->Archives = nullptr;
    this->Modules = new LinkRefTab(32, 0);
    this->Files = new RefTab(8, 2);

    g_pSysFile = this;
}

// 0x0ffa53f0
// 0x0ffa5430
ZSysFile::~ZSysFile() {
    this->FUN_0ffa7d20(true);

    if (this->Files != nullptr) {
        RefLink link;
        this->Files->GetStart(&link);
        RefKeyValue* kv = this->Files->GetNext(&link);

        while (kv != nullptr) {
            // TODO NOT IMPLEMENTED

            kv = this->Files->GetNext(&link);
        }
    }

    this->Files->Clear();

    if (this->Modules != nullptr) {
        RefLink link;
        this->Modules->GetEnd(&link);

        RefKeyValue* kv = this->Modules->GetPrevious(&link);

        if (kv != nullptr) {
            // TODO NOT IMPLEMENTED

            kv = this->Modules->GetPrevious(&link);
        }
    }

    delete this->Modules;
    delete this->Files;

    g_pSysFile = nullptr;
}

// 0x0ffa55e0
HANDLE ZSysFile::Open(const char* path) {
    HANDLE file = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ, NULL,
        OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN | FILE_ATTRIBUTE_NORMAL, NULL);

    return (file != NULL && file != INVALID_HANDLE_VALUE) ? file : NULL;
}

// 0x0ffa5610
HANDLE ZSysFile::OpenForRandomAccess(const char* path) {
    HANDLE file = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ, 0,
        OPEN_EXISTING, FILE_FLAG_RANDOM_ACCESS | FILE_ATTRIBUTE_NORMAL, NULL);

    if (file != INVALID_HANDLE_VALUE) {
        g_pSysCom->Log("Z:\\Engine\\System\\_Wintel\\Source\\SysFileWintel.cpp", 75)
            ->LogMessage("ZSysFileWintel::OpenForRandomAccess: Couldn't open file\"%s\"", path);

        return NULL;
    }

    return file;
}

// 0x0ffa5660
HANDLE ZSysFile::OpenForAppend(const char* path) {
    HANDLE file = CreateFileA(path, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if (file == INVALID_HANDLE_VALUE) {
        g_pSysCom->Log("Z:\\Engine\\System\\_Wintel\\Source\\SysFileWintel.cpp", 86)
            ->LogMessage("ZSysFileWintel::OpenForAppend: Couldn't open file\"%s\"", path);

        return NULL;
    }

    if (SetFilePointer(file, 0, NULL, FILE_END) == INVALID_SET_FILE_POINTER) {
        g_pSysCom->Log("Z:\\Engine\\System\\_Wintel\\Source\\SysFileWintel.cpp", 90)
            ->LogMessage("ZSysFileWintel::OpenForAppend: Couldn't set FilePointer correctly for \"%s\"", path);

    }

    return file;
}

// 0x0ffa56f0
void ZSysFile::Close(HANDLE file) {
    if (CloseHandle(file)) {
        g_pSysCom->Log("Z:\\Engine\\System\\_Wintel\\Source\\SysFileWintel.cpp", 97)
            ->LogMessage("ZSysFileWintel::Close: Couldn't close file with handle %d", file);
    }
}

// 0x0ffa5730
const char* ZSysFile::GetFileName(const char* path) {
    if (this->Blocked != nullptr) {
        const char* ext = strrchr(path, '.');

        if (ext == nullptr) {
            return nullptr;
        }

        if (strcmpi(ext, ".mor") == 0) {
            ext = ".anm";
        }

        if (this->Blocked != nullptr) {
            RefLink link;
            this->Blocked->GetStart(&link);
            char* key = (char*)REFTAB_KEY_TO_PTR(this->Blocked->GetNextKey(&link));

            while (link.Next != nullptr) {
                if (_stricmp(ext, key) == 0) {
                    return nullptr;
                }

                key = (char*)REFTAB_KEY_TO_PTR(this->Blocked->GetNextKey(&link));
            }
        }
    }

    const char* result = path;
    const u32 length = g_pSysInterface->BasePath.GetLength();

    if (length <= strlen(path)) {
        if (_strnicmp(g_pSysInterface->BasePath.AsString(), result, length) == 0) {
            result += length;

            if (result[0] == '\\') {
                result++;
            }
        }
    }

    return result;
}

// 0x0ffa5ea0
void* ZSysFile::Method0x9C(const char* path) {
    RefLink link;

    if (path != nullptr && this->Archives != nullptr) {
        this->Archives->GetStart(&link);
        RefKeyValue* kv = this->Archives->GetNext(&link);

        while (kv != nullptr) {
            // TODO NOT IMPLEMENTED

            kv = this->Archives->GetNext(&link);
        }
    }

    return nullptr;
}

// 0x0ffa5f20
bool ZSysFile::GetTime(const char* path, LPFILETIME time, bool real) {
    if (real) {
        HANDLE file = this->Open(path);

        if (file == NULL) {
            return false;
        }

        FILETIME create, access;
        BOOL result = GetFileTime(file, &create, &access, time);

        this->Close(file);

        return result;
    }
    else {
        const char* name = this->GetFileName(path);

        // TODO NOT IMPLEMENTED
    }
}

// 0x0ffa5fb0
u32 ZSysFile::GetSize(const char* path, bool real) {
    if (real) {
        HANDLE file = this->Open(path);

        if (file == NULL) {
            return INVALID_FILE_SIZE;
        }

        const u32 size = GetFileSize(file, nullptr);

        if (size == INVALID_FILE_SIZE) {
            g_pSysCom->Log("Z:\\Engine\\System\\_Wintel\\Source\\SysFileWintel.cpp", 212)
                ->LogMessage("ZSysFileWintel::GetSize failed");
        }

        this->Close(file);

        return size;
    }
    else {
        const char* name = this->GetFileName(path);

        // TODO NOT IMPLEMENTED
    }
}

// 0x0ffa6060
HANDLE ZSysFile::Create(const char* path) {
    HANDLE file = CreateFileA(path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if (file == INVALID_HANDLE_VALUE) {
        g_pSysCom->Log("Z:\\Engine\\System\\_Wintel\\Source\\SysFileWintel.cpp", 225)
            ->LogMessage("ZSysFileWintel::Create: Couldn't create file\"%s\"", path);

        return NULL;
    }

    return file;
}

// 0x0ffa60b0
bool ZSysFile::Touch(const char* path) {
    HANDLE file = CreateFileA(path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if (file == INVALID_HANDLE_VALUE) {
        g_pSysCom->Log("Z:\\Engine\\System\\_Wintel\\Source\\SysFileWintel.cpp", 236)
            ->LogMessage("ZSysFileWintel::Create: Couldn't create file\"%s\"", path);

        return false;
    }

    this->Close(file);

    return true;
}

// 0x0ffa6120
bool ZSysFile::Exists(const char* path, bool real) {
    if (real) {
        WIN32_FIND_DATA find;
        HANDLE file = FindFirstFileA(path, &find);

        if (file != INVALID_HANDLE_VALUE) {
            FindClose(file);
            return true;
        }

        return false;
    }
    else {
        const char* name = this->GetFileName(path);

        // TODO NOT IMPLEMENTED
    }
}

// 0x0ffa61b0
bool ZSysFile::WriteTo(HANDLE file, const void* ptr, u32 size) {
    DWORD bytes = 0;

    if (!WriteFile(file, ptr, size, &bytes, NULL)) {
        g_pSysCom->Log("Z:\\Engine\\System\\_Wintel\\Source\\SysFileWintel.cpp", 277)
            ->LogMessage("ZSysFileWintel::WriteTo failed");

        return false;
    }

    return true;
}

// 0x0ffa6200
u32 ZSysFile::ReadFrom(HANDLE file, void* ptr, u32 size) {
    DWORD bytes = 0;

    if (!ReadFile(file, ptr, size, &bytes, NULL)) {
        char* buffer = nullptr;
        FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM
            | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ALLOCATE_BUFFER,
            nullptr, GetLastError(), LANG_USER_DEFAULT, buffer, 0, nullptr);

        MessageBoxA(NULL, buffer, "Error", MB_TOPMOST | MB_ICONASTERISK);

        g_pSysCom->Log("Z:\\Engine\\System\\_Wintel\\Source\\SysFileWintel.cpp", 302)
            ->LogMessage("ZSysFileWintel::ReadFrom failed %s", buffer);

        LocalFree(buffer);
    }

    return bytes;
}

// 0x0ffa62b0
void ZSysFile::CD(const char* path) {
    if (!SetCurrentDirectoryA(path)) {
        g_pSysCom->Log("Z:\\Engine\\System\\_Wintel\\Source\\SysFileWintel.cpp", 311)
            ->LogMessage("ZSysFileWintel::CD failed");
    }
}

// 0x0ffa62f0
void ZSysFile::MakeDir(const char* path) {
    char* dir = new char[strlen(path) + 1];
    strcpy(dir, path);

    char* slash = strrchr(dir, '\\');
    if (slash != nullptr) {
        *slash = NULL;
    }

    if (!this->Exists(dir, false)) {
        g_pSysCom->Log("Z:\\Engine\\System\\_Wintel\\Source\\SysFileWintel.cpp", 323)
            ->LogMessage("ZSysFileWintel::MakeDir failed");
    }

    delete[] dir;
}

// 0x0ffa63e0
void ZSysFile::Rename(const char* src, const char* dst) {
    if (!MoveFileA(src, dst)) {
        g_pSysCom->Log("Z:\\Engine\\System\\_Wintel\\Source\\SysFileWintel.cpp", 331)
            ->LogMessage("ZSysFileWintel::Rename failed");
    }
}

// 0x0ffa6420
void ZSysFile::Copy(const char* src, const char* dst) {
    if (!CopyFileA(src, dst, TRUE)) {
        g_pSysCom->Log("Z:\\Engine\\System\\_Wintel\\Source\\SysFileWintel.cpp", 337)
            ->LogMessage("ZSysFileWintel::Copy failed");
    }
}

// 0x0ffa6460
bool ZSysFile::Delete(const char* path) {
    return DeleteFileA(path);
}

// 0x0ffa6480
u32 ZSysFile::ReadAt(const char* path, void* ptr, u32 size, u32 offset, bool real) {
    if (real) {
        if (offset == 0) {
            HANDLE file = this->Open(path);

            if (file != NULL) {
                const u32 result = this->ReadFrom(file, ptr, size);
                this->Close(file);

                return result;
            }
        }
        else {
            FILE* file = fopen(path, "rb");

            if (file != nullptr) {
                fseek(file, offset, SEEK_SET);
                const u32 result = fread(ptr, 1, size, file);
                fclose(file);

                return result;
            }
        }
    }
    else {
        const char* name = this->GetFileName(path);

        // TODO NOT IMPLEMENTED
    }

    return INVALID_FILE_SIZE;
}

// 0x0ffa6c40
u32 ZSysFile::Method0x54(const char* path, u32* token) {
    *token = 0;

    if (this->Files != nullptr) {
        // TODO NOT IMPLEMENTED
    }

    const u32 length = this->GetSize(path, false);

    if (length == INVALID_FILE_SIZE) {
        return INVALID_FILE_SIZE;
    }

    char* name = new char[strlen(path) + 1];
    strcpy(name, path);

    void* value = new u8[length];
    this->ReadAt(path, value, length, 0, false);



    // TODO NOT IMPLEMENTED

    SysFileInfo* info = (SysFileInfo*)this->Files->Insert(key);

    info->Size = length;
    info->Name = name;

    *token = TODO;

    return length;
}

// 0x0ffa6e20
void ZSysFile::Method0x58(const char* path, const void* ptr, u32 size, u32 offset /* TODO */) {
    HANDLE file = this->Create(path);

    if (file != NULL) {
        if (offset != 0 && offset < size) {
            this->WriteTo(file, ptr, offset);
            this->Close(file);

            HANDLE append = this->OpenForAppend(path);

            // TODO NOT IMPLEMENTED

            this->Close(append);

            return;
        }

        this->WriteTo(file, ptr, size);
        this->Close(file);
    }
}

// 0x0ffa6f00
void ZSysFile::Append(const char* path, const void* ptr, u32 size) {
    HANDLE file = this->OpenForAppend(path);

    if (file != NULL) {
        this->WriteTo(file, ptr, size);
        this->Close(file);
    }
}

// 0x0ffc74e0
void ZSysFile::Method0x4() {}

// 0x0ffc74e0
void ZSysFile::Method0x8() {}

// 0x0ffa7020
void ZSysFile::BlockExtensions(const char* value) {
    if (this->Blocked = nullptr) {
        this->Blocked = new StringRefTab(8, 0);
    }

    const char* input = value == nullptr ? "" : value;

    const u32 length = strlen(input);
    char* extensions = new char[length + 1];

    strcpy(extensions, input);

    char* csv = extensions;

    while (true) {
        char* separator = strchr(csv, ';');

        // Depending on the separator search result do either or:
        // 1. Separator was found:
        //      1. Set the first character of separator to NULL.
        //          Value:      .abc;.xyz;.exe
        //          Separator:      ;.xyz;.exe
        //          Separator:       .xyz;.exe
        //      2. Insert string from the beginning to the beginning of the separator
        //              ->Insert(.abc)
        //      3. Set the string to the next character after the separator.
        //          Value:      .xyz;.exe
        // 2. Separator was not found - insert the whole string as it has only one element. Done.

        if (separator == nullptr) {
            this->Blocked->TryInsertString(csv);
            break;
        }
        else {
            separator[0] = NULL;
            this->Blocked->TryInsertString(csv);
            csv = &separator[1];
        }
    }

    delete[] extensions;
}

// 0x0ffa7500
LinkRefTab* ZSysFile::Method0x68(/* TODO*/) {
    // TODO NOT IMPLEMENTED
    return nullptr;
}

// 0x0ffa7750
void* ZSysFile::Method0xB0(const char* path) {
    const char* input = path == nullptr ? "" : path;
    char* name = new char[strlen(input) + 1];

    for (u32 i = 0; path[i] != NULL; i++) {
        name[i] = path[i] == '\\' ? '/' : path[i];
        name[i + 1] = NULL;
    }

    char* ext = strrchr(name, '.');

    if (ext != nullptr) {

        // TODO NOT IMPLEMENTED

    }

    delete[] name;

    return nullptr;
}

// 0x0ffa7df0
void ZSysFile::PrintStatus() {
    RefLink link;

    if (this->Archives != nullptr) {
        this->Archives->GetStart(&link);
        ZipIO* file = (ZipIO*)this->Archives->GetNext(&link);

        while (file != nullptr) {
            if (file->Mode & ZIPIO_MODE_WRITE) {
                if (file->ZFS.Exists("Pack.*")) {
                    g_pSysCom->Log("Z:\\Engine\\System\\_Wintel\\Source\\SysFileWintel.cpp", 844)
                        ->LogMessage("Zip file \"%s\" pack info", file->Name);
                }
                else {
                    g_pSysCom->Log("Z:\\Engine\\System\\_Wintel\\Source\\SysFileWintel.cpp", 846)
                        ->LogMessage("Zip file \"%s\"", file->Name);
                }
            }
            else {
                g_pSysCom->Log("Z:\\Engine\\System\\_Wintel\\Source\\SysFileWintel.cpp", 850)
                    ->LogMessage("Big file \"%s\"", ((BigIO*)file)->Name);
            }

            file = (ZipIO*)this->Archives->GetNext(&link);
        }
    }
}

// 0x0ffa7ed0
HMODULE ZSysFile::LoadModule(const char* path) {
    const char* name = strrchr(path, '\\');
    name = name == nullptr ? path : &name[1]; // \lib.dll -> lib.dll

    RefLink link;
    if (this->Modules != nullptr) {
        this->Modules->GetStart(&link);
        RefKeyValue* kv = this->Modules->GetNext(&link);

        while (kv != nullptr) {
            const char* n = (char*)REFTAB_KEY_TO_PTR(kv);

            if (strcmpi(n, name) == 0) {
                ZSysFileModule* m = (ZSysFileModule*)kv->Value;

                m->Count++;

                return m->Module;
            }

            kv = this->Modules->GetNext(&link);
        }
    }

    HMODULE module = LoadLibraryA(path);
    char* file = new char[strlen(path) + 1];

    strcpy(file, name);

    ZSysFileModule* result =
        (ZSysFileModule*)this->Modules->Insert(REFTAB_PTR_TO_KEY(file));

    result->Module = module;
    result->Count = 1;
    result->Unk0x8 = nullptr;
    result->Unk0xC = nullptr;

    return module;
}

// 0x0ffa7fe0
bool ZSysFile::ReleaseModule(HMODULE module) {
    RefLink link;

    if (this->Modules != nullptr) {
        this->Modules->GetStart(&link);
        RefKeyValue* kv = this->Modules->GetNext(&link);

        while (kv != nullptr) {
            ZSysFileModule* value = (ZSysFileModule*)kv->Value;

            if (module == value->Module) {
                value->Count--;

                if (value->Count != 0) {
                    return false;
                }

                // TODO NOT IMPLEMENTED

                this->Modules->Remove(&link);
                FreeLibrary(module);

                return true;
            }

            kv = this->Modules->GetNext(&link);
        }
    }

    return false;
}

// 0x0ffa8340
void ZSysFile::Method0xAC(/* TODO */) {
    // TODO NOT IMPLEMENTED
}
