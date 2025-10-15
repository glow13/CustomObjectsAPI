#include "struct/CustomObjectSprite.hpp"

CustomObjectSprite::CustomObjectSprite() : frame(""), sourceFrame(""), rect({0, 0, 0, 0, false}) {}
CustomObjectSprite::CustomObjectSprite(const rectpack2D::rect_xywhf& rect) : frame(""), sourceFrame(""), rect(rect) {}
CustomObjectSprite::CustomObjectSprite(std::string frame, std::string sourceFrame, CCSize size, Quality quality) {
    this->frame = frame;
    this->sourceFrame = sourceFrame;
    this->size = size * quality;
    this->offset = CCPoint(0, 0);
    this->quality = quality;

    int scaledWidth = (int)size.width * quality + SPRITE_BUFFER;
    int scaledHeight = (int)size.height * quality + SPRITE_BUFFER;
    this->rect = {0, 0, scaledWidth, scaledHeight, false};
} // CustomObjectSprite

std::string CustomObjectSprite::offString() const {
    auto minYFlipped = size.height - (rect.h - SPRITE_BUFFER) - offset.y;
    int offsetX = (size.width  / 2.0f) - (offset.x + (rect.w - SPRITE_BUFFER)  / 2.0f);
    int offsetY = (size.height / 2.0f) - (minYFlipped + (rect.h - SPRITE_BUFFER) / 2.0f);
    return "{" + fmt::format("{},{}", -offsetX, -offsetY) + "}";
} // offString

std::string CustomObjectSprite::sizeString() const {
    int flippedWidth = (rect.flipped ? rect.h : rect.w) - SPRITE_BUFFER;
    int flippedHeight = (rect.flipped ? rect.w : rect.h) - SPRITE_BUFFER;
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
    log::info("{}", frame);
    auto frames = CCSpriteFrameCache::sharedSpriteFrameCache()->m_pSpriteFrames;
    auto source = static_cast<CCSpriteFrame*>(frames->objectForKey(sourceFrame));
    if (!source) return;

    auto oSprite = CCSprite::createWithSpriteFrame(source);
    auto oSize = source->getOriginalSize();

    auto render = CCRenderTexture::create((int)size.width / quality, (int)size.height / quality);
    render->beginWithClear(1, 1, 1, 0);
    oSprite->setAnchorPoint(CCPoint(0, 0));
    oSprite->setScaleX(size.width / (float)quality / oSize.width);
    oSprite->setScaleY(size.height / (float)quality / oSize.height);
    oSprite->visit();
    render->end();

    auto image = render->newCCImage();
    auto width = image->getWidth();
    auto height = image->getHeight();
    auto data = image->getData();
    image->release();

    log::info("{} {}", width, height);

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
    rect.w = maxX - minX + 1 + SPRITE_BUFFER;
    rect.h = maxY - minY + 1 + SPRITE_BUFFER;
    log::info("{} {} | {} {}", minX, maxX, minY, maxY);
} // calculateTrimRect