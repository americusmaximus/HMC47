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

#define CONFIGFILE_MAX_FILE_LENGTH  10000

#define INI_VALUE_DEFAULT_LENGTH    12
#define INI_VALUE_MAX_LENGTH        16
#define INI_VALUE_BUFFER_LENGTH     256

#pragma pack(push, 1)

struct IniFileItem {
    bool Set;
    const char* Name;
    const char* Value;
};

#pragma pack(pop)

static const char* Actions[] = {
    "WalkForward", "Run", "WalkLeft", "WalkRight", "WalkBackWard",
    "TurnLeft", "TurnRight", "LeanOutLeft", "LeanOutRight",
    "DoAction", "Sneak", "ToggleMouseControl", "DropItemInHand",
    "Binoculars", "EmptyHands", "FireWeapon", "ReloadWeapon",
    "ToggleWeaponMode", "SelectNext2", "SelectPrev2",
    "ShowMap", "ShowLaptop", "ShowStatus", nullptr
};

// 0x0ff61040
// 0x0ff90150
ConfigFile::ConfigFile()
    : XMLFile(true), Items(8, 39 /* KeyMapping */) {
    this->Unk0xF1 = 0;
}

// 0x0ff611e0
// 0x0ff61220
ConfigFile::~ConfigFile() {}

// 0x0ff610b0
s32 ConfigFile::GetMouseSpeed() {
    return this->Mouse.Speed;
}

// 0x0ff610c0
void ConfigFile::SetMouseSpeed(s32 value) {
    this->Mouse.Speed = value;
}

// 0x0ff610d0
void ConfigFile::SetSoundSfxVolume(s32 value) {
    this->Sound.SfxVolume = max(0, min(100, value));
}

// 0x0ff61100
void ConfigFile::SetSoundMusicVolume(s32 value) {
    this->Sound.MusicVolume = max(0, min(100, value));
}

// 0x0ff61130
void ConfigFile::SetSoundSpeechVolume(s32 value) {
    this->Sound.SpeechVolume = max(0, min(100, value));
}

// 0x0ff61160
void ConfigFile::SetSoundUseEAX(bool value) {
    this->Sound.UseEAX = value;
}

// 0x0ff61170
void ConfigFile::SetSoundUseHW(bool value) {
    this->Sound.UseHW = value;
}

// 0x0ff61180
void ConfigFile::SetSoundMusicQuality(s32 value) {
    this->Sound.MusicQuality = value;
}

// 0x0ff61190
void ConfigFile::SetSoundUseStreaming(bool value) {
    this->Sound.UseStreaming = value;
}

// 0x0ff611a0
void ConfigFile::SetSoundNumBuffers(s32 value) {
    this->Sound.NumBuffers = value;
}

// 0x0ff611b0
SoundConfig* ConfigFile::GetSoundSettings() {
    return &this->Sound;
}

// 0x0ff611c0
bool ConfigFile::GetMouseInvert() {
    return this->Mouse.Invert;
}

// 0x0ff611d0
void ConfigFile::SetMouseInvert(bool value) {
    this->Mouse.Invert = value;
}

// 0x0ff612a0
u32 ConfigFile::GetActionCount() {
    u32 result = 0;

    if (Actions != nullptr) {
        for (u32 i = 0; Actions[i] != nullptr; i++) {
            result++;
        }
    }

    return result;
}

