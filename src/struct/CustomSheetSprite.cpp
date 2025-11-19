#include "struct/CustomSheetSprite.hpp"

CustomSheetSprite::CustomSheetSprite() : frame(""), sourceFrame(""), rect({0, 0, 0, 0, false}) {}
CustomSheetSprite::CustomSheetSprite(const rectpack2D::rect_xywhf& rect) : frame(""), sourceFrame(""), rect(rect) {}
CustomSheetSprite::CustomSheetSprite(std::string frame, std::string sourceFrame, CCSize size, Quality quality) {
    this->frame = frame;
    this->sourceFrame = sourceFrame;

    // Find the original sprite frame
    auto frames = CCSpriteFrameCache::sharedSpriteFrameCache()->m_pSpriteFrames;
    auto source = static_cast<CCSpriteFrame*>(frames->objectForKey(sourceFrame));
    if (!source) {
        this->size = {(int)std::ceil(size.width * (int)quality), (int)std::ceil(size.height * (int)quality)};
        this->trim = {0, 0, this->size.w, this->size.h};
        this->rect = {0, 0, this->size.w, this->size.h, false};
        return;
    } // if

    // Find the original sprite and original size of the sprite
    auto oSprite = CCSprite::createWithSpriteFrame(source);
    auto oSize = source->getOriginalSize();

    // Set the size of the sprite, use the original size if no size was provided
    if (size.isZero()) this->size = {(int)std::ceil(oSize.width * (int)quality), (int)std::ceil(oSize.height * (int)quality)};
    else this->size = {(int)std::ceil(size.width * (int)quality), (int)std::ceil(size.height * (int)quality)};

    // Render the sprite at the desired size
    auto render = CCRenderTexture::create(this->size.w / quality, this->size.h / quality);
    render->beginWithClear(1, 1, 1, 0);
    oSprite->setAnchorPoint(CCPoint(0, 0));
    oSprite->setScaleX(this->size.w / oSize.width / (int)quality);
    oSprite->setScaleY(this->size.h / oSize.height / (int)quality);
    oSprite->visit();
    render->end();

    // Get the raw sprite pixel data
    auto image = render->newCCImage();
    auto width = image->getWidth();
    auto height = image->getHeight();
    auto data = image->getData();

    int maxX = 0;
    int maxY = 0;
    int minX = width;
    int minY = height;
    int threshold = 4;

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
        for (int y = minY; y <= maxY; y++) if (*(data + (y * width * 4) + (x * 4) + 3) > 0) total++;
        if (total >= threshold) minX = x;
    } // for

    // Find right edge
    for (int x = width - 1, total = 0; x >= minX && maxX == 0; x--) {
        for (int y = minY; y <= maxY; y++) if (*(data + (y * width * 4) + (x * 4) + 3) > 0) total++;
        if (total >= threshold) maxX = x;
    } // for

    // Calculate the sprite trim rect and sprite sheet rect
    int sprWidth = std::ceil((maxX - minX + 1) / (float)quality) * (int)quality;
    int sprHeight = std::ceil((maxY - minY + 1) / (float)quality) * (int)quality;
    this->trim = {minX, minY, sprWidth, sprHeight};
    this->rect = {0, 0, sprWidth, sprHeight, false};

    // Clean up
    oSprite->release();
    render->release();
    image->release();
} // CustomObjectSprite

std::string CustomSheetSprite::offString() const {
    int offsetX = (trim.x + trim.w / 2) - (size.w / 2);
    int offsetY = (size.h / 2) - (trim.y + trim.h / 2);
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
    return "{" + fmt::format("{},{}", size.w, size.h) + "}";
} // sourceString

std::string CustomSheetSprite::rotatedString() const {
    return rect.flipped ? "<true/>" : "<false/>";
} // rotatedString