#include "CustomObjectsAPI.hpp"

#include "CustomObjectsManager.hpp"

namespace CustomObjectsUtils {
    namespace {
        inline geode::Mod* currentGeodeMod;

        inline int currentOffset;
        inline CustomObjectsMod* currentMod;
    }

    CustomObjectsMod* getMod(geode::Mod* mod) {
        log::info("old {} {} {}", currentGeodeMod, currentMod ? currentMod->getModID() : "no", currentOffset);
        if (mod != currentGeodeMod) {
            currentGeodeMod = mod;
            currentOffset = 0;
        } // if

        if (!currentMod || currentMod->getModID() != currentGeodeMod->getID()) {
            currentMod = CustomObjectsManager::get()->registerCustomObjectsMod(currentGeodeMod, currentOffset);
        } // if
        log::info("new {} {} {}", currentGeodeMod, currentMod->getModID(), currentOffset);
        return currentMod;
    } // getMod

    void setCollisionOffset(geode::Mod* mod, uint8_t offset) {
        if (mod != currentGeodeMod) {
            currentGeodeMod = mod;
        } // if
        currentOffset = offset;
    } // setCollisionOffset
}