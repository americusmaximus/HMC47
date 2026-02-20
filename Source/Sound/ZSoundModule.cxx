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

// 0x0ff50a98
ZSoundModule::ZSoundModule() {
    this->Sound = nullptr;
    this->Unk0x18 = new LinkRefTab(8, 0);
    this->Configuration = new SoundConfig();

    this->Configuration->UseStreaming = true;
    this->Configuration->UseEAX = false;
    this->Configuration->UseHW = false;

    this->Configuration->MusicQuality = 0;
    this->Configuration->MusicVolume = 50;
    this->Configuration->NumBuffers = 8;
    this->Configuration->SfxVolume = 50;
    this->Configuration->SpeechVolume = 30;
}

// 0x0ff35130
// 0x0ff35170
ZSoundModule::~ZSoundModule() {
    RefLink link;

    if (this->Unk0x18 != nullptr) {
        this->Unk0x18->GetStart(&link);
        u32 todo = this->Unk0x18->GetNextKey(&link); // TODO

        while (link.Next != nullptr) {
            // TODO NOT IMPLEMENTED

            todo = this->Unk0x18->GetNextKey(&link); // TODO
        }
    }

    if (this->Unk0x18 != nullptr) {
        delete this->Unk0x18;
    }

    delete this->Configuration;
}

// 0x0ff35270
void ZSoundModule::Initialize() {
    if (!g_pSysInterface->Unk0x38F1) {
        ConfigFile* config = g_pSysInterface->EngineData->GetConfigFile();

        if (config->ReadConfigFile()) {
            CopyMemory(this->Configuration, config->GetSoundSettings(), sizeof(SoundConfig));
        }

        if (config != nullptr) {
            delete config;
        }

        g_pSysCom->Log("Z:\\Engine\\Sound\\_Wintel\\Source\\Sound.cpp", 82)->LogMessage("config");

        g_pSysCom->Log("Z:\\Engine\\Sound\\_Wintel\\Source\\Sound.cpp", 83)
            ->LogMessage("HW %d", this->Configuration->UseHW);

        g_pSysCom->Log("Z:\\Engine\\Sound\\_Wintel\\Source\\Sound.cpp", 84)
            ->LogMessage("EAX %d", this->Configuration->UseEAX);

        g_pSysCom->Log("Z:\\Engine\\Sound\\_Wintel\\Source\\Sound.cpp", 85)
            ->LogMessage("buffers %d", this->Configuration->NumBuffers);

        this->Configuration->NumBuffers = max(this->Configuration->NumBuffers, 14);

        this->SetConfiguration(this->Configuration->UseHW,
            this->Configuration->UseEAX, this->Configuration->NumBuffers);
    }
    else {
        this->Sound = new ZSound();
    }
}

// 0x0ff35420
void ZSoundModule::SetConfiguration(bool hw, bool eax, s32 buffers) {
    RefTab tab(8, 0);

    u32 uVar5 = 0; // TODO
    bool apply = false;

    if (this->Sound != nullptr) {
        if (this->Configuration->UseEAX == eax && this->Configuration->UseHW == hw
            && this->Configuration->NumBuffers == buffers) {
            return;
        }

        uVar5 = this->Sound->Method0xF8(&tab);

        this->Sound->Method0x14C();
        this->Sound->Method0x48();

        if (this->Sound != nullptr) {
            delete this->Sound;
        }

        apply = true;
    }

    this->Configuration->UseEAX = eax;
    this->Configuration->UseHW = hw;
    this->Configuration->NumBuffers = buffers;

    this->Sound = new ZSound();

    this->Sound->Method0x148();
    this->Sound->Method0x1A4(0x3000);
    this->Sound->Method0x1A8(true);
    this->Sound->SetUseHW(false);
    this->Sound->SetUseEAX(false);

    if (buffers == 0) {
        buffers = 8;
    }

    this->Sound->SetBuffers(buffers);

    if (hw) {
        this->Sound->Method0x1A8(false);
        this->Sound->SetUseHW(true);

        if (eax) {
            this->Sound->SetUseEAX(true);
        }
    }

    if (apply) {
        this->BeginInit();
        this->Sound->Method0x50(this->Unk0x14);
        this->Sound->SetSfxVolume(this->Configuration->SfxVolume);
        this->Sound->SetSpeechVolume(this->Configuration->SpeechVolume, false);
        this->Sound->Method0x78();
        this->Sound->Method0xFC(&tab);
        this->Sound->SetMusicVolume(this->Configuration->MusicVolume);
        this->Sound->Method0x104(uVar5);
    }
}

// 0x0ff35640
void ZSoundModule::Release() {
    if (this->Sound != nullptr) {
        this->Sound->Method0x14C();

        if (this->Sound != nullptr) {
            delete this->Sound;
        }

        this->Sound = nullptr;
    }
}

// 0x0ff35670
void ZSoundModule::Method0x10(const char* todo) {


    TODO
}

// 0x0ff35740
void ZSoundModule::Method0x14() {
    TODO
}

// 0x0ff357a0
void ZSoundModule::BeginInit() {
    if (this->Sound != nullptr) {
        HWND window = g_pSysInterface->Render == nullptr
            ? g_pSysInterface->Unk0xA85
            : g_pSysInterface->Render->Window;

        if (g_pSysInterface->Unk0x361[0] != NULL) {
            this->Method0x10(g_pSysInterface->Unk0x361.AsString());
        }

        this->Sound->Init(window);
        this->Unk0x10 = this->Sound->Method0x1AC();
    }
}

// 0x0ff35800
void ZSoundModule::EndInit() {
    if (this->Sound != nullptr) {
        this->Sound->EndInit(nullptr);
    }
}

// 0x0ff35810
void ZSoundModule::Method0x20() {
    if (this->Sound != nullptr) {
        this->Sound->Method0xF0();
    }
}

// 0x0ff35820
void ZSoundModule::Method0x28() {
    if (this->Sound != nullptr) {
        this->Sound->Method0xF4();
    }
}

// 0x0ff35830
void ZSoundModule::PrintStatus() {
    if (this->Sound != nullptr) {
        this->Sound->PrintStatus();
    }
}

// 0x0ff35840
bool ZSoundModule::RenderFrame() {
    this->Sound->RenderFrame();
    return true;
}

// 0x0ff35850
s32 ZSoundModule::Method0x40() {
    return this->Unk0x10;
}
