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
#include "Midi.hxx"

#define MAX_MIDI_BUFFER_SIZE    256

static MIDIHDR MidiHeader;  // 0x0ff5a618
static HMIDIIN MidiIn;      // 0x0ff5a658

void HandleMidiMessage(DWORD_PTR dwMidiMessage);
void CALLBACK MidiInProc(HMIDIIN hMidiIn, UINT wMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2);

// 0x0ff349f0
void InitializeMidi() {
    char buffer[MAX_MIDI_BUFFER_SIZE];

    if (midiInOpen(&MidiIn, 0, (DWORD_PTR)MidiInProc, (DWORD_PTR)nullptr, CALLBACK_FUNCTION) != MMSYSERR_NOERROR) {

        MidiHeader.dwBufferLength = MAX_MIDI_BUFFER_SIZE;
        MidiHeader.dwFlags = 0;
        MidiHeader.lpData = buffer;

        if (midiInPrepareHeader(MidiIn, &MidiHeader, sizeof(MIDIHDR)) == MMSYSERR_NOERROR) {
            if (midiInAddBuffer(MidiIn, &MidiHeader, sizeof(MIDIHDR)) != MMSYSERR_NOERROR) {
                midiInUnprepareHeader(MidiIn, &MidiHeader, sizeof(MIDIHDR));
                return;
            }

            if (midiInStart(MidiIn) != MMSYSERR_NOERROR) {
                midiInUnprepareHeader(MidiIn, &MidiHeader, sizeof(MIDIHDR));
            }
        }
    }
}

// 0x0ff34ab0
void CALLBACK MidiInProc(HMIDIIN hMidiIn, UINT wMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2) {
    if (wMsg == MIM_DATA) {
        HandleMidiMessage(dwParam1);
    }
}

// 0x0ff34ad0
void HandleMidiMessage(DWORD_PTR dwMidiMessage) {
    ZSoundListener* listener
        = g_pSysInterface->SoundModule->GetSound()->GetListener();

    if (listener != nullptr) {
        void* todo_1 = FUN_0ff34d40(listener->Method0x24());

        if (todo_1 != nullptr) {
            // TODO NOT IMPLEMENTED
            // https://learn.microsoft.com/en-us/windows/win32/multimedia/mim-data?redirectedfrom=MSDN
        }
    }
}
