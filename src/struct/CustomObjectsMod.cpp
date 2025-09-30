#include "struct/CustomObjectsMod.hpp"

#include "object/CustomGameObject.hpp"

CustomObjectsMod::CustomObjectsMod(geode::Mod* mod, char offset) : mod(mod), modID(mod->getID()) {
    uint32_t min = 10000;
    uint32_t max = INT32_MAX - 100;

    // Hash the mod id for a good objectID
    uint64_t hash = geode::utils::hash(modID + offset);
    uint64_t transform = min + (hash * (max - min)) / UINT32_MAX;
    objectID = transform - (transform % 100);
} // CustomObjectsMod

CustomObjectConfig& CustomObjectsMod::registerCustomObject(std::string spr, int sprWidth, int sprHeight, std::function<GameObject*(CustomObjectConfig)> create) {
    int id = objectID + objects.size();
    log::debug("Registered custom object with id {}", id);

    auto config = CustomObjectConfig(modID, id, create);
    config.mainSprite = CustomSpriteConfig(spr, modID, CCSize(sprWidth, sprHeight));
    return objects.emplace_back(config);
} // registerCustomObject

CustomObjectConfig& CustomObjectsMod::registerCustomObject(std::string spr, int sprWidth, int sprHeight) {
    return registerCustomObject(spr, sprWidth, sprHeight, CustomGameObject::create);
} // registerCustomObject

CustomSpriteConfig& CustomObjectsMod::registerCustomSprite(std::string spr, int sprWidth, int sprHeight) {
    log::debug("Registered custom sprite \"{}\"", spr);
    auto config = CustomSpriteConfig(spr, modID, CCSize(sprWidth, sprHeight));
    return sprites.emplace_back(config);
} // registerCustomSprite