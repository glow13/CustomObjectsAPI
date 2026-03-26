#pragma once
#include <Geode/Geode.hpp>

#include "CustomObjectConfig.hpp"

struct CUSTOM_OBJECTS_DLL CustomObjectsMod final {
private:
    const geode::Mod* mod;
    int baseObjectID;

    std::vector<CustomSpriteConfig*> sprites;
    std::vector<CustomObjectConfigBase*> objects;

    friend class CustomObjectsManager;
    friend class CustomObjectsUtils;

public:
    std::string getModID() const;
    std::string getModName() const;
    int getBaseObjectID() const;
    int getNextObjectID() const;

private:
    CustomObjectsMod(geode::Mod* mod, uint8_t offset);

    explicit CustomObjectsMod() = delete;
    CustomObjectsMod(const CustomObjectsMod&) = delete;
    CustomObjectsMod& operator=(const CustomObjectsMod&) = delete;

    CustomObjectConfigBase* registerCustomObject(CustomObjectConfigBase* config, std::string spr, int sprOffsetX, int sprOffsetY, int sprWidth, int sprHeight);
    CustomSpriteConfig* registerCustomSprite(std::string spr, int sprOffsetX, int sprOffsetY, int sprWidth, int sprHeight);
    void registerCustomAnimationSprites(std::string spr, int sprOffsetX, int sprOffsetY, int sprWidth, int sprHeight, int frames);
};