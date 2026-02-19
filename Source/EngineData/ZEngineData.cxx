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

// 0x0ff64660
void* ZEngineData::Method0x2C() {
    return this->Unk0x165;
}

// 0x0ff64670
void* ZEngineData::Method0x30() {
    return this->Unk0x169;
}

// 0x0ff67480
void* ZEngineData::Method0x128() {
    return this->Unk0x171;
}

// 0x0ff69e90
void ZEngineData::Method0xB0() {
    this->Method0xB4();
    this->Unk0x16D = new RefTab(32, 2);
}

// 0x0ff6e980
void ZEngineData::Method0x68() {
    if (g_pSysInterface->SoundModule != nullptr
        && g_pSysInterface->SoundModule->GetSound() != nullptr) {
        void* todo = g_pSysInterface->EngineData->Method0x128();

        if (todo != nullptr) {
            // TODO NOT IMPLEMENTED
        }
    }
}

// 0x0ff6ef40
bool ZEngineData::Method0x44(s32 param_2, const char* function, const void* data) {
    if (g_pSysInterface->Unk0x38F1) {
        if (param_2 != 0) { // TODO
            return false;
        }

        if (strcmpi(function, "Hira") != 0 && strcmpi(function, "Dir") != 0) {
            if (strcmpi(function, "HiraClip") == 0 || strcmpi(function, "DirClip") == 0) {
                if (this->Unk0x10E != nullptr) {
                    this->Unk0x10E->Method0x364(0); // TODO

                    return true;
                }
            }

            g_pSysCom->Log("Z:\\Engine\\EngineData\\Source\\EngineData.cpp", 3263)
                ->LogMessage("Unknown pFunction :%s, pData:%d\n", function, data);

            return true;
        }
    }
    else {
        switch (param_2) {
        case 0: { // TODO
            if (strcmpi(function, "Cams") == 0) {
                LinkSortRefTab* cams = g_pSysInterface->Render->Method0x6C();

                if (cams == nullptr) {
                    return true;
                }

                RefLink link;
                cams->GetStart(&link);

                // TODO NOT IMPLEMENTED

                return true;
            }

            if (strnicmp(function, "KillScript", 10) == 0) {
                if (data != nullptr) {
                    const s32 value = atoi((const char*)data);

                    if (value != 0) {
                        g_pSysCom->Log("Z:\\Engine\\EngineData\\Source\\EngineData.cpp", 3312)
                            ->LogMessage("KillScript cmd not implemented yet -- smack Obi");

                        return true;
                    }

                    g_pSysCom->Log("Z:\\Engine\\EngineData\\Source\\EngineData.cpp", 3314)
                        ->LogMessage("Usage: KillScript int");
                }
                else {
                    g_pSysCom->Log("Z:\\Engine\\EngineData\\Source\\EngineData.cpp", 3317)
                        ->LogMessage("Usage: KillScript int");
                }

                return true;
            }

            if (strnicmp(function, "ReloadScripts", 10) == 0) {
                if (g_pSysInterface->ScriptModule != nullptr) {
                    g_pSysCom->Log("Z:\\Engine\\EngineData\\Source\\EngineData.cpp", 3323)
                        ->LogMessage("Reloading devel scripts...");

                    g_pSysInterface->ScriptModule->Method0x94();

                    return true;
                }

                g_pSysCom->Log("Z:\\Engine\\EngineData\\Source\\EngineData.cpp", 3327)
                    ->LogMessage("Script Engine not running");

                return true;
            }

            if (strnicmp(function, "ScriptStatus", 10) == 0) {
                if (g_pSysInterface->ScriptModule != nullptr) {
                    g_pSysInterface->ScriptModule->Method0xB0();

                    // TODO NOT IMPLEMENTED
                }

                g_pSysCom->Log("Z:\\Engine\\EngineData\\Source\\EngineData.cpp", 3342)
                    ->LogMessage("Script Engine not running");

                return true;
            }

            if (strcmpi(function, "Globals") == 0) {
                this->Unk0x165->Method0x8();
                return true;
            }

            if (strcmpi(function, "ZDefines") == 0) {
                this->Unk0x169->Method0x8();
                return true;
            }

            if (strcmpi(function, "Msg") == 0) {
                this->Method0x100();
                return true;
            }

            if (strcmpi(function, "Hira") != 0 && strcmpi(function, "Dir") != 0) {
                if (strcmpi(function, "HiraClip") == 0 || strcmpi(function, "DirClip") == 0) {
                    if (this->Unk0x10E != nullptr) {
                        this->Unk0x10E->Method0x364(0); // TODO
                        return true;
                    }

                    g_pSysCom->Log("Z:\\Engine\\EngineData\\Source\\EngineData.cpp", 3366)
                        ->LogMessage("Clip root empty\n");
                }
                else {
                    if (strcmpi(function, "Dump") == 0) {
                        g_pSysInterface->Render->Method0x64(data);
                        return true;
                    }

                    if (strcmpi(function, "Reload") == 0) {
                        // TODO NOT IMPLEMENTED

                        return true;
                    }

                    if (strcmpi(function, "GiveAll") == 0) {
                        this->GiveAll = true;
                        return true;
                    }

                    if (strcmpi(function, "InfAmmo") == 0) {
                        this->InfAmmo = true;

                        g_pSysCom->Log("Z:\\Engine\\EngineData\\Source\\EngineData.cpp", 3386)
                            ->LogMessage("Infinte ammo added");

                        return true;
                    }

                    if (strcmpi(function, "XpressQuit") == 0) {
                        exit(EXIT_SUCCESS);
                    }

                    if (strcmpi(function, "disableCUI") == 0) {
                        void* todo_1 = g_pSysInterface->EngineData->Method0x30(); // TODO

                        // TODO NOT IMPLEMENTED

                        if (this->Unk0x10A != nullptr) {
                            g_pSysInterface->EngineData->Method0xF8("disableCUI", todo);

                            this->Unk0x10A->Method0x26C();

                            return true;
                        }

                        g_pSysCom->Log("Z:\\Engine\\EngineData\\Source\\EngineData.cpp", 3397)
                            ->LogMessage("Cannot disable CUI - no root");

                        return true;
                    }

                    if (strcmpi(function, "enableCUI") == 0) {
                        void* todo_1 = g_pSysInterface->EngineData->Method0x30(); // TODO

                        // TODO NOT IMPLEMENTED

                        if (this->Unk0x10A != nullptr) {
                            g_pSysInterface->EngineData->Method0xF8("CUI", todo);

                            this->Unk0x10A->Method0x26C();

                            return true;
                        }

                        g_pSysCom->Log("Z:\\Engine\\EngineData\\Source\\EngineData.cpp", 3397)
                            ->LogMessage("Cannot disable CUI - no root");

                        return true;
                    }

                    g_pSysCom->Log("Z:\\Engine\\EngineData\\Source\\EngineData.cpp", 3407)
                        ->LogMessage("Unknown pFunction :%s, pData:%d\n", function, data);
                }

                return true;
            }

            break;
        }
        case 1:   // TODO
        case 2:   // TODO
        case 3: { // TODO
            return true;
        }
        default: {
            return false;
        }
        }
    }

    this->Unk0x106->Method0x364(0); // TODO

    return true;
}

// 0x0ff753f0
ConfigFile* ZEngineData::GetConfigFile() {
    return new ConfigFile();
}
