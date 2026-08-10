#include <Geode/Geode.hpp>
#include "../impl.hpp"

#include "../manager/CustomObjectsManager.hpp"
#include "../manager/CustomObjectsSheet.hpp"

using namespace geode::prelude;
using namespace rectpack2D;

// Checks sprite frames AND files
inline CCSpriteFrame* getCCSpriteFrame(std::string name) {
    if (auto frame = CCSpriteFrameCache::get()->m_pSpriteFrames->objectForKey(name))
        return static_cast<CCSpriteFrame*>(frame);
    else if (CCSprite spr; spr.initWithFile(name.c_str())) return spr.displayFrame();
    else return nullptr;
}

std::string formatSpriteFrameName(std::string frame, std::string mod, int x, int y, int w, int h) {
    return fmt::format("custom-objects/{}/{}.{}.{}.{}/{}", mod, x, y, w, h, frame.substr(frame.find("/") + 1));
}

CustomSpriteConfig::CustomSpriteConfig(CustomObjectConfig* object, std::string frameName, int x, int y, int w, int h, bool sheet) : m_impl(std::make_unique<Impl>()) {
    m_impl->m_object = object;
    m_impl->m_sourceFrame = frameName;
    m_impl->m_offset = rect_wh(x, y);
    m_impl->m_size = rect_wh(w, h);
    m_impl->m_customSprite = sheet;

    if (!sheet) return;
    m_impl->m_frameName = formatSpriteFrameName(frameName, object->getModID(), x, y, w, h);
    CustomObjectsManager::get()->registerCustomSprite(this);
}

CustomSpriteConfig::~CustomSpriteConfig() {
    if (isCustomSprite()) CustomObjectsManager::get()->unregisterCustomSprite(this);
}

std::string CustomSpriteConfig::getModID() const {
    return m_impl->m_object ? m_impl->m_object->getModID() : "";
}

std::string CustomSpriteConfig::getFrameName() const {
    return !m_impl->m_customSprite || m_impl->m_frameName.empty() ? m_impl->m_sourceFrame : m_impl->m_frameName;
}

std::string CustomSpriteConfig::getSourceFrame() const {
    return m_impl->m_sourceFrame;
}

SheetInfo CustomSpriteConfig::getSheetInfo(Quality quality) const {
    auto frame = getCCSpriteFrame(m_impl->m_sourceFrame);
    if (!frame) {
        log::error("Failed to find sprite \"{}\"", m_impl->m_sourceFrame);
        return SheetInfo {
            this,
            rect_wh(0, 0),
            rect_wh(30 * quality, 30 * quality),
            rect_xywh(0, 0, 30 * quality, 30 * quality),
            rect_xywhf(0, 0, 30 * quality, 30 * quality, false)
        };
    }

    auto size = CCSize(m_impl->m_size.w, m_impl->m_size.h);
    float qualityScale = (float)quality / (float)CustomObjectsSheet::getTextureQuality();
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

    return SheetInfo {
        this,
        rect_wh(m_impl->m_offset.w * quality, m_impl->m_offset.h * quality),
        rect_wh(size.width, size.height),
        rect_xywh(trimX, trimY, trimSize.width, trimSize.height),
        rect_xywhf(0, 0, trimSize.width, trimSize.height, false)
    };
}

bool CustomSpriteConfig::isCustomSprite() const {
    return m_impl->m_customSprite && !m_impl->m_frameName.empty();
}

bool CustomSpriteConfig::isAnimationFrame() const {
    return m_impl->m_sourceFrame.find("_001") != std::string::npos;
}

bool CustomSpriteConfig::isModTrigger() const {
    return false; //m_impl->m_object && m_impl->m_object->isModTrigger();
}