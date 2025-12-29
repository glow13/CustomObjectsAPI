#pragma once
#include <Geode/Geode.hpp>
#include <rect_structs.h>

using namespace geode::prelude;

enum Quality : int;

class CustomSpriteConfig;

struct CustomSheetSprite final {
private:
    CustomSpriteConfig* sprite;

    CCSize size; // desired final size of the sprite in the spritesheet
    CCPoint offset; // user-defined offset of the sprite, seperate from the trim process
    rectpack2D::rect_xywh trim; // rect that designates the size of the not-transparent content in the scaled sprite
    rectpack2D::rect_xywhf rect; // rect that designates the size and position of this sprite in the spritesheet

    friend class CustomObjectsSheet;

public:
    CustomSheetSprite();
    CustomSheetSprite(const rectpack2D::rect_xywhf& rect);
    CustomSheetSprite(CustomSpriteConfig* sprite, Quality quality);

    std::string getModID() const;
    std::string getFrameName() const;
    std::string getSourceFrame() const;
    bool isModTrigger() const;

    std::string offString() const;
    std::string sizeString() const;
    std::string rectString() const;
    std::string sourceString() const;
    std::string rotatedString() const;

    inline auto& get_rect() { return rect; }
    inline const auto& get_rect() const { return rect; }
};