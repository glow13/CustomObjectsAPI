#pragma once
#include <Geode/Geode.hpp>
#include <rect_structs.h>

using namespace geode::prelude;

enum Quality : int;

struct CustomSheetSprite {
    std::string frame;
    std::string sourceFrame;

    rectpack2D::rect_wh size; // desired final size of the sprite in the spritesheet
    rectpack2D::rect_xywh trim; // rect that designates the size of the not-transparent content in the scaled sprite
    rectpack2D::rect_xywhf rect; // rect that designates the size and position of this sprite in the spritesheet

    CustomSheetSprite();
    CustomSheetSprite(const rectpack2D::rect_xywhf& rect);
    CustomSheetSprite(std::string frame, std::string sourceFrame, CCSize size, Quality quality);

    std::string offString() const;
    std::string sizeString() const;
    std::string rectString() const;
    std::string sourceString() const;
    std::string rotatedString() const;

    inline auto& get_rect() { return rect; }
    inline const auto& get_rect() const { return rect; }
};