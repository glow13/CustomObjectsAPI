#pragma once
#include <Geode/Geode.hpp>
#include <rect_structs.h>

using namespace geode::prelude;

enum Quality : int;

struct CustomObjectSprite {
    std::string frame;
    std::string sourceFrame;
    CCPoint offset;
    CCSize size;

    rectpack2D::rect_xywhf rect;

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