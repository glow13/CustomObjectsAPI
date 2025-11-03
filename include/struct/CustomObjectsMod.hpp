#pragma once
#include <Geode/Geode.hpp>

#include "struct/CustomSpriteConfig.hpp"
#include "struct/CustomObjectConfig.hpp"

using namespace geode::prelude;

struct CustomObjectsMod {
public:
    geode::Mod* mod;
    std::string modID;

    int objectID;
    std::vector<CustomSpriteConfig*> sprites;
    std::vector<CustomObjectConfig*> objects;

    CustomObjectsMod(geode::Mod* mod, char offset);

    CustomObjectConfig& registerCustomObject(std::string spr, int sprWidth, int sprHeight, std::function<GameObject*(const CustomObjectConfig*)> create);

    // Registers a default custom object with the type CustomGameObject
    CustomObjectConfig& registerCustomObject(std::string spr, int sprWidth, int sprHeight);

    void registerCustomSprite(std::string spr, int sprWidth, int sprHeight);
    void registerCustomAnimationSprites(std::string spr, int sprWidth, int sprHeight, int frames);

    // Inline helper functions
    inline CustomObjectConfig& registerCustomObject(std::string spr, int sprSize, std::function<GameObject*(const CustomObjectConfig*)> create) { return registerCustomObject(spr, sprSize, sprSize, create); }
    inline CustomObjectConfig& registerCustomObject(std::string spr, std::function<GameObject*(const CustomObjectConfig*)> create) { return registerCustomObject(spr, 0, 0, create); }
    inline CustomObjectConfig& registerCustomObject(std::string spr, int sprSize) { return registerCustomObject(spr, sprSize, sprSize); }
    inline CustomObjectConfig& registerCustomObject(std::string spr) { return registerCustomObject(spr, 0, 0); }
    inline void registerCustomAnimationSprites(std::string spr, int sprSize, int frames) { return registerCustomAnimationSprites(spr, sprSize, sprSize, frames); }
    inline void registerCustomAnimationSprites(std::string spr, int frames) { return registerCustomAnimationSprites(spr, 0, 0, frames); }
    inline void registerCustomSprite(std::string spr, int sprSize) { return registerCustomSprite(spr, sprSize, sprSize); }
    inline void registerCustomSprite(std::string spr) { return registerCustomSprite(spr, 0, 0); }
};