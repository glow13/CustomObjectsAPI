#pragma once
#include <Geode/Geode.hpp>
#include <rect_structs.h>

using namespace geode::prelude;

enum Quality : int;

#define SPRITE_BUFFER 2

struct CustomObjectSprite {
    std::string frame;
    std::string sourceFrame;
    Quality quality;

    CCSize size; // actual size
    CCPoint offset; // trimmed offset

    rectpack2D::rect_xywhf rect; // trimmed size

    CustomObjectSprite();
    CustomObjectSprite(const rectpack2D::rect_xywhf& rect);
    CustomObjectSprite(std::string frame, std::string sourceFrame, CCSize size, Quality quality);

    void calculateTrimRect();

    std::string offString() const;
    std::string sizeString() const;
    std::string rectString() const;
    std::string sourceString() const;
    std::string rotatedString() const;

    inline auto& get_rect() { return rect; }
    inline const auto& get_rect() const { return rect; }
};