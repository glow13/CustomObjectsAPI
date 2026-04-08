#pragma once

#include <Geode/loader/Mod.hpp>

#include "CustomObjectConfig.hpp"

class CustomObjectsMod final {
private:
    const geode::Mod* mod;
    int baseObjectID;

    std::vector<std::unique_ptr<CustomSpriteConfig>> sprites;
    std::vector<std::unique_ptr<CustomObjectConfigBase>> objects;

    friend class CustomObjectsManager;
    friend class CustomObjectsUtils;

public:
    std::string getModID() const;
    std::string getModName() const;
    int getBaseObjectID() const;
    int getNextObjectID() const;

    CustomObjectsMod(geode::Mod* mod, uint8_t offset);

private:
    explicit CustomObjectsMod() = delete;
    CustomObjectsMod(const CustomObjectsMod&) = delete;
    CustomObjectsMod& operator=(const CustomObjectsMod&) = delete;

    CustomObjectConfigBase* registerCustomObject(std::unique_ptr<CustomObjectConfigBase>& config, std::string spr, int sprOffsetX, int sprOffsetY, int sprWidth, int sprHeight);
    CustomSpriteConfig* registerCustomSprite(std::string spr, int sprOffsetX, int sprOffsetY, int sprWidth, int sprHeight);
    void registerCustomAnimationSprites(std::string spr, int sprOffsetX, int sprOffsetY, int sprWidth, int sprHeight, int frames);
};