#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

struct CustomSpriteConfig;

enum Quality : int {
    LOW = 1,
    MEDIUM = 2,
    HIGH = 4
};

class CustomSpritesManager {
private:
    static inline CustomSpritesManager* s_manager;

    std::vector<CustomSpriteConfig*> customSpritesCache;

public:
    static CustomSpritesManager* get();

    std::string getCacheDirectory();
    Quality getTextureQuality();
    std::string getSpritesheetQualityName();

    void registerCustomObjectSprite(CustomSpriteConfig* spr);
    void processRegisteredSprites();

    bool isTheSpritesheetCacheUpToDate();
    void saveSpritesheetDataToCache(std::string name);
    void addSpritesheetToCache(std::string name, Quality quality);

    static CCSize getPixelDataFromSprite(CCSprite* spr, ByteVector& data);
    static CCSprite* getSpriteFromPixelData(ByteVector& data, CCSize size);
};