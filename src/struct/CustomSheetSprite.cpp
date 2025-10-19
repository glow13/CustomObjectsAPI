#include "struct/CustomSheetSprite.hpp"

CustomObjectSprite::CustomObjectSprite() : frame(""), sourceFrame(""), rect({0, 0, 0, 0, false}) {}
CustomObjectSprite::CustomObjectSprite(const rectpack2D::rect_xywhf& rect) : frame(""), sourceFrame(""), rect(rect) {}
CustomObjectSprite::CustomObjectSprite(std::string frame, std::string sourceFrame, CCSize size, Quality quality) {
    this->frame = frame;
    this->sourceFrame = sourceFrame;
    this->size = size * quality;

    int scaledWidth = (int)size.width * quality;
    int scaledHeight = (int)size.height * quality;
    this->rect = {0, 0, scaledWidth, scaledHeight, false};
} // CustomObjectSprite

std::string CustomObjectSprite::offString() const {
    int offsetX = (offset.x * 2 + rect.w - size.width) * 0.5;
    int offsetY = (offset.y * 2 + rect.h - size.height) * -0.5;
    return "{" + fmt::format("{},{}", offsetX, offsetY) + "}";
} // offString

std::string CustomObjectSprite::sizeString() const {
    int flippedWidth = rect.flipped ? rect.h : rect.w;
    int flippedHeight = rect.flipped ? rect.w : rect.h;
    return "{" + fmt::format("{},{}", flippedWidth, flippedHeight) + "}";
} // sizeString

std::string CustomObjectSprite::rectString() const {
    return "{{" + fmt::format("{},{}", rect.x, rect.y) + "}," + sizeString() + "}";
} // rectString

std::string CustomObjectSprite::sourceString() const {
    return "{" + fmt::format("{},{}", size.width, size.height) + "}";
} // sourceString

std::string CustomObjectSprite::rotatedString() const {
    return rect.flipped ? "<true/>" : "<false/>";
} // rotatedString

void CustomObjectSprite::calculateTrimRect() {
    auto frames = CCSpriteFrameCache::sharedSpriteFrameCache()->m_pSpriteFrames;
    auto source = static_cast<CCSpriteFrame*>(frames->objectForKey(sourceFrame));
    if (!source) return;

    auto oSprite = CCSprite::createWithSpriteFrame(source);
    auto oSize = source->getOriginalSize();

    int csf = CCDirector::get()->getContentScaleFactor();
    auto render = CCRenderTexture::create((int)size.width / csf, (int)size.height / csf);
    render->beginWithClear(1, 1, 1, 0);
    oSprite->setAnchorPoint(CCPoint(0, 0));
    oSprite->setScaleX(size.width / oSize.width / csf);
    oSprite->setScaleY(size.height / oSize.height / csf);
    oSprite->visit();
    render->end();

    auto image = render->newCCImage();
    auto width = image->getWidth();
    auto height = image->getHeight();
    auto data = image->getData();
    image->release();

    int maxX = 0;
    int maxY = 0;
    int minX = width;
    int minY = height;
    int threshold = 5;

    // Find top edge
    for (int y = 0, total = 0; y < height && minY == height; y++) {
        for (int x = 0; x < width; x++) if (*(data + (y * width * 4) + (x * 4) + 3) > 0) total++;
        if (total >= threshold) minY = y;
    } // for

    // Find bottom edge
    for (int y = height - 1, total = 0; y >= minY && maxY == 0; y--) {
        for (int x = 0; x < width; x++) if (*(data + (y * width * 4) + (x * 4) + 3) > 0) total++;
        if (total >= threshold) maxY = y;
    } // for

    // Find left edge
    for (int x = 0, total = 0; x < width && minX == width; x++) {
        for (int y = minY; y <= maxY && minX == width; y++) if (*(data + (y * width * 4) + (x * 4) + 3) > 0) total++;
        if (total >= threshold) minX = x;
    } // for

    // Find right edge
    for (int x = width - 1, total = 0; x >= minX && maxX == 0; x--) {
        for (int y = minY; y <= maxY && maxX == 0; y++) if (*(data + (y * width * 4) + (x * 4) + 3) > 0) total++;
        if (total >= threshold) maxX = x;
    } // for

    offset = CCPoint(minX, minY);
    rect.w = maxX - minX + 1;
    rect.h = maxY - minY + 1;
} // calculateTrimRect