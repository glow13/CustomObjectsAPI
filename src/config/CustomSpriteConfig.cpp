#include <Geode/Geode.hpp>
#include "CustomSpriteConfig.hpp"
#include "CustomObjectConfig.hpp"
#include "../manager/CustomObjectsManager.hpp"

using namespace geode::prelude;
using namespace rectpack2D;

CustomSpriteConfig::SheetInfo::SheetInfo(int x, int y, int w, int h) :
    m_offset(x, y), m_size(w, h), m_trim(0, 0, w, h), m_rect(0, 0, w, h, false) {}

auto& CustomSpriteConfig::SheetInfo::get_rect() { return m_rect; }
const auto& CustomSpriteConfig::SheetInfo::get_rect() const { return m_rect; }

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

CustomSpriteConfig::CustomSpriteConfig(CustomObjectConfig* object, std::string frameName, int x, int y, int w, int h, bool sheet) : m_impl(std::make_unique<Impl>()) {
    m_impl->m_object = object;
    m_impl->m_sourceFrame = frameName;
    if (!sheet) return;

    m_impl->m_frameName = formatSpriteFrameName(frameName, object->getModID(), x, y, w, h);
    m_impl->m_sheet = std::make_unique<SheetInfo>(x, y, w, h);
    CustomObjectsManager::get()->registerCustomSprite(this);
}

CustomSpriteConfig::~CustomSpriteConfig() {
    if (isCustomSprite()) CustomObjectsManager::get()->unregisterCustomSprite(this);
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