// 0x0ff613b0
bool ConfigFile::ReadConfigFile() {
    this->Method0x98(0);

    this->Keys.DoubleTap = false;
    this->Keys.Codes[0] = 0;
    this->Keys.Codes[1] = 0;
    this->Keys.Codes[2] = 0;
    this->Keys.Codes[3] = 0;
    this->Keys.Count = 0;

    this->Sound.UseEAX = true;
    this->Sound.UseHW = true;
    this->Sound.SfxVolume = 80;
    this->Sound.MusicVolume = 80;
    this->Sound.SpeechVolume = 90;
    this->Sound.UseStreaming = true;
    this->Sound.MusicQuality = 1;
    this->Sound.NumBuffers = 8;

    this->Mouse.Speed = 4;
    this->Mouse.Invert = false;

    this->State = CONFIGFILESTATE_INIT;

    const u32 size = g_pSysFile->GetSize("Hitman.cfg", false);

    if (size != INVALID_FILE_SIZE || size == 0) {
        return false;
    }

    char* value = new char[size];

    g_pSysFile->ReadAt("Hitman.cfg", value, size, 0, false);

    const bool ok = this->Parse(value, size, 1) != XML_STATUS_ERROR;

    delete[] value;

    if (!ok) {
        g_pSysCom->Log("Z:\\Engine\\EngineData\\Source\\ConfigFile.cpp", 210)
            ->LogMessage("Error in config file %s line %d (code %d)",
                "Hitman.cfg", this->GetFileLineNumber(), this->GetErrorCode());
    }

    this->Mouse.Speed = max(1, min(70, this->Mouse.Speed));
    this->Sound.SfxVolume = max(0, min(100, this->Sound.SfxVolume));
    this->Sound.MusicVolume = max(0, min(100, this->Sound.MusicVolume));
    this->Sound.SpeechVolume = max(0, min(100, this->Sound.SpeechVolume));

    return ok;
}

// 0x0ff615a0
bool ConfigFile::WriteConfigFile() {
    if (g_pSysInterface->DisableOptions) {
        return true;
    }

    char* value = new char[CONFIGFILE_MAX_FILE_LENGTH];
    value[0] = NULL;

    char* current = value + sprintf(value, "<%s", "Settings");
    current = current + sprintf(current, ">\n");

    current = current + sprintf(current, "<%s", "Controls");
    current = current + sprintf(current, ">\n");

    {
        RefLink link;
        this->Items.GetStart(&link);

        KeyMapping* item = (KeyMapping*)this->Items.GetNext(&link);

        while (item != nullptr) {
            current = current + sprintf(current, "<%s", "Mapping");
            current = current + sprintf(current, " %s=\"%s\"", "Action", item->Action.AsString());

            if (item->DoubleTap) {
                current = current + sprintf(current, " %s=\"%d\"", "DoubleTap", 1);
            }

            current = current + sprintf(current, ">\n");

            for (u32 i = 0; i < CONFIGFILE_MAX_KEY_COUNT; i++) {
                current = current + sprintf(current, "<%s", "Key");
                current = current + sprintf(current, " %s=\"%d\"", "SCode", item->Codes[i]);
                current = current + sprintf(current, "/>\n");
            }

            current = current + sprintf(current, "</%s>\n", "Mapping");

            item = (KeyMapping*)this->Items.GetNext(&link);
        }
    }

    current = current + sprintf(current, "<%s", "Mouse");
    current = current + sprintf(current, " %s=\"%d\"", "Speed", this->Mouse.Speed);
    current = current + sprintf(current, " %s=\"%d\"", "Invert", this->Mouse.Invert);
    current = current + sprintf(current, "/>\n");

    current = current + sprintf(current, "</%s>\n", "Controls");

    current = current + sprintf(current, "<%s", "Sound");
    current = current + sprintf(current, " %s=\"%d\"", "SfxVol", this->Sound.SfxVolume);
    current = current + sprintf(current, " %s=\"%d\"", "MusicVol", this->Sound.MusicVolume);
    current = current + sprintf(current, " %s=\"%d\"", "SpeechVol", this->Sound.SpeechVolume);
    current = current + sprintf(current, " %s=\"%d\"", "UseEAX", this->Sound.UseEAX != false);
    current = current + sprintf(current, " %s=\"%d\"", "UseHW", this->Sound.UseHW != false);
    current = current + sprintf(current, " %s=\"%d\"", "UseStreaming", this->Sound.UseStreaming != false);
    current = current + sprintf(current, " %s=\"%d\"", "MusicQuality", this->Sound.MusicQuality);
    current = current + sprintf(current, " %s=\"%d\"", "NumBuffers", this->Sound.NumBuffers);
    current = current + sprintf(current, "/>\n");

    current = current + sprintf(current, "</%s>\n", "Settings");

    HANDLE file = g_pSysFile->Create("Hitman.cfg");

    if (file != NULL) {
        g_pSysFile->WriteTo(file, value, current - value);
        g_pSysFile->Close(file);

        delete[] value;

        return true;
    }

    g_pSysCom->Log("Z:\\Engine\\EngineData\\Source\\ConfigFile.cpp", 291)
        ->LogMessage("ERROR: Unable to write to file %s", "Hitman.cfg");

    delete[] value;

    return false;
}

