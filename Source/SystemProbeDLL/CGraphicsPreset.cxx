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

#include "CGraphics.hxx"

// 0x0fbb2af0
CGraphicsPreset::CGraphicsPreset() {
    this->Next = nullptr;

    this->Settings.API                  = GRAPHICSAPI_UNKNOWN;
    this->Settings.Resolution           = GRAPHICSRESOLUTION_UNKNOWN;
    this->Settings.ColorDepth           = GRAPHICSCOLORDEPTH_UNKNOWN;
    this->Settings.ShadowQuality        = GRAPHICSSHADOWQUALITY_UNKNOWN;
    this->Settings.TextureQuality       = GRAPHICSTEXTUREQUALITY_UNKNOWN;
    this->Settings.Unk14                = GRAPHICSGENERICSETTING_UNKNOWN;
    this->Settings.AnisotropicFiltering = GRAPHICSGENERICSETTING_UNKNOWN;
    this->Settings.AntiAliasing         = GRAPHICSGENERICSETTING_UNKNOWN;
    this->Settings.TextureCompression   = GRAPHICSGENERICSETTING_UNKNOWN;
    this->Settings.PaletteCompression   = GRAPHICSGENERICSETTING_UNKNOWN;
    this->Settings.VertexLighing        = GRAPHICSGENERICSETTING_UNKNOWN;
    this->Settings.FullScreen           = GRAPHICSGENERICSETTING_UNKNOWN;
}

// 0x0fbb2b20
CGraphicsPreset::~CGraphicsPreset() {
    if (this->Next != nullptr) {
        delete this->Next;
    }
}

// 0x0fbb2b40
CGraphicsPreset* CGraphicsPreset::GetNext() {
    CGraphicsPreset* c = this;
    CGraphicsPreset* n = this->Next;

    while (n != nullptr) {
        c = n;
        n = n->Next;
    }

    c->Next = new CGraphicsPreset();

    return c->Next;
}
