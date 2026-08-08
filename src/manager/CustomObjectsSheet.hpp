#pragma once

enum Quality : int;
class CustomSpriteConfig;

// Namespace for spritesheet helper functions
namespace CustomObjectsSheet {
    std::string getCacheDirectory();
    Quality getTextureQuality();
    std::string getSpritesheetQualityName(Quality = getTextureQuality());
    void addSpritesheetToCache(const std::vector<CustomSpriteConfig*>&, Quality);
    cocos2d::CCTexture2D* getCustomSpritesheetTexture();
}