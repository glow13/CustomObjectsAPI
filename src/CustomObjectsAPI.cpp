#include <Geode/Geode.hpp>
#include "CustomObjectsAPI.hpp"

#include "config/CustomSpriteConfig.hpp"
#include "manager/CustomObjectsManager.hpp"

using namespace geode::prelude;

CustomObjectConfig&& CustomObjectsUtils::registerCustomObject(std::string objectID, CustomObjectConfig::ObjectConstructor ctor, Mod* mod) {
    if (objectID.substr(0, objectID.find("/")) != mod->getID() || objectID.size() == mod->getID().size() + 1) {
        log::logImpl(Severity::Error, mod, "Invalid object ID! Must use a non-empty string followed by \"\"_spr operator!");
        return std::move(*(new CustomObjectConfig(objectID, 0, std::move(ctor)))); // Dummy unregistered object config
    }
    return std::move(*CustomObjectsManager::get()->registerObjectConfig(objectID, std::move(ctor)));
}

void CustomObjectsUtils::registerCustomSprite(std::string frameName, int offsetX, int offsetY, int width, int height, geode::Mod* mod) {
    CustomObjectsManager::get()->registerCustomSprite(new CustomSpriteConfig(mod, frameName, offsetX, offsetY, width, height, true));
}

void CustomObjectsUtils::registerCustomAnimationSprites(std::string firstFrame, int offsetX, int offsetY, int width, int height, int frames, geode::Mod* mod) {
    auto baseFrameName = firstFrame.substr(0, firstFrame.find("_001"));
    for (int i = 1; i <= frames; i++) {
        auto animFrameName = fmt::format("{}_{:03d}.png", baseFrameName, i);
        registerCustomSprite(animFrameName, offsetX, offsetY, width, height, mod);
    }
}

$execute {
    if (auto editorTabMod = Loader::get()->getInstalledMod("alphalaneous.editortab_api")) {
        Mod::get()->setSavedValue("editortab-api-enabled", editorTabMod->isOrWillBeEnabled());
    } else Mod::get()->setSavedValue("editortab-api-enabled", false);
}