#include "data/CustomObjectsMod.hpp"

using namespace geode::prelude;

CustomObjectsMod::CustomObjectsMod(geode::Mod* mod, uint8_t offset) : mod(mod) {
    uint32_t min = 10000;
    uint32_t max = INT32_MAX - 100;

    // Hash the mod id for a good objectID
    uint64_t hash = geode::utils::hash(getModID() + (char)offset);
    uint64_t transform = min + (hash * (max - min)) / UINT32_MAX;
    baseObjectID = transform - (transform % 100);
} // CustomObjectsMod

std::string CustomObjectsMod::getModID() const { return mod->getID(); }
std::string CustomObjectsMod::getModName() const { return mod->getName(); }
int CustomObjectsMod::getBaseObjectID() const { return baseObjectID; }

CustomSpriteConfig& CustomObjectsMod::registerCustomSprite(std::string spr, int sprOffsetX, int sprOffsetY, int sprWidth, int sprHeight) {
    log::debug("Registered custom sprite \"{}\"", spr);
    auto config = new CustomSpriteConfig(this, nullptr, spr, sprOffsetX, sprOffsetY, sprWidth, sprHeight);
    sprites.emplace_back(config);
    return *config;
} // registerCustomSprite

void CustomObjectsMod::registerCustomAnimationSprites(std::string spr, int sprOffsetX, int sprOffsetY, int sprWidth, int sprHeight, int frames) {
    auto baseFrameName = spr.substr(0, spr.find("_001"));
    for (int i = 1; i <= frames; i++) {
        auto animFrameName = fmt::format("{}_{:03d}.png", baseFrameName, i);
        registerCustomSprite(animFrameName, sprOffsetX, sprOffsetY, sprWidth, sprHeight);
    } // for
} // registerCustomAnimation