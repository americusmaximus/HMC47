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

#pragma once

#include "LinkRefTab.hxx"
#include "XMLFile.hxx"

#include <System/ZString.hxx>

#define CONFIGFILE_MAX_KEY_COUNT            4

#pragma pack(push, 1)

enum ConfigFileState {
    CONFIGFILESTATE_INIT            = 0,
    CONFIGFILESTATE_CONTROLS        = 1,
    CONFIGFILESTATE_CONTROLS_KEY    = 2,
    CONFIGFILESTATE_CONTROLS_MOUSE  = 3,
    CONFIGFILESTATE_CONTROLS_KEYS   = 4,
    CONFIGFILESTATE_SOUND           = 5,
    CONFIGFILESTATE_DISPLAY         = 6,
    CONFIGFILESTATE_FORCE_DWORD     = 0x7FFFFFFF
};

struct SoundConfig {
    s32 SfxVolume;                                                                              // 0x0
    s32 MusicVolume;                                                                            // 0x4
    s32 SpeechVolume;                                                                           // 0x8
    s32 MusicQuality;                                                                           // 0xC
    s32 NumBuffers;                                                                             // 0x10
    bool UseHW;                                                                                 // 0x14
    bool UseEAX;                                                                                // 0x15
    bool UseStreaming;                                                                          // 0x16
};

struct KeyConfig {
    u32 Count;                                                                                  // 0x0
    ZString Action;                                                                             // 0x4
    s32 Codes[CONFIGFILE_MAX_KEY_COUNT];                                                        // 0x84
    bool DoubleTap;                                                                             // 0x94
};

struct MouseConfig {
    s32 Speed;                                                                                  // 0x0
    bool Invert;                                                                                // 0x4
};

struct KeyMapping {
    ZString Action;                                                                             // 0x0
    s32 Codes[CONFIGFILE_MAX_KEY_COUNT];                                                        // 0x80
    bool DoubleTap;                                                                             // 0x90
    u32 Unk0x91;                                                                                // 0x91
    u32 Unk0x95;                                                                                // 0x95
};

class ConfigFile : public XMLFile {
public:
    ConfigFile();

public:
    virtual ~ConfigFile();                                                                      // 0x0
    virtual void HandleStartElement(const char* name, const char** atts);                       // 0x4
    virtual void HandleEndElement(const char* name);                                            // 0x8
    virtual void HandleCharacterData(const char* s, s32 len);                                   // 0xC
    virtual void HandleProcessingInstruction(const char* target, const char* data);             // 0x10
    virtual void HandleSetDefault(const char* s, s32 len);                                      // 0x14
    virtual s32 HandleNotStandalone();                                                          // 0x18
    virtual void HandleUnparsedEntityDecl(const char* entityName, const char* base,
        const char* systemId, const char* publicId, const char* notationName);                  // 0x1C
    virtual void HandleNotationDecl(const char* notationName,
        const char* base, const char* systemId, const char* publicId);                          // 0x20
    virtual void HandleStartNamespaceDecl(const char* prefix, const char* uri);                 // 0x24
    virtual void HandleEndNamespaceDecl(const char* prefix);                                    // 0x28
    virtual u32 GetActionCount();                                                               // 0x2C
    virtual const char** GetActions();                                                          // 0x30
    virtual u32 AddMapping(const char* action,
        bool dbl, s32 code1, s32 code2, s32 code3, s32 code4);                                  // 0x34
    virtual void RemoveMapping(KeyMapping* value);                                              // 0x38
    virtual u32 Method0x3C(const char*, u32);                                                   // 0x3C
    virtual void Method0x40();                                                                  // 0x40
    virtual s32 GetMouseSpeed();                                                                // 0x44
    virtual void SetMouseSpeed(s32 value);                                                      // 0x48
    virtual void SetSoundSfxVolume(s32 value);                                                  // 0x4C
    virtual void SetSoundMusicVolume(s32 value);                                                // 0x50
    virtual void SetSoundSpeechVolume(s32 value);                                               // 0x54
    virtual void SetSoundUseEAX(bool value);                                                    // 0x58
    virtual void SetSoundUseHW(bool value);                                                     // 0x5C
    virtual void SetSoundMusicQuality(s32 value);                                               // 0x60
    virtual void SetSoundUseStreaming(bool value);                                              // 0x64
    virtual void SetSoundNumBuffers(s32 value);                                                 // 0x68
    virtual SoundConfig* GetSoundSettings();                                                    // 0x6C
    virtual void SetMouseInvert(bool value);                                                    // 0x70
    virtual bool GetMouseInvert();                                                              // 0x74
    virtual bool ReadConfigFile();                                                              // 0x78
    virtual bool Method0x7C();                                                                  // 0x7C
    virtual bool WriteConfigFile();                                                             // 0x80
    virtual bool Method0x84();                                                                  // 0x84
    virtual void ApplyKeySettings();                                                            // 0x88
    virtual void ApplyMouseSettings();                                                          // 0x8C
    virtual void ApplySoundSettings();                                                          // 0x90
    virtual void Method0x94();                                                                  // 0x94
    virtual void Method0x98(s32 value);                                                         // 0x98
    virtual void ApplyAllSettings();                                                            // 0x9C

protected:
    LinkRefTab Items;                                                                           // 0x8
    KeyConfig Keys;                                                                             // 0x30
    u8 Unk0xC5[12];                                                                             // 0xC5
    SoundConfig Sound;                                                                          // 0xD1
    ConfigFileState State;                                                                      // 0xE8
    MouseConfig Mouse;                                                                          // 0xEC
    u32 Unk0xF1;                                                                                // 0xF1
};

#pragma pack(pop)

#if defined(_DEBUG) && !defined(_WIN64)
static_assert(sizeof(ConfigFile)    == 0xF5,    "ConfigFile size mismatch.");
static_assert(sizeof(SoundConfig)   == 0x17,    "SoundConfig size mismatch.");
#endif
