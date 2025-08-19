#pragma once
#include <Geode/Geode.hpp>
#include <rect_structs.h>

using namespace geode::prelude;

enum Quality : int;

#define SPRITE_BUFFER 2

struct CustomObjectSprite {
    std::string frame;
    std::string sourceFrame;
    rectpack2D::rect_xywhf rect;

    CustomObjectSprite() : frame(""), sourceFrame(""), rect({0, 0, 30 + SPRITE_BUFFER, 30 + SPRITE_BUFFER, false}) {}
    CustomObjectSprite(const rectpack2D::rect_xywhf& rect) : frame(""), sourceFrame(""), rect(rect) {}
    CustomObjectSprite(std::string frame, std::string sourceFrame, rectpack2D::rect_wh size, Quality quality) : frame(frame), sourceFrame(sourceFrame), rect({0, 0, size.w * quality + SPRITE_BUFFER, size.h * quality + SPRITE_BUFFER, false}) {}

    std::string sizeString() const { return "{" + fmt::format("{},{}", (rect.flipped ? rect.h : rect.w) - SPRITE_BUFFER, (rect.flipped ? rect.w : rect.h) - SPRITE_BUFFER) + "}"; }
    std::string rectString() const { return "{{" + fmt::format("{},{}", rect.x, rect.y) + "}," + sizeString() + "}"; }
    std::string rotatedString() const { return rect.flipped ? "<true/>" : "<false/>"; }

    auto& get_rect() { return rect; }
    const auto& get_rect() const { return rect; }
};