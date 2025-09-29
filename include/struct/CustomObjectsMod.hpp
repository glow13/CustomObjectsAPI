#pragma once
#include <Geode/Geode.hpp>

#include "object/CustomGameObject.hpp"

using namespace geode::prelude;

struct CustomObjectsMod {
    geode::Mod* mod;
    std::string modID;

    int objectID;
    std::vector<CustomSpriteConfig> sprites;
    std::vector<CustomObjectConfig> objects;

    CustomObjectsMod(geode::Mod* mod, char offset) : mod(mod), modID(mod->getID()) {
        uint32_t min = 10000;
        uint32_t max = INT32_MAX - 100;

        // Hash the mod id for a good objectID
        uint64_t hash = geode::utils::hash(modID + offset);
        uint64_t transform = min + (hash * (max - min)) / UINT32_MAX;
        objectID = transform - (transform % 100);
    } // CustomObjectsMod

    CustomObjectConfig& registerCustomObject(std::string spr, int sprWidth, int sprHeight, std::function<GameObject*(CustomObjectConfig)> create = CustomGameObject::create) {
        int id = objectID + objects.size();
        log::debug("Registered custom object with id {}", id);

        auto config = CustomObjectConfig(modID, id, create);
        config.mainSprite = CustomSpriteConfig(spr, modID, CCSize(sprWidth, sprHeight));
        return objects.emplace_back(config);
    } // registerCustomObject

    CustomObjectConfig& registerCustomObject(std::string spr, int sprSize, std::function<GameObject*(CustomObjectConfig)> create = CustomGameObject::create) { return registerCustomObject(spr, sprSize, sprSize, create); }
    CustomObjectConfig& registerCustomObject(std::string spr, std::function<GameObject*(CustomObjectConfig)> create = CustomGameObject::create) { return registerCustomObject(spr, 30, 30, create); }

    CustomSpriteConfig& registerCustomSprite(std::string spr, int sprWidth, int sprHeight) {
        log::debug("Registered custom sprite \"{}\"", spr);
        auto config = CustomSpriteConfig(spr, modID, CCSize(sprWidth, sprHeight));
        return sprites.emplace_back(config);
    } // registerCustomSprite

    CustomSpriteConfig& registerCustomSprite(std::string spr, int sprSize) { return registerCustomSprite(spr, sprSize, sprSize); }
    CustomSpriteConfig& registerCustomSprite(std::string spr) { return registerCustomSprite(spr, 30, 30); }
};