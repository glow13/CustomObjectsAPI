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
    std::vector<CustomSpriteConfig> sprites;
    std::vector<CustomObjectConfig> objects;

    CustomObjectsMod(geode::Mod* mod, char offset);

    CustomObjectConfig& registerCustomObject(std::string spr, int sprWidth, int sprHeight, std::function<GameObject*(CustomObjectConfig)> create);

    // Registers a default custom object with the type CustomGameObject
    CustomObjectConfig& registerCustomObject(std::string spr, int sprWidth, int sprHeight);

    CustomSpriteConfig& registerCustomSprite(std::string spr, int sprWidth, int sprHeight);

    // Inline helper functions
    inline CustomObjectConfig& registerCustomObject(std::string spr, int sprSize, std::function<GameObject*(CustomObjectConfig)> create) { return registerCustomObject(spr, sprSize, sprSize, create); }
    inline CustomObjectConfig& registerCustomObject(std::string spr, std::function<GameObject*(CustomObjectConfig)> create) { return registerCustomObject(spr, 0, 0, create); }
    inline CustomObjectConfig& registerCustomObject(std::string spr, int sprSize) { return registerCustomObject(spr, sprSize, sprSize); }
    inline CustomObjectConfig& registerCustomObject(std::string spr) { return registerCustomObject(spr, 0, 0); }
    inline CustomSpriteConfig& registerCustomSprite(std::string spr, int sprSize) { return registerCustomSprite(spr, sprSize, sprSize); }
    inline CustomSpriteConfig& registerCustomSprite(std::string spr) { return registerCustomSprite(spr, 0, 0); }
};