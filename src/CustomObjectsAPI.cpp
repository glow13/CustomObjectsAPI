#include "CustomObjectsAPI.hpp"
#include "CustomObjectsManager.hpp"

using namespace geode::prelude;

Mod* CustomObjectsUtils::currentGeodeMod = nullptr;
CustomObjectsMod* CustomObjectsUtils::currentMod = nullptr;
int CustomObjectsUtils::currentOffset = 0;

CustomObjectsMod* CustomObjectsUtils::getMod(Mod* mod) {
    // log::info("old {} {} {}", currentGeodeMod, currentMod ? currentMod->getModID() : "no", currentOffset);
    if (mod != currentGeodeMod) {
        currentGeodeMod = mod;
        currentOffset = 0;
    } // if

    if (!currentMod || currentMod->getModID() != currentGeodeMod->getID()) {
        currentMod = CustomObjectsManager::get()->registerCustomObjectsMod(currentGeodeMod, currentOffset);
    } // if
    // log::info("new {} {} {}", currentGeodeMod, currentMod->getModID(), currentOffset);
    return currentMod;
} // getMod

void CustomObjectsUtils::setCollisionOffset(Mod* mod, uint8_t offset) {
    if (mod != currentGeodeMod) {
        currentGeodeMod = mod;
    } // if
    assert(currentMod->getModID() != currentGeodeMod->getID() && "must set collision offset before registering any objects lol");
    currentOffset = offset;
} // setCollisionOffset