// 0x0ff619a0
bool ConfigFile::UpdateIniFile() {
    if (g_pSysInterface->DisableOptions) {
        return true;
    }

    const char* cmd = g_pSysInterface->CommandLine.AsString();

    if (cmd[0] == '@') {
        cmd = &cmd[1];

        if (g_pSysFile->Exists(cmd, false)) {
            // Open existing file.
            const u32 size = g_pSysFile->GetSize(cmd, false);
            char* content = new char[size + 1];

            g_pSysFile->ReadAt(cmd, content, size, 0, false);
            content[size] = NULL;

            // Create new (replacement) file.
            HANDLE file = g_pSysFile->Create(cmd);

            // Collect and prepare settings.
            ZRender* render = g_pSysInterface->Render;

            u32 caps = 0;
            render->GetCaps(&caps);
            render->Method0x1C(3, 0);

            char resolution[INI_VALUE_MAX_LENGTH];
            sprintf(resolution, "%dx%d",
                g_pSysInterface->WindowWidth, g_pSysInterface->WindowHeight);

            char depth[INI_VALUE_DEFAULT_LENGTH];
            sprintf(depth, "%d", g_pSysInterface->ColorDepth);

            char shadows[INI_VALUE_DEFAULT_LENGTH];
            sprintf(shadows, "%d", g_pSysInterface->ShadowDetail);

            char textures[INI_VALUE_DEFAULT_LENGTH];
            sprintf(textures, "%d", g_pSysInterface->TextureResolution);

            char lod[INI_VALUE_DEFAULT_LENGTH];
            sprintf(lod, "%d", g_pSysInterface->LevelOfDetail);

            IniFileItem items[] = {
                { false, "Anisotropy", g_pSysInterface->Anisotropy ? "2" : "0" },
                { !g_pSysInterface->EnableLight, "AntiAlias", nullptr },
                { (caps & 4) && g_pSysInterface->EnableLight, "BumpMap", nullptr },
                { g_pSysInterface->DisableDXT, "DisableDXT", nullptr },
                { g_pSysInterface->DisablePAL, "DisablePAL", nullptr },
                { g_pSysInterface->EnableLight, "DisableLight", nullptr },
                { this->Unk0xF1 == 0
                ? g_pSysInterface->FullScreen : (this->Unk0xF1 != 1), "Window", nullptr },
                { false, "Resolution", resolution },
                { false, "DrawDll", g_pSysInterface->RenderModule->ModuleName },
                { false, "ColorDepth", depth },
                { false, "ShadowDetail", shadows },
                { false, "TextureResolution", textures },
                { false, nullptr, nullptr }
            };

            const char* current = content;
            char buffer[INI_VALUE_BUFFER_LENGTH];

            do {
                // Go through the each line of content in the file.
                const char* start = current;
                const char* end = strchr(current, '\n');

                if (end != nullptr) {
                    current = strchr(end, '\r') == nullptr
                        ? current + strlen(start) : current + 1;
                }

                // Then go through each unset setting and,
                // for matching setting name - write new values into the file.
                if (start != nullptr && start[0] != NULL) {
                    bool found = false;

                    for (u32 i = 0; items[i].Name != nullptr; i++) {
                        const size_t length = strlen(items[i].Name);

                        if (strnicmp(start, items[i].Name, length) == 0) {
                            found = true;

                            // If the setting is not set (written) - write it,
                            // otherwise skip it alltogether, thus it won't be in the new file.
                            if (!items[i].Set) {
                                const u32 bytes = items[i].Value == nullptr
                                    ? sprintf(buffer, "%s\r\n", items[i].Name)
                                    : sprintf(buffer, "%s %s\r\n", items[i].Name, items[i].Value);

                                g_pSysFile->WriteTo(file, buffer, bytes);
                            }

                            items[i].Set = true;
                        }
                    }

                    // If there's no match - write the original content.
                    if (!found) {
                        g_pSysFile->WriteTo(file, start, current - start);
                    }
                }
                else {
                    // The end of the original file is reached.
                    // Write all the settings that have'nt been updated to the end of the file.
                    for (u32 i = 0; items[i].Name != nullptr; i++) {
                        if (!items[i].Set) {
                            const u32 bytes = items[i].Value == nullptr
                                ? sprintf(buffer, "%s\r\n", items[i].Name)
                                : sprintf(buffer, "%s %s\r\n", items[i].Name, items[i].Value);

                            g_pSysFile->WriteTo(file, buffer, bytes);
                        }
                    }
                }
            } while (current != nullptr && current[0] != NULL);

            delete[] content;

            g_pSysFile->Close(file);

            return true;
        }
    }

    return false;
}

