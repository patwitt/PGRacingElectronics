/* DO NOT EDIT THIS FILE */
/* This file is autogenerated by the text-database code generator */

#ifndef TOUCHGFX_APPLICATIONFONTPROVIDER_HPP
#define TOUCHGFX_APPLICATIONFONTPROVIDER_HPP

#include <touchgfx/FontManager.hpp>

namespace touchgfx
{
class FlashDataReader;
}

struct Typography
{
    static const touchgfx::FontId DEFAULT = 0;
    static const touchgfx::FontId LARGE = 1;
    static const touchgfx::FontId SMALL = 2;
    static const touchgfx::FontId TYPOGRAPHY_00 = 3;
    static const touchgfx::FontId TYPOGRAPHY_01 = 4;
    static const touchgfx::FontId TYPOGRAPHY_02 = 5;
    static const touchgfx::FontId TYPOGRAPHY_03 = 6;
    static const touchgfx::FontId TYPOGRAPHY_04 = 7;
    static const touchgfx::FontId TYPOGRAPHY_05 = 8;
    static const touchgfx::FontId TYPOGRAPHY_06 = 9;
};

struct TypographyFontIndex
{
    static const touchgfx::FontId DEFAULT = 0;       // verdana_20_4bpp
    static const touchgfx::FontId LARGE = 1;         // verdana_40_4bpp
    static const touchgfx::FontId SMALL = 2;         // verdana_10_4bpp
    static const touchgfx::FontId TYPOGRAPHY_00 = 0; // verdana_20_4bpp
    static const touchgfx::FontId TYPOGRAPHY_01 = 3; // swisop3_300_4bpp
    static const touchgfx::FontId TYPOGRAPHY_02 = 4; // JetBrainsMono_Regular_40_4bpp
    static const touchgfx::FontId TYPOGRAPHY_03 = 5; // JetBrainsMono_Regular_60_4bpp
    static const touchgfx::FontId TYPOGRAPHY_04 = 6; // swisop3_50_4bpp
    static const touchgfx::FontId TYPOGRAPHY_05 = 5; // JetBrainsMono_Regular_60_4bpp
    static const touchgfx::FontId TYPOGRAPHY_06 = 0; // verdana_20_4bpp
    static const uint16_t NUMBER_OF_FONTS = 7;
};

class ApplicationFontProvider : public touchgfx::FontProvider
{
public:
    virtual touchgfx::Font* getFont(touchgfx::FontId typography);

    static void setFlashReader(touchgfx::FlashDataReader* /* flashReader */)
    {
    }
    static touchgfx::FlashDataReader* getFlashReader()
    {
        return 0;
    }
};

#endif // TOUCHGFX_APPLICATIONFONTPROVIDER_HPP
