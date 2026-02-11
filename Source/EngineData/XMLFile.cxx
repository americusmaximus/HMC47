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

#include <System/XMLFile.hxx>

void XMLCALL XMLStartElementHandler(void* userData, const char* name, const char** atts);
void XMLCALL XMLEndElementHandler(void* userData, const char* name);
void XMLCALL XMLCharacterDataHandler(void* userData, const char* s, s32 len);
void XMLCALL XMLProcessingInstructionHandler(void* userData, const char* target, const char* data);
void XMLCALL XMLDefaultHandler(void* userData, const char* s, s32 len);
void XMLCALL XMLUnparsedEntityDeclHandler(void* userData, const char* entityName, const char* base,
    const char* systemId, const char* publicId, const char* notationName);
void XMLCALL XMLNotationDeclHandler(void* userData,
    const char* notationName, const char* base, const char* systemId, const char* publicId);
s32 XMLCALL XMLNotStandaloneHandler(void* userData);
void XMLCALL XMLStartNamespaceDeclHandler(void* userData, const char* prefix, const char* uri);
void XMLCALL XMLEndNamespaceDeclHandler(void* userData, const char* prefix);

// 0x0ff84500
// 0x0ff90d5c
XMLFile::XMLFile(bool initialize) {
    this->Parser = nullptr;

    if (initialize) {
        this->Parser = XML_ParserCreate(nullptr);
        XML_SetUserData(this->Parser, this);
        XML_SetElementHandler(this->Parser, XMLStartElementHandler, XMLEndElementHandler);
        XML_SetCharacterDataHandler(this->Parser, XMLCharacterDataHandler);
        XML_SetProcessingInstructionHandler(this->Parser, XMLProcessingInstructionHandler);
        XML_SetDefaultHandler(this->Parser, XMLSetDefaultHandler);
        XML_SetUnparsedEntityDeclHandler(this->Parser, XMLUnparsedEntityDeclHandler);
        XML_SetNotationDeclHandler(this->Parser, XMLNotationDeclHandler);
        XML_SetNotStandaloneHandler(this->Parser, XMLNotStandaloneHandler);
        XML_SetNamespaceDeclHandler(this->Parser, XMLStartNamespaceDeclHandler, XMLEndNamespaceDeclHandler);
    }
}

// 0x0ff845c0
XMLFile::~XMLFile() {
    if (this->Parser != nullptr) {
        XML_ParserFree(this->Parser);
    }
}

// 0x0ff84610
void XMLCALL XMLStartElementHandler(void* userData, const char* name, const char** atts) {
    ((XMLFile*)userData)->HandleStartElement(name, atts);
}

// 0x0ff84630
void XMLCALL XMLEndElementHandler(void* userData, const char* name) {
    ((XMLFile*)userData)->HandleEndElement(name);
}

// 0x0ff84670
void XMLCALL XMLCharacterDataHandler(void* userData, const char* s, s32 len) {
    ((XMLFile*)userData)->HandleCharacterData(s, len);
}

// 0x0ff84690
void XMLCALL XMLProcessingInstructionHandler(void* userData, const char* target, const char* data) {
    ((XMLFile*)userData)->HandleProcessingInstruction(target, data);
}

// 0x0ff846b0
void XMLCALL XMLSetDefaultHandler(void* userData, const char* s, s32 len) {
    ((XMLFile*)userData)->HandleSetDefault(s, len);
}

// 0x0ff846e0
void XMLCALL XMLUnparsedEntityDeclHandler(void* userData, const char* entityName, const char* base,
    const char* systemId, const char* publicId, const char* notationName) {
    ((XMLFile*)userData)->HandleUnparsedEntityDecl(entityName, base, systemId, publicId, notationName);
}

// 0x0ff84710
void XMLCALL XMLNotationDeclHandler(void* userData,
    const char* notationName, const char* base, const char* systemId, const char* publicId) {
    ((XMLFile*)userData)->HandleNotationDecl(notationName, base, systemId, publicId);
}

// 0x0ff846d0
s32 XMLCALL XMLNotStandaloneHandler(void* userData) {
    return ((XMLFile*)userData)->HandleNotStandalone();
}

// 0x0ff84640
void XMLCALL XMLStartNamespaceDeclHandler(void* userData, const char* prefix, const char* uri) {
    return ((XMLFile*)userData)->HandleStartNamespaceDecl(prefix, uri);
}

// 0x0ff84660
void XMLCALL XMLEndNamespaceDeclHandler(void* userData, const char* prefix) {
    return ((XMLFile*)userData)->HandleEndNamespaceDecl(prefix);
}
