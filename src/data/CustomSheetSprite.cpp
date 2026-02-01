#include "data/CustomSheetSprite.hpp"
#include "data/CustomSpriteConfig.hpp"
#include "CustomSpritesManager.hpp"

using namespace geode::prelude;

CustomSheetSprite::CustomSheetSprite() : rect({0, 0, 0, 0, false}) {}
CustomSheetSprite::CustomSheetSprite(const rectpack2D::rect_xywhf& rect) : rect(rect) {}

CustomSheetSprite::CustomSheetSprite(CustomSpriteConfig* sprite, Quality quality) : sprite(sprite) {
    auto sourceFrame = sprite->getSourceFrame();
    auto size = sprite->getSize();

    CCSpriteFrame* frame;
    if (auto spr = CCSpriteFrameCache::get()->m_pSpriteFrames->objectForKey(sourceFrame)) {
        frame = static_cast<CCSpriteFrame*>(spr);
    } else if (CCSprite spr; spr.initWithFile(sourceFrame.c_str())) {
        frame = spr.displayFrame();
    } else {
        log::error("Failed to find sprite \"{}\"", sourceFrame);
        this->size = CCSizeZero;
        return;
    } // if

    float qualityScale = (float)quality / (float)CustomSpritesManager::getTextureQuality();
    auto originalSize = frame->getOriginalSizeInPixels() * qualityScale;
    size = size.isZero() ? originalSize : size * (int)quality;

    float scaleX = size.width / originalSize.width;
    float scaleY = size.height / originalSize.height;

    auto offset = frame->getOffsetInPixels() * qualityScale;
    auto trimSize = frame->getRectInPixels().size * qualityScale;
    offset = CCPoint(offset.x * scaleX, offset.y * scaleY);
    trimSize = CCSize(trimSize.width * scaleX, trimSize.height * scaleY);

    int trimX = (size.width - trimSize.width) * 0.5f + offset.x;
    int trimY = (size.height - trimSize.height) * 0.5f - offset.y;

    this->size = size;
    this->offset = sprite->getOffset() * (int)quality;
    this->trim = {trimX, trimY, (int)trimSize.width, (int)trimSize.height};
    this->rect = {0, 0, (int)trimSize.width, (int)trimSize.height, false};
} // CustomSheetSprite

std::string CustomSheetSprite::getModID() const { return sprite->getModID(); }
std::string CustomSheetSprite::getFrameName() const { return sprite->getFrameName(); }
std::string CustomSheetSprite::getSourceFrame() const { return sprite->getSourceFrame(); }
bool CustomSheetSprite::isModTrigger() const { return sprite->isModTrigger(); }

std::string CustomSheetSprite::offString() const {
    int offsetX = (trim.x + trim.w * 0.5f) - (size.width * 0.5f) + offset.x;
    int offsetY = (size.height * 0.5f) - (trim.y + trim.h * 0.5f) + offset.y;
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