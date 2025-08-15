#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

enum Quality : unsigned int;

struct CustomObjectSprite {
    std::string frame;
    std::string sourceFrame;
    CCPoint pos;
    CCSize size;
    bool rotated;

    CustomObjectSprite() : frame(""), sourceFrame(""), pos(CCPoint(0, 0)), size(CCSize(30, 30)), rotated(false) {}
    CustomObjectSprite(std::string frame, std::string sourceFrame, CCSize size, Quality quality) : frame(frame), sourceFrame(sourceFrame), pos(CCPoint(0, 0)), size(size * quality), rotated(false) {}

    std::string sizeString() const { return "{" + fmt::format("{},{}", rotated ? size.height : size.width, rotated ? size.width : size.height) + "}"; }
    std::string rectString() const { return "{{" + fmt::format("{},{}", pos.x, pos.y) + "}," + sizeString() + "}"; }
    std::string rotatedString() const { return rotated ? "<true/>" : "<false/>"; }
};