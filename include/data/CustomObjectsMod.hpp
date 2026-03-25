#pragma once
#include <Geode/Geode.hpp>

#include "CustomObjectConfig.hpp"

class CustomGameObject;

struct CUSTOM_OBJECTS_DLL CustomObjectsMod final {
private:
    const geode::Mod* mod;
    int baseObjectID;

    std::vector<CustomSpriteConfig*> sprites;
    std::vector<CustomObjectConfigBase*> objects;

    friend class CustomObjectsManager;
    friend class CustomObjectsAPI;

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

    template <class ObjectType = CustomGameObject>
    CustomObjectConfig<ObjectType>& registerCustomObject(std::string spr, int sprOffsetX, int sprOffsetY, int sprWidth, int sprHeight) {
        auto config = new CustomObjectConfig<ObjectType>(this);
        config->setMainSprite(spr, sprOffsetX, sprOffsetY, sprWidth, sprHeight);
        objects.emplace_back(config);
        return *config;
    } // registerCustomObject

    CustomSpriteConfig& registerCustomSprite(std::string spr, int sprOffsetX, int sprOffsetY, int sprWidth, int sprHeight);
    void registerCustomAnimationSprites(std::string spr, int sprOffsetX, int sprOffsetY, int sprWidth, int sprHeight, int frames);
};