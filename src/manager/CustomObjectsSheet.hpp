#pragma once
#include <rect_structs.h>

enum Quality : int {
    LOW = 1,
    MEDIUM = 2,
    HIGH = 4
};

struct SheetInfo final {
    const class CustomSpriteConfig* m_sprite;
    rectpack2D::rect_wh m_offset;
    rectpack2D::rect_wh m_size;
    rectpack2D::rect_xywh m_trim;
    rectpack2D::rect_xywhf m_rect;

    auto& get_rect() { return m_rect; }
    const auto& get_rect() const { return m_rect; }
};

// Namespace for spritesheet helper functions
namespace CustomObjectsSheet {
    std::string getCacheDirectory();
    Quality getTextureQuality();
    std::string getSpritesheetQualityName(Quality = getTextureQuality());
    void addSpritesheetToCache(const std::vector<CustomSpriteConfig*>&, Quality);
    cocos2d::CCTexture2D* getCustomSpritesheetTexture();
}