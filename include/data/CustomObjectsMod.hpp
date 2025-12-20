#pragma once
#include <Geode/Geode.hpp>

#include "data/CustomObjectConfig.hpp"

using namespace geode::prelude;

class CustomGameObject;

struct CustomObjectsMod {
public:
    geode::Mod* mod;
    std::string modID;

    int objectID;
    std::vector<CustomSpriteConfig*> sprites;
    std::vector<ICustomObjectConfig*> objects;

    CustomObjectsMod(geode::Mod* mod, char offset);

    template <class ObjectType = CustomGameObject>
    CustomObjectConfig<ObjectType>& registerCustomObject(std::string spr, int sprOffsetX, int sprOffsetY, int sprWidth, int sprHeight) {
        int id = objectID + objects.size();
        log::debug("Registered custom object with id {}", id);

        auto config = new CustomObjectConfig<ObjectType>(modID, id);
        config->mainSprite = CustomSpriteConfig(spr, modID, CCRect(sprOffsetX, sprOffsetY, sprWidth, sprHeight));
        objects.emplace_back(static_cast<ICustomObjectConfig*>(config));
        return *config;
    } // registerCustomObject

    template <class ObjectType = CustomGameObject>
    inline CustomObjectConfig<ObjectType>& registerCustomObject(std::string spr, int sprWidth, int sprHeight) {
        return registerCustomObject<ObjectType>(spr, 0, 0, sprWidth, sprHeight);
    } // registerCustomObject

    template <class ObjectType = CustomGameObject>
    inline CustomObjectConfig<ObjectType>& registerCustomObject(std::string spr, int sprSize) {
        return registerCustomObject<ObjectType>(spr, 0, 0, sprSize, sprSize);
    } // registerCustomObject

    template <class ObjectType = CustomGameObject>
    inline CustomObjectConfig<ObjectType>& registerCustomObject(std::string spr) {
        return registerCustomObject<ObjectType>(spr, 0, 0, 0, 0);
    } // registerCustomObject

    void registerCustomSprite(std::string spr, int sprOffsetX, int sprOffsetY, int sprWidth, int sprHeight);
    void registerCustomAnimationSprites(std::string spr, int sprOffsetX, int sprOffsetY, int sprWidth, int sprHeight, int frames);

    // Inline helper functions
    inline void registerCustomAnimationSprites(std::string spr, int sprWidth, int sprHeight, int frames) { return registerCustomAnimationSprites(spr, 0, 0, sprWidth, sprHeight, frames); }
    inline void registerCustomAnimationSprites(std::string spr, int sprSize, int frames) { return registerCustomAnimationSprites(spr, 0, 0, sprSize, sprSize, frames); }
    inline void registerCustomAnimationSprites(std::string spr, int frames) { return registerCustomAnimationSprites(spr, 0, 0, 0, 0, frames); }
    inline void registerCustomSprite(std::string spr, int sprWidth, int sprHeight) { return registerCustomSprite(spr, 0, 0, sprWidth, sprHeight); }
    inline void registerCustomSprite(std::string spr, int sprSize) { return registerCustomSprite(spr, 0, 0, sprSize, sprSize); }
    inline void registerCustomSprite(std::string spr) { return registerCustomSprite(spr, 0, 0, 0, 0); }
};