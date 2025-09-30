#include "struct/CustomObjectSprite.hpp"

CustomObjectSprite::CustomObjectSprite() : frame(""), sourceFrame(""), rect({0, 0, 0, 0, false}) {}
CustomObjectSprite::CustomObjectSprite(const rectpack2D::rect_xywhf& rect) : frame(""), sourceFrame(""), rect(rect) {}
CustomObjectSprite::CustomObjectSprite(std::string frame, std::string sourceFrame, CCSize size, Quality quality) {
    this->frame = frame;
    this->sourceFrame = sourceFrame;

    int scaledWidth = (int)size.width * quality + SPRITE_BUFFER;
    int scaledHeight = (int)size.height * quality + SPRITE_BUFFER;
    this->rect = {0, 0, scaledWidth, scaledHeight, false};
} // CustomObjectSprite

std::string CustomObjectSprite::sizeString() const {
    int flippedWidth = (rect.flipped ? rect.h : rect.w) - SPRITE_BUFFER;
    int flippedHeight = (rect.flipped ? rect.w : rect.h) - SPRITE_BUFFER;
    return "{" + fmt::format("{},{}", flippedWidth, flippedHeight) + "}";
} // sizeString

std::string CustomObjectSprite::rectString() const {
    return "{{" + fmt::format("{},{}", rect.x, rect.y) + "}," + sizeString() + "}";
} // rectString

std::string CustomObjectSprite::rotatedString() const {
    return rect.flipped ? "<true/>" : "<false/>";
} // rotatedString