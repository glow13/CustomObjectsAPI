#pragma once
#include <Geode/Geode.hpp>

struct CustomSpriteConfig;

enum Quality : int {
    LOW = 1,
    MEDIUM = 2,
    HIGH = 4
};

class CustomSpritesManager final {
private:
    std::vector<CustomSpriteConfig*> customSpritesCache;

    explicit CustomSpritesManager() = default;
    CustomSpritesManager(const CustomSpritesManager&) = delete;
    CustomSpritesManager& operator=(const CustomSpritesManager&) = delete;

public:
    static CustomSpritesManager* get();

    static std::string getCacheDirectory();
    static Quality getTextureQuality();
    static std::string getSpritesheetQualityName();

    void registerCustomObjectSprite(CustomSpriteConfig* spr);
    void processRegisteredSprites();

    bool isTheSpritesheetCacheUpToDate();
    void saveSpritesheetDataToCache(std::string name);
    void addSpritesheetToCache(std::string name, Quality quality);
};