// 0x0ff61f00
bool ConfigFile::SaveConfiguration() {
    const bool cfg = this->WriteConfigFile();
    const bool ini = this->UpdateIniFile();

    return cfg && ini;
}

// 0x0ff61f30
void ConfigFile::HandleStartElement(const char* name, const char** atts) {
    switch (this->State) {
    case CONFIGFILESTATE_INIT: {
        if (strcmp(name, "Controls") == 0) {
            this->State = CONFIGFILESTATE_CONTROLS;

            return;
        }

        if (strcmp(name, "Sound") == 0) {
            const char* attr = this->GetAttributeValue(atts, "SfxVol");

            if (attr != nullptr) {
                const s32 value = atoi(attr);
                this->Sound.SfxVolume = min(max(0, value), 100);
            }

            attr = this->GetAttributeValue(atts, "MusicVol");

            if (attr != nullptr) {
                const s32 value = atoi(attr);
                this->Sound.MusicVolume = min(max(0, value), 100);
            }

            attr = this->GetAttributeValue(atts, "SpeechVol");

            if (attr != nullptr) {
                const s32 value = atoi(attr);
                this->Sound.SpeechVolume = min(max(0, value), 100);
            }

            attr = this->GetAttributeValue(atts, "UseEAX");

            if (attr != nullptr) {
                this->Sound.UseEAX = atoi(attr) != 0;
            }

            attr = this->GetAttributeValue(atts, "MusicQuality");

            if (attr != nullptr) {
                const s32 value = atoi(attr);
                this->Sound.MusicQuality = min(max(0, value), 2);
            }

            attr = this->GetAttributeValue(atts, "NumBuffers");

            if (attr != nullptr) {
                const s32 value = atoi(attr);
                this->Sound.NumBuffers = min(max(3, value), 16);
            }

            attr = this->GetAttributeValue(atts, "UseHW");

            if (attr != nullptr) {
                this->Sound.UseHW = atoi(attr) != 0;
            }

            attr = this->GetAttributeValue(atts, "UseStreaming");

            if (attr != nullptr) {
                this->Sound.UseStreaming = atoi(attr) != 0;
            }

            this->State = CONFIGFILESTATE_SOUND;

            return;
        }

        if (strcmp(name, "Display") == 0) {
            this->State = CONFIGFILESTATE_DISPLAY;

            return;
        }

        if (strcmp(name, "Settings") == 0) {
            return;
        }

        g_pSysCom->Log("Z:\\Engine\\EngineData\\Source\\ConfigFile.cpp", 631)
            ->LogMessage("Error in config file %s, line %d : %s", "Hitman.cfg",
                this->GetFileLineNumber(), "Unknown start tag");

        return;
    }
    case CONFIGFILESTATE_CONTROLS: {
        if (strcmp(name, "Mapping") == 0) {
            ZString action = this->GetAttributeValue(atts, "Action");

            const char* attr = this->GetAttributeValue(atts, "DoubleTap");

            if (attr != nullptr) {
                this->Keys.DoubleTap = atoi(attr) != 0;
            }

            this->State = CONFIGFILESTATE_CONTROLS_KEY;

            return;
        }

        if (strcmp(name, "Mouse") == 0) {
            const char* attr = this->GetAttributeValue(atts, "Speed");

            if (attr != nullptr) {
                this->Mouse.Speed = atoi(attr);
            }

            attr = this->GetAttributeValue(atts, "Invert");

            if (attr != nullptr) {
                this->Mouse.Invert = atoi(attr) != 0;
            }

            this->State = CONFIGFILESTATE_CONTROLS_MOUSE;

            return;
        }

        g_pSysCom->Log("Z:\\Engine\\EngineData\\Source\\ConfigFile.cpp", 631)
            ->LogMessage("Error in config file %s, line %d : %s", "Hitman.cfg",
                this->GetFileLineNumber(), "Invalid start tag inside Controls section");

        return;
    }
    case CONFIGFILESTATE_CONTROLS_KEY: {
        if (strcmp(name, "Key") == 0) {
            const char* attr = this->GetAttributeValue(atts, "SCode");

            if (attr != nullptr) {
                if (this->Keys.Count < CONFIGFILE_MAX_KEY_COUNT) {
                    this->Keys.Codes[this->Keys.Count] = atoi(attr);
                    this->Keys.Count++;

                    this->State = CONFIGFILESTATE_CONTROLS_KEYS;

                    return;
                }

                g_pSysCom->Log("Z:\\Engine\\EngineData\\Source\\ConfigFile.cpp", 535)
                    ->LogMessage("ERROR: Too many keys defined for action %s", this->Keys.Action.AsString());
            }

            this->State = CONFIGFILESTATE_CONTROLS_KEYS;

            return;
        }

        g_pSysCom->Log("Z:\\Engine\\EngineData\\Source\\ConfigFile.cpp", 631)
            ->LogMessage("Error in config file %s, line %d : %s", "Hitman.cfg",
                this->GetFileLineNumber(), "Invalid start tag inside Controls section");

        return;
    }
    case CONFIGFILESTATE_CONTROLS_KEYS: {
        g_pSysCom->Log("Z:\\Engine\\EngineData\\Source\\ConfigFile.cpp", 631)
            ->LogMessage("Error in config file %s, line %d : %s", "Hitman.cfg",
                this->GetFileLineNumber(), "Invalid start tag inside Key section");

        return;
    }
    case CONFIGFILESTATE_SOUND:
    case CONFIGFILESTATE_DISPLAY: {
        g_pSysCom->Log("Z:\\Engine\\EngineData\\Source\\ConfigFile.cpp", 631)
            ->LogMessage("Error in config file %s, line %d : %s", "Hitman.cfg",
                this->GetFileLineNumber(), "Unexpected start tag inside Sound section");

        return;
    }
    }
}

