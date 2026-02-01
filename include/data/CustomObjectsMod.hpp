#pragma once
#include <Geode/Geode.hpp>

#include "CustomObjectConfig.hpp"

class CustomGameObject;

struct CUSTOM_OBJECTS_DLL CustomObjectsMod final {
private:
    geode::Mod* mod;
    int baseObjectID;

    std::vector<CustomSpriteConfig*> sprites;
    std::vector<CustomObjectConfigBase*> objects;

    friend class CustomObjectsManager;
    friend class CustomObjectsAPI;

public:
    std::string getModID() const;
    std::string getModName() const;
    int getBaseObjectID() const;

private:
    CustomObjectsMod(geode::Mod* mod, uint8_t offset);

    template <class ObjectType = CustomGameObject>
    CustomObjectConfig<ObjectType>& registerCustomObject(std::string spr, int sprOffsetX, int sprOffsetY, int sprWidth, int sprHeight) {
        int id = baseObjectID + objects.size();
        geode::log::debug("Registered custom object with id {}", id);

        auto config = new CustomObjectConfig<ObjectType>(this, id);
        config->setMainSprite(spr, sprOffsetX, sprOffsetY, sprWidth, sprHeight);
        objects.emplace_back(static_cast<CustomObjectConfigBase*>(config));
        return *config;
    } // registerCustomObject

    CustomSpriteConfig& registerCustomSprite(std::string spr, int sprOffsetX, int sprOffsetY, int sprWidth, int sprHeight);
    void registerCustomAnimationSprites(std::string spr, int sprOffsetX, int sprOffsetY, int sprWidth, int sprHeight, int frames);
};