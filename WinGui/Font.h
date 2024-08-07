#pragma once
#include "Windows.h"

class Font {
public:
    Font(const wchar_t* typefaceName) {
        hFont = CreateFontW(
            16,                        // Height of font
            0,                         // Width of font
            0,                         // Angle of escapement
            0,                         // Orientation angle
            FW_NORMAL,                 // Font weight
            FALSE,                     // Italic attribute option
            FALSE,                     // Underline attribute option
            FALSE,                     // Strikeout attribute option
            ANSI_CHARSET,              // Character set identifier
            OUT_DEFAULT_PRECIS,        // Output precision
            CLIP_DEFAULT_PRECIS,       // Clipping precision
            CLEARTYPE_QUALITY,           // Output quality
            DEFAULT_PITCH | FF_SWISS,  // Family and pitch
            typefaceName                    // Font typeface name
        );
    }

    ~Font() {
        if (hFont) {
            DeleteObject(hFont);
        }
    }

    HFONT hFont;
};