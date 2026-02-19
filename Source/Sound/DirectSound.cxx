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

#include "DirectSound.hxx"

#define DIRECTSOUND_MAX_LOGMESSAGE_LENGTH   512

const char* SoundEnvironments[] = {
    "GENERIC", "PADDEDCELL", "ROOM", "BATHROOM",
    "LIVINGROOM", "STONEROOM", "AUDITORIUM", "CONCERTHALL",
    "CAVE", "ARENA", "HANGAR", "CARPETEDHALLWAY", 
    "HALLWAY", "STONECORRIDOR", "ALLEY", "FOREST",
    "CITY", "MOUNTAINS", "QUARRY", "PLAIN",
    "PARKINGLOT", "SEWERPIPE", "UNDERWATER", "DRUGGED",
    "DIZZY", "PSYCHOTIC"
}; // 0x0ff55cbc

// 0x0ff34d70
void DirectSoundLogMessage(HRESULT hr, const char* message) {
    char buffer[DIRECTSOUND_MAX_LOGMESSAGE_LENGTH];

    strcpy(buffer, message);
    strcat(buffer, " ");

    const char* desc = nullptr;

    switch (hr) {
    case DSERR_INVALIDCALL: { desc = "invalid call"; break; }
    case DSERR_OUTOFMEMORY: { desc = "out of mem"; break; }
    case DSERR_UNSUPPORTED: { desc = "Unsupported function"; break; }
    case DSERR_GENERIC: { desc = "generic error"; break; }
    case DSERR_NOAGGREGATION: { desc = "no aggregation"; break; }
    case DSERR_INVALIDPARAM: { desc = "invalid param"; break; }
    case DSERR_ALLOCATED: { desc = "allocated"; break; }
    case DSERR_CONTROLUNAVAIL: { desc = "control unavailable"; break; }
    case DSERR_PRIOLEVELNEEDED: { desc = "prio level needed"; break; }
    case DSERR_BADFORMAT: { desc = "bad format"; break; }
    case DSERR_NODRIVER: { desc = "no driver"; break; }
    case DSERR_ALREADYINITIALIZED: { desc = "already initialized"; break; }
    case DSERR_BUFFERLOST: { desc = "buffer lost"; break; }
    case DSERR_OTHERAPPHASPRIO: { desc = "other application has prio"; break; }
    case DSERR_UNINITIALIZED: { desc = "uninitialized"; break; }
    default: { desc = "fuck"; break; }
    }

    strcat(buffer, desc);

    g_pSysCom->Log("Z:\\Engine\\Sound\\_Wintel\\Source\\Misc.cpp", 59)
        ->LogMessage("DIRECT SOUND ERROR - %s", buffer);
}