// 0x0ff62690
void ConfigFile::HandleEndElement(const char* name) {
    switch (this->State) {
    case CONFIGFILESTATE_INIT: {
        if (strcmp(name, "Settings") == 0) {
            return;
        }

        g_pSysCom->Log("Z:\\Engine\\EngineData\\Source\\ConfigFile.cpp", 631)
            ->LogMessage("Error in config file %s, line %d : %s", "Hitman.cfg",
                this->GetFileLineNumber(), "Unexpected </> token outside all sections");

        return;
    }
    case CONFIGFILESTATE_CONTROLS: {
        if (strcmp(name, "Controls") == 0) {
            this->State = CONFIGFILESTATE_INIT;

            return;
        }

        g_pSysCom->Log("Z:\\Engine\\EngineData\\Source\\ConfigFile.cpp", 631)
            ->LogMessage("Error in config file %s, line %d : %s", "Hitman.cfg",
                this->GetFileLineNumber(), "Misplaced end token");

        return;
    }
    case CONFIGFILESTATE_CONTROLS_KEY: {
        if (strcmp(name, "Mapping") == 0) {
            if (strlen(this->Keys.Action.AsString()) != 0) {
                if (this->Keys.Codes[0] != 0) {
                    this->AddMapping(this->Keys.Action.AsString(), this->Keys.DoubleTap,
                        this->Keys.Codes[0], this->Keys.Codes[1], this->Keys.Codes[2], this->Keys.Codes[3]);
                }
            }

            this->Keys.DoubleTap = false;

            this->Keys.Codes[0] = 0;
            this->Keys.Codes[1] = 0;
            this->Keys.Codes[2] = 0;
            this->Keys.Codes[3] = 0;

            this->Keys.Count = 0;

            this->State = CONFIGFILESTATE_CONTROLS;

            return;
        }

        g_pSysCom->Log("Z:\\Engine\\EngineData\\Source\\ConfigFile.cpp", 631)
            ->LogMessage("Error in config file %s, line %d : %s", "Hitman.cfg",
                this->GetFileLineNumber(), "Misplaced end token");

        return;
    }
    case CONFIGFILESTATE_CONTROLS_MOUSE: {
        if (strcmp(name, "Mouse") == 0) {
            this->State = CONFIGFILESTATE_CONTROLS;

            return;
        }

        g_pSysCom->Log("Z:\\Engine\\EngineData\\Source\\ConfigFile.cpp", 631)
            ->LogMessage("Error in config file %s, line %d : %s", "Hitman.cfg",
                this->GetFileLineNumber(), "Misplaced end token");

        return;
    }
    case CONFIGFILESTATE_CONTROLS_KEYS: {
        if (strcmp(name, "Key") == 0) {
            this->State = CONFIGFILESTATE_CONTROLS_KEY;

            return;
        }

        g_pSysCom->Log("Z:\\Engine\\EngineData\\Source\\ConfigFile.cpp", 631)
            ->LogMessage("Error in config file %s, line %d : %s", "Hitman.cfg",
                this->GetFileLineNumber(), "Misplaced end token");

        return;
    }
    case CONFIGFILESTATE_SOUND:
    case CONFIGFILESTATE_DISPLAY: {
        this->State = CONFIGFILESTATE_INIT;

        return;
    }
    }
}

