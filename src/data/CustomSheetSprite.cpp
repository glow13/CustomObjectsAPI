#include "data/CustomSheetSprite.hpp"

CustomSheetSprite::CustomSheetSprite() : frameName(""), sourceFrame(""), rect({0, 0, 0, 0, false}) {}
CustomSheetSprite::CustomSheetSprite(const rectpack2D::rect_xywhf& rect) : frameName(""), sourceFrame(""), rect(rect) {}
CustomSheetSprite::CustomSheetSprite(std::string frameName, std::string sourceFrame, CCRect rect, Quality quality) {
    this->frameName = frameName;
    this->sourceFrame = sourceFrame;

    auto frame = CCSpriteFrameCache::get()->spriteFrameByName(sourceFrame.c_str());
    auto originalSize = frame->getOriginalSizeInPixels();

    rect.size = rect.size.isZero() ? originalSize : rect.size * (int)quality;
    this->offset = rect.origin * (int)quality;
    this->size = rect.size;

    float scaleX = rect.size.width / originalSize.width;
    float scaleY = rect.size.height / originalSize.height;

    auto offset = frame->getOffsetInPixels();
    auto trimSize = frame->getRectInPixels().size;
    offset = CCPoint(offset.x * scaleX, offset.y * scaleY);
    trimSize = CCSize(trimSize.width * scaleX, trimSize.height * scaleY);

    int trimX = (size.width - trimSize.width) * 0.5f + offset.x;
    int trimY = (size.height - trimSize.height) * 0.5f - offset.y;

    this->trim = {trimX, trimY, (int)trimSize.width, (int)trimSize.height};
    this->rect = {0, 0, (int)trimSize.width, (int)trimSize.height, false};
} // CustomSheetSprite

std::string CustomSheetSprite::offString() const {
    int offsetX = (trim.x + trim.w / 2) - (size.width / 2) + offset.x;
    int offsetY = (size.height / 2) - (trim.y + trim.h / 2) + offset.y;
    return "{" + fmt::format("{},{}", offsetX, offsetY) + "}";
} // offString

std::string CustomSheetSprite::sizeString() const {
    int width = rect.flipped ? rect.h : rect.w;
    int height = rect.flipped ? rect.w : rect.h;
    return "{" + fmt::format("{},{}", width, height) + "}";
} // sizeString

std::string CustomSheetSprite::rectString() const {
    return "{{" + fmt::format("{},{}", rect.x, rect.y) + "}," + sizeString() + "}";
} // rectString

std::string CustomSheetSprite::sourceString() const {
    return "{" + fmt::format("{},{}", size.width, size.height) + "}";
} // sourceString

std::string CustomSheetSprite::rotatedString() const {
    return rect.flipped ? "<true/>" : "<false/>";
} // rotatedString