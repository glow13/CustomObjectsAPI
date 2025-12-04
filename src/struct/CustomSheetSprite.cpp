#include "struct/CustomSheetSprite.hpp"

CustomSheetSprite::CustomSheetSprite() : frame(""), sourceFrame(""), rect({0, 0, 0, 0, false}) {}
CustomSheetSprite::CustomSheetSprite(const rectpack2D::rect_xywhf& rect) : frame(""), sourceFrame(""), rect(rect) {}
CustomSheetSprite::CustomSheetSprite(std::string frame, std::string sourceFrame, CCRect rect, Quality quality) {
    this->frame = frame;
    this->sourceFrame = sourceFrame;
    this->offset = {(int)rect.origin.x * quality, (int)rect.origin.y * quality};

    // Find the original sprite frame
    auto frames = CCSpriteFrameCache::sharedSpriteFrameCache()->m_pSpriteFrames;
    auto source = static_cast<CCSpriteFrame*>(frames->objectForKey(sourceFrame));
    if (!source) {
        this->size = {(int)rect.size.width * quality, (int)rect.size.height * quality};
        this->trim = {0, 0, this->size.w, this->size.h};
        this->rect = {0, 0, this->size.w, this->size.h, false};
        return;
    } // if

    // Find the original sprite and original size of the sprite
    auto oSprite = CCSprite::createWithSpriteFrame(source);
    auto oSize = source->getOriginalSize();

    // Set the size of the sprite, use the original size if no size was provided
    if (rect.size.isZero()) this->size = {(int)oSize.width * quality, (int)oSize.height * quality};
    else this->size = {(int)rect.size.width * quality, (int)rect.size.height * quality};

    // Render the sprite at the desired size
    auto render = CCRenderTexture::create(this->size.w / quality, this->size.h / quality);
    render->beginWithClear(1, 1, 1, 0);
    oSprite->setAnchorPoint(CCPoint(0, 0));
    oSprite->setScaleX(this->size.w / oSize.width / (int)quality);
    oSprite->setScaleY(this->size.h / oSize.height / (int)quality);
    oSprite->visit();

    // Get the raw sprite pixel data
    auto& s = render->m_pTexture->getContentSizeInPixels();
    unsigned int w = s.width, h = s.height;

    std::vector<uint8_t> data(w * h * 4);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, data.data());

    render->end();
    render->release();
    oSprite->release();

    // Calculate the minimum rect to contain the sprite
    int maxX = 0;
    int maxY = 0;
    int minX = w - 1;
    int minY = h - 1;
    int threshold = 4;

    // Find top edge
    for (int y = 0, total = 0; y < h && minY == h - 1; y++) {
        for (int x = 0; x < w; x++) if (data[(y * w + x) * 4 + 3]) total++;
        if (total >= threshold) minY = y;
    } // for

    // Find bottom edge
    for (int y = h - 1, total = 0; y >= minY && maxY == 0; y--) {
        for (int x = 0; x < w; x++) if (data[(y * w + x) * 4 + 3]) total++;
        if (total >= threshold) maxY = y;
    } // for

    // Find left edge
    for (int x = 0, total = 0; x < w && minX == w - 1; x++) {
        for (int y = minY; y <= maxY; y++) if (data[(y * w + x) * 4 + 3]) total++;
        if (total >= threshold) minX = x;
    } // for

    // Find right edge
    for (int x = w - 1, total = 0; x >= minX && maxX == 0; x--) {
        for (int y = minY; y <= maxY; y++) if (data[(y * w + x) * 4 + 3]) total++;
        if (total >= threshold) maxX = x;
    } // for

    // Calculate the sprite trim rect and sprite sheet rect
    int sprWidth = std::ceil((maxX - minX + 1) / (float)quality) * (int)quality;
    int sprHeight = std::ceil((maxY - minY + 1) / (float)quality) * (int)quality;
    this->trim = {minX, (int)h - maxY - 1, sprWidth, sprHeight}; // flip y
    this->rect = {0, 0, sprWidth, sprHeight, false};
} // CustomObjectSprite

std::string CustomSheetSprite::offString() const {
    int offsetX = (trim.x + trim.w / 2) - (size.w / 2);
    int offsetY = (size.h / 2) - (trim.y + trim.h / 2);
    return "{" + fmt::format("{},{}", offsetX + offset.w, offsetY + offset.h) + "}";
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