#include <Geode/Geode.hpp>
#include "CustomSpriteConfig.hpp"
#include "CustomObjectConfig.hpp"
#include "../manager/CustomObjectsManager.hpp"

using namespace geode::prelude;
using namespace rectpack2D;

CustomSpriteConfig::SheetInfo::SheetInfo(int x, int y, int w, int h) : m_offset(x, y), m_size(w, h), m_trim(0, 0, w, h), m_rect(0, 0, w, h, false) {}
CustomSpriteConfig::SheetInfo::SheetInfo(const rect_xywhf& rect) : m_rect(rect) {}

std::string CustomSpriteConfig::SheetInfo::offString() const {
    int offsetX = (m_trim.x + m_trim.w * 0.5f) - (m_size.w * 0.5f) + m_offset.w;
    int offsetY = (m_size.h * 0.5f) - (m_trim.y + m_trim.h * 0.5f) + m_offset.h;
    return "{" + fmt::format("{},{}", offsetX, offsetY) + "}";
} // offString

std::string CustomSpriteConfig::SheetInfo::sizeString() const {
    int width = m_rect.flipped ? m_rect.h : m_rect.w;
    int height = m_rect.flipped ? m_rect.w : m_rect.h;
    return "{" + fmt::format("{},{}", width, height) + "}";
} // sizeString

std::string CustomSpriteConfig::SheetInfo::rectString() const {
    return "{{" + fmt::format("{},{}", m_rect.x, m_rect.y) + "}," + sizeString() + "}";
} // rectString

std::string CustomSpriteConfig::SheetInfo::sourceString() const {
    return "{" + fmt::format("{},{}", m_size.w, m_size.h) + "}";
} // sourceString

std::string CustomSpriteConfig::SheetInfo::rotatedString() const {
    return m_rect.flipped ? "<true/>" : "<false/>";
} // rotatedString

struct CustomSpriteConfig::Impl {
    CustomObjectConfig* m_object;
    std::string m_frameName;
    std::string m_sourceFrame;
    std::unique_ptr<SheetInfo> m_sheet;
};

// Checks sprite frames AND files
CCSpriteFrame* getCCSpriteFrame(std::string frameName) {
    if (auto frame = CCSpriteFrameCache::get()->m_pSpriteFrames->objectForKey(frameName)) return static_cast<CCSpriteFrame*>(frame);
    else if (CCSprite spr; spr.initWithFile(frameName.c_str())) return spr.displayFrame();
    else return nullptr;
}

std::string formatSpriteFrameName(std::string frame, std::string mod, int x, int y, int w, int h) {
    return fmt::format("custom-objects/{}/{}.{}.{}.{}/{}", mod, x, y, w, h, frame.substr(frame.find("/") + 1));
}

CustomSpriteConfig::~CustomSpriteConfig() = default;
CustomSpriteConfig::CustomSpriteConfig(CustomObjectConfig* object, std::string frameName, int x, int y, int w, int h, bool sheet) : m_impl(std::make_unique<Impl>()) {
    m_impl->m_object = object;
    m_impl->m_sourceFrame = frameName;
    if (!sheet) return;

    m_impl->m_frameName = formatSpriteFrameName(frameName, object->getModID(), x, y, w, h);
    m_impl->m_sheet = std::make_unique<SheetInfo>(x, y, w, h);
    CustomObjectsManager::get()->registerSprite(this);
}

std::string CustomSpriteConfig::getModID() const {
    return m_impl->m_object->getModID();
}

std::string CustomSpriteConfig::getFrameName() const {
    return m_impl->m_sheet == nullptr || m_impl->m_frameName.empty() ? m_impl->m_sourceFrame : m_impl->m_frameName;
}

std::string CustomSpriteConfig::getSourceFrame() const {
    return m_impl->m_sourceFrame;
}

bool CustomSpriteConfig::isCustomSprite() const {
    return m_impl->m_sheet != nullptr && !m_impl->m_frameName.empty();
}

bool CustomSpriteConfig::isAnimationFrame() const {
    return m_impl->m_sourceFrame.find("_001") != std::string::npos;
}

bool CustomSpriteConfig::isModTrigger() const {
    return false; //m_impl->m_object && m_impl->m_object->isModTrigger();
}