// 0x0ff62910
u32 ConfigFile::AddMapping(const char* action, bool dbl, s32 code1, s32 code2, s32 code3, s32 code4) {
    u32 index = 0;

    if (code1 < 0) {
        g_pSysCom->Log("Z:\\Engine\\EngineData\\Source\\ConfigFile.cpp", 640)
            ->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\EngineData\\Source\\ConfigFile.cpp", 640);

        __asm { int 3 }
    }

    if (code2 < 0) {
        g_pSysCom->Log("Z:\\Engine\\EngineData\\Source\\ConfigFile.cpp", 641)
            ->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\EngineData\\Source\\ConfigFile.cpp", 641);

        __asm { int 3 }
    }

    if (code3 < 0) {
        g_pSysCom->Log("Z:\\Engine\\EngineData\\Source\\ConfigFile.cpp", 642)
            ->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\EngineData\\Source\\ConfigFile.cpp", 642);

        __asm { int 3 }
    }

    if (code4 < 0) {
        g_pSysCom->Log("Z:\\Engine\\EngineData\\Source\\ConfigFile.cpp", 643)
            ->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\EngineData\\Source\\ConfigFile.cpp", 643);

        __asm { int 3 }
    }

    RefLink link;
    this->Items.GetStart(&link);

    KeyMapping* item = (KeyMapping*)this->Items.GetNext(&link);

    bool found = false;
    while (item != nullptr) {
        if (strcmp(item->Action, action) == 0) {
            index++;

            if (code1 == item->Codes[0] && code2 == item->Codes[1]
                && code3 == item->Codes[2] && code4 == item->Codes[3]) {
                found = true;
            }
        }

        item = (KeyMapping*)this->Items.GetNext(&link);
    }

    if (found) {
        return index;
    }

    KeyMapping* item = (KeyMapping*)this->Items.Insert(0);

    item->Action = ZString(action);

    item->DoubleTap = dbl;

    item->Codes[0] = code1;
    item->Codes[1] = code2;
    item->Codes[2] = code3;
    item->Codes[3] = code4;

    return index + 1;
}

