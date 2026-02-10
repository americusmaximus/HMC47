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
    this->Configuration = new ZSoundModuleConfiguration();

    this->Configuration->Unk0x16 = true;
    this->Configuration->EAX = 0;
    this->Configuration->HW = 0;

    this->Configuration->Unk0xC = 0;
    this->Configuration->Unk0x4 = 50;
    this->Configuration->Buffers = 8;
    this->Configuration->Unk0x0 = 50;
    this->Configuration->Unk0x8 = 30;
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
        void* todo_1 = g_pSysInterface->EngineData->Method0x10(); // TODO

        if (todo_1->Method0x78()) {
            CopyMemory(this->Configuration, todo_1->Method0x6C(), sizeof(ZSoundConfiguration));
        }

        if (todo_1 != nullptr) {
            delete todo_1;
        }

        g_pSysCom->Log("Z:\\Engine\\Sound\\_Wintel\\Source\\Sound.cpp", 82)->LogMessage("config");

        g_pSysCom->Log("Z:\\Engine\\Sound\\_Wintel\\Source\\Sound.cpp", 83)
            ->LogMessage("HW %d", this->Configuration->HW);

        g_pSysCom->Log("Z:\\Engine\\Sound\\_Wintel\\Source\\Sound.cpp", 84)
            ->LogMessage("EAX %d", this->Configuration->EAX);

        g_pSysCom->Log("Z:\\Engine\\Sound\\_Wintel\\Source\\Sound.cpp", 85)
            ->LogMessage("buffers %d", this->Configuration->Buffers);

        this->Configuration->Buffers = max(this->Configuration->Buffers, 14);

        this->SetConfiguration(this->Configuration->HW,
            this->Configuration->EAX, this->Configuration->Buffers);
    }
    else {
        this->Sound = new ZSound();
    }
}

// 0x0ff35420
void ZSoundModule::SetConfiguration(u8 hw, u8 eax, u32 buffers) {
    RefTab tab(8, 0);

    u32 uVar5 = 0; // TODO
    bool bVar2 = false; // TODO

    // TODO NOT IMPLEMENTED
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
void ZSoundModule::Method0x34() {
    if (this->Sound != nullptr) {
        this->Sound->Method0x128();
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
