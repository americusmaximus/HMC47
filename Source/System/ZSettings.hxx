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

enum GraphicsAnisotropy {
    GRAPHICSANISOTROPY_UNKNOWN      = -1,
    GRAPHICSANISOTROPY_OFF          = 0,
    GRAPHICSANISOTROPY_ON           = 1,
    GRAPHICSANISOTROPY_FORCE_DWORD  = 0x7FFFFFFF
};

enum GraphicsAPI {
    GRAPHICSAPI_UNKNOWN = -1,
    GRAPHICSAPI_DIRECTX = 0,
    GRAPHICSAPI_OPENGL = 1,
    GRAPHICSAPI_GLIDE3D = 2,
    GRAPHICSAPI_FORCE_DWORD = 0x7FFFFFFF
};

enum GraphicsResolution {
    GRAPHICSRESOLUTION_UNKNOWN      = -1,
    GRAPHICSRESOLUTION_640x480      = 0,
    GRAPHICSRESOLUTION_800x600      = 1,
    GRAPHICSRESOLUTION_1024x768     = 2,
    GRAPHICSRESOLUTION_1280x1024    = 3,
    GRAPHICSRESOLUTION_1600x1200    = 4,
    GRAPHICSRESOLUTION_FORCE_DWORD  = 0x7FFFFFFF
};

enum GraphicsColorDepth {
    GRAPHICSCOLORDEPTH_UNKNOWN      = -1,
    GRAPHICSCOLORDEPTH_16BIT        = 0,
    GRAPHICSCOLORDEPTH_32BIT        = 1,
    GRAPHICSCOLORDEPTH_FORCE_DWORD  = 0x7FFFFFFF
};

enum GraphicsShadowQuality {
    GRAPHICSSHADOWQUALITY_UNKNOWN       = -1,
    GRAPHICSSHADOWQUALITY_NONE          = 0,
    GRAPHICSSHADOWQUALITY_HITMAN        = 1,
    GRAPHICSSHADOWQUALITY_ALL           = 2,
    GRAPHICSSHADOWQUALITY_FORCE_DWORD   = 0x7FFFFFFF
};

enum GraphicsTextureQuality {
    GRAPHICSTEXTUREQUALITY_UNKNOWN      = -1,
    GRAPHICSTEXTUREQUALITY_VERY_LOW     = 0,
    GRAPHICSTEXTUREQUALITY_LOW          = 1,
    GRAPHICSTEXTUREQUALITY_MEDIUM       = 2,
    GRAPHICSTEXTUREQUALITY_FULL         = 3,
    GRAPHICSTEXTUREQUALITY_FORCE_DWORD  = 0x7FFFFFFF
};

enum GraphicsGenericSetting {
    GRAPHICSGENERICSETTING_UNKNOWN      = -1,
    GRAPHICSGENERICSETTING_OFF          = 0,
    GRAPHICSGENERICSETTING_ON           = 1,
    GRAPHICSGENERICSETTING_FORCE_DWORD  = 0x7FFFFFFF
};

enum GraphicsLevelOfDetail {
    GRAPHICSLEVELOFDETAIL_UNKNOWN       = -1,
    GRAPHICSLEVELOFDETAIL_0             = 0, // TODO
    GRAPHICSLEVELOFDETAIL_1             = 1, // TODO
    GRAPHICSLEVELOFDETAIL_FORCE_DWORD   = 0x7FFFFFFF
};
