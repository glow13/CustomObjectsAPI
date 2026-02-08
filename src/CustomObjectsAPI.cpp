#include "CustomObjectsAPI.hpp"
#include "CustomObjectsManager.hpp"

using namespace geode::prelude;

Mod* CustomObjectsUtils::currentGeodeMod = nullptr;
CustomObjectsMod* CustomObjectsUtils::currentMod = nullptr;
int CustomObjectsUtils::currentOffset = 0;

CustomObjectsMod* CustomObjectsUtils::getMod(Mod* mod) {
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
    if (mod != currentGeodeMod) {
        currentGeodeMod = mod;
    } // if

    auto id = currentMod ? currentMod->getModID() : "";
    if (auto geodeId = currentGeodeMod->getID(); id == geodeId) {
        log::error("Mod with id \"{}\" tried to incorrectly set the mod's collision offset!", geodeId);
        log::error("If you want to set the collision offset, it must be set BEFORE registering any objects!");
        throw std::logic_error("Incorrectly set collision offset");
    } // if

    currentOffset = offset;
} // setCollisionOffset