// 0x0ff62d20
void ConfigFile::RemoveMapping(KeyMapping* value) {
    RefLink link;
    this->Items.GetStart(&link);

    KeyMapping* item = (KeyMapping*)this->Items.GetNext(&link);

    while (item != nullptr) {
        if (item->Codes[0] == value->Codes[0]
            && item->Codes[1] == value->Codes[1]
            && item->Codes[2] == value->Codes[2]
            && item->Codes[3] == value->Codes[4] && item->DoubleTap == value->DoubleTap
            && strcmpi(item->Action.AsString(), value->Action.AsString()) == 0) {
            this->Items.Remove(&link);
            return;
        }

        item = (KeyMapping*)this->Items.GetNext(&link);
    }

    g_pSysCom->Log("Z:\\Engine\\EngineData\\Source\\ConfigFile.cpp", 703)
        ->LogMessage("WARNING unable to remove mapping for %s (keys %d and %d%s)",
            value->Action.AsString(), value->Codes[0], value->Codes[1], value->DoubleTap ? " DBL" : "");
}

// 0x0ff632b0
void ConfigFile::ApplyMouseSettings() {
    g_pSysInterface->MouseSpeed = this->Mouse.Speed * 0.1;
    g_pSysInterface->MouseInvert = this->Mouse.Invert;

    // TODO NOT IMPLEMENTED
}

// 0x0ff63340
void ConfigFile::ApplySoundSettings() {
    if (g_pSysInterface->SoundModule != nullptr) {
        ZSound* sound = g_pSysInterface->SoundModule->GetSound();

        if (sound != nullptr) {
            sound->SetSpeechVolume((f32)this->Sound.SpeechVolume, false);
            sound->SetMusicVolume((f32)this->Sound.MusicVolume);
            sound->SetSfxVolume((f32)this->Sound.SfxVolume);
            sound->SetStreaming(this->Sound.UseStreaming);
            sound->SetMusicQuality(this->Sound.MusicQuality);

            g_pSysInterface->SoundModule->SetConfiguration(this->Sound.UseHW,
                this->Sound.UseEAX, this->Sound.NumBuffers);
        }
    }
}

// 0x0ff63420
void ConfigFile::ApplyAllSettings() {
    this->ApplyKeySettings();
    this->ApplyMouseSettings();
    this->ApplySoundSettings();
    this->Method0x94();
}

// 0x0ff63450
const char** ConfigFile::GetActions() {
    return Actions;
}

// 0x0ff84730
void ConfigFile::HandleCharacterData(const char*, s32) {}

// 0x0ff84730
void ConfigFile::HandleProcessingInstruction(const char*, const char*) {}

// 0x0ff84730
void ConfigFile::HandleSetDefault(const char*, s32) {}

// 0x0ff84740
s32 ConfigFile::HandleNotStandalone() {
    return 0;
}

// 0x0ff84750
void ConfigFile::HandleUnparsedEntityDecl(const char*, const char*, const char*, const char*, const char*) {}

// 0x0ff84760
void ConfigFile::HandleNotationDecl(const char*, const char*, const char*, const char*) {}

// 0x0ff84730
void ConfigFile::HandleStartNamespaceDecl(const char*, const char*) {}

// 0x0ff8e220
void ConfigFile::HandleEndNamespaceDecl(const char*) {}

// 0x0ff8e230
void ConfigFile::Method0x94() {}
