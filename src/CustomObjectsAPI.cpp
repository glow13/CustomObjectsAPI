#include "CustomObjectsAPI.hpp"
#include "CustomObjectsManager.hpp"
#include "data/CustomObjectsMod.hpp"

using namespace geode::prelude;

CustomObjectsMod* CustomObjectsUtils::getMod(Mod* mod, int offset = -1) {
    static CustomObjectsMod* currentMod = nullptr;
    static Mod* currentGeodeMod = nullptr;

    if (mod != currentGeodeMod) {
        currentGeodeMod = mod;
    } // if

    if (!currentMod || currentMod->getModID() != currentGeodeMod->getID()) {
        currentMod = CustomObjectsManager::get()->registerCustomObjectsMod(currentGeodeMod, offset < 0 ? 0 : offset);
        if (offset >= 0) return nullptr;
    } // if

    return currentMod;
} // getMod

void CustomObjectsUtils::setCollisionOffset(Mod* mod, uint8_t offset) {
    if (getMod(mod, offset)) {
        log::error("Mod with id \"{}\" tried to incorrectly set the mod's collision offset!", mod->getID());
        log::error("If you want to set the collision offset, it must be set BEFORE registering any objects!");
        throw std::logic_error("Incorrectly set collision offset");
    } // if
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