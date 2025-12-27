#include "CustomObjectsAPI.hpp"

#include "CustomObjectsManager.hpp"

namespace CustomObjectsAPI {
    namespace {
        geode::Mod* currentGeodeMod;

        int currentOffset;
        CustomObjectsMod* currentMod;
    }

    CustomObjectsMod* getMod() {
        if (auto mod = Mod::get(); mod != currentGeodeMod) {
            currentGeodeMod = mod;
            currentOffset = 0;
        } // if

        if (!currentMod || currentMod->getModID() != currentGeodeMod->getID()) {
            currentMod = CustomObjectsManager::get()->registerCustomObjectsMod(currentGeodeMod, currentOffset);
        } // if
        return currentMod;
    } // getMod

    void setCollisionOffset(uint8_t offset) {
        if (auto mod = Mod::get(); mod != currentGeodeMod) {
            currentGeodeMod = mod;
        } // if
        currentOffset = offset;
    } // setCollisionOffset
}