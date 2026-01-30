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

// 0x0ffa8da0
ZSysInterfaceBase::ZSysInterfaceBase() {
    this->EnableTextures = true;
    this->EnableLight = true;
    this->Unk0xF = false;
    this->EnableZBuffer = false;
    this->DisableMultiTexture = false;
    this->TrilinearFiltering = false;
    this->DisableHWTnL = false;
    this->EnableEMBM = false;
    this->EnableTripleBuffering = false;
    this->Unk0x6 = false;
    this->Unk0x7 = false;
    this->TimeMultiplierVisibility = 1.0f;
    this->WindowHasFocus = FALSE;
    this->Unk0x3A1A = 0; // TODO
    this->Unk0x3A16 = nullptr;
    this->Unk0x3A1E = 0; // TODO

    ZeroMemory(this->Unk0x3996, 128 /* TODO */);

    this->Unk0x3A27 = 0; // TODO
    this->Unk0x3A2B = 0; // TODO
    this->Unk0x37DD = 0; // TODO
    this->ProcessorCounter = 0.0f;

    this->Unk0x3A2F = 0; // TODO
    this->SaveRecordRuntime = false;
    this->EnablePack = true;
    this->DisableConfig = false;
    this->Unk0x4 = true;
    this->Unk0x5 = true;
    this->Unk0x3A22 = 0; // TODO
    this->Unk0x3A33 = nullptr;
    this->Unk0x3995 = false;
    this->Unk0x38ED = false;
    this->FullScreen = false;
    this->StereoView = false;
    this->Antialias = false;
    this->Anisotropy = ANISOTROPY_0;
    this->DisableDXT = false;
    this->DisablePAL = false;
    this->ColorDepth = 0; // TODO
    this->RefreshRate = 0; // TODO
    this->TextureResolution = GRAPHICSTEXTUREQUALITY_VERY_LOW;
    this->LevelOfDetail = GRAPHICSLEVELOFDETAIL_1;
    this->ShadowDetail = GRAPHICSSHADOWQUALITY_ALL;
    this->DisableOptions = false;
    this->Unk0x37E1 = 1.875f;
    this->Unk0x38F3 = false;
    this->DebugVideo = false;
    this->Blood = true;
    this->PathfinderInfo = false;
    this->EnableScriptRTC = false;
    this->Console = nullptr;
    this->Unk0xA65 = 0; // TODO
    this->Unk0xA69 = 0; // TODO
    this->FreeVideoMemory = 0;

    this->ProjectFile = "Intro.zip";
}

// 0x0ffa9290
// 0x0ffa92d0
ZSysInterfaceBase::~ZSysInterfaceBase() {
    if (strlen(this->Unk0x7E1) != 0) {
        g_pSysFile->Method0x58(this->Unk0x7E1, this->Unk0x3A16,
            (void*)((size_t)this->Unk0x3A1A - (size_t)this->Unk0x3A16), 0); // TODO
    }

    if (this->Unk0x3A16 != nullptr) {
        delete this->Unk0x3A16;
    }

    this->Unk0x3A16 = nullptr;

    this->Unk0x3A1A = 0; // TODO
    this->Unk0x3A1E = 0; // TODO

    ZeroMemory(&this->Unk0x3996, 128 /* TODO */);

    if (this->Unk0x3A33 != nullptr) {
        delete this->Unk0x3A33;
    }

    if (this->Console != nullptr) {
        delete this->Console;
    }
}

// 0x0ffa9890
bool ZSysInterfaceBase::Method0x0() {
    if (!this->Unk0x3995) {
        if (strlen(this->Unk0x861) != 0) {
            return true;
        }
    }

    return false;
}

// 0x0ffaa720
void ZSysInterfaceBase::RegisterConsoleCommand(ZConsoleCommand* command) {
    ZConsole* console = this->GetConsole();

    if (console != nullptr) {
        console->RegisterCommand(command);
    }
}

// 0x0ffaa750
void ZSysInterfaceBase::UnregisterConsoleCommand(ZConsoleCommand* command) {
    ZConsole* console = this->GetConsole();

    if (console != nullptr) {
        console->UnregisterCommand(command);
    }
}

// 0x0ffaa780
void ZSysInterfaceBase::RunConsoleCommand(const char* command) {
    ZConsole* console = this->GetConsole();

    if (console != nullptr) {
        console->RunCommand(command);
    }
}
