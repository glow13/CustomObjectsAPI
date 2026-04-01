#include "CustomObjectsAPI.hpp"
#include "CustomObjectsManager.hpp"
#include "data/CustomObjectsMod.hpp"

using namespace geode::prelude;

Mod* CustomObjectsUtils::currentGeodeMod = nullptr;
int CustomObjectsUtils::currentOffset = 0;

CustomObjectsMod* CustomObjectsUtils::getMod(Mod* mod) {
    static CustomObjectsMod* currentMod = nullptr;

    if (mod != currentGeodeMod) {
        currentGeodeMod = mod;
        currentOffset = 0;
    } // if

    if (!currentMod || currentMod->getModID() != currentGeodeMod->getID()) {
        currentMod = CustomObjectsManager::get()->registerCustomObjectsMod(currentGeodeMod, currentOffset);
    } // if
    return currentMod;
} // getMod

void CustomObjectsUtils::setCollisionOffset(Mod* mod, uint8_t offset) {
    static CustomObjectsMod* currentMod = nullptr;

    if (mod != currentGeodeMod) {
        currentGeodeMod = mod;
    } // if

    if (auto geodeId = currentGeodeMod->getID(); (currentMod ? currentMod->getModID() : "") == geodeId) {
        log::error("Mod with id \"{}\" tried to incorrectly set the mod's collision offset!", geodeId);
        log::error("If you want to set the collision offset, it must be set BEFORE registering any objects!");
        throw std::logic_error("Incorrectly set collision offset");
    } // if

    currentOffset = offset;
} // setCollisionOffset

CustomObjectConfigBase* CustomObjectsUtils::registerCustomObject(geode::Mod* mod, CustomObjectConfigBase* (*factory)(CustomObjectsMod*), std::string spr, int offsetX, int offsetY, int width, int height) {
    return getMod(mod)->registerCustomObject(factory(getMod(mod)), spr, offsetX, offsetY, width, height);
} // registerCustomObject

CustomSpriteConfig* CustomObjectsUtils::registerCustomSprite(geode::Mod* mod, std::string spr, int offsetX, int offsetY, int width, int height) {
    return getMod(mod)->registerCustomSprite(spr, offsetX, offsetY, width, height);
} // registerCustomSprite

void CustomObjectsUtils::registerCustomAnimationSprites(geode::Mod* mod, std::string spr, int offsetX, int offsetY, int width, int height, int frames) {
    return getMod(mod)->registerCustomAnimationSprites(spr, offsetX, offsetY, width, height, frames);
} // registerCustomAnimationSprites