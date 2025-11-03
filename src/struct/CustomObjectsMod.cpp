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

CustomObjectConfig& CustomObjectsMod::registerCustomObject(std::string spr, int sprWidth, int sprHeight, std::function<GameObject*(const CustomObjectConfig*)> create) {
    int id = objectID + objects.size();
    log::debug("Registered custom object with id {}", id);

    auto config = new CustomObjectConfig(modID, id, create);
    config->mainSprite = CustomSpriteConfig(spr, modID, CCSize(sprWidth, sprHeight));
    return *(objects.emplace_back(config));
} // registerCustomObject

CustomObjectConfig& CustomObjectsMod::registerCustomObject(std::string spr, int sprWidth, int sprHeight) {
    return registerCustomObject(spr, sprWidth, sprHeight, CustomGameObject::create);
} // registerCustomObject

void CustomObjectsMod::registerCustomSprite(std::string spr, int sprWidth, int sprHeight) {
    log::debug("Registered custom sprite \"{}\"", spr);
    auto config = new CustomSpriteConfig(spr, modID, CCSize(sprWidth, sprHeight));
    sprites.emplace_back(config);
} // registerCustomSprite

void CustomObjectsMod::registerCustomAnimationSprites(std::string spr, int sprWidth, int sprHeight, int frames) {
    auto baseFrame = spr.substr(0, spr.find("_001"));
    for (int i = 1; i <= frames; i++) {
        auto animFrame = fmt::format("{}_{:03d}.png", baseFrame, i);
        registerCustomSprite(animFrame, sprWidth, sprHeight);
    } // for
} // registerCustomAnimation