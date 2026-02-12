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

// 0x0ff753f0
ConfigFile* ZEngineData::GetConfigFile() {
    return new ConfigFile();
}
