#include <Geode/Geode.hpp>
#include <Geode/modify/LoadingLayer.hpp>

#include "CustomObjectsAPI.hpp"

using namespace geode::prelude;

class $modify(LoadingLayer) {
    void loadAssets() {
        LoadingLayer::loadAssets();
        auto manager = CustomObjectsManager::get();
        
        if (m_loadStep == 9) {
            manager->processRegisteredMods();
            manager->printModObjectCount();

            if (Mod::get()->getSettingValue<bool>("force-generation")) {
                log::info("Forced spritesheet generation is enabled!");
            } else if (manager->isTheSpritesheetCacheUpToDate()) {
                log::info("Cache is up-to-date, skipping spritesheet generation!");
                return;
            } // if

            log::info("Cache is outdated, generating the custom object spritesheets...");

            manager->addSpritesheetToCache("CustomObjects", Quality::LOW);
            manager->addSpritesheetToCache("CustomObjects-hd", Quality::MEDIUM);
            manager->addSpritesheetToCache("CustomObjects-uhd", Quality::HIGH);

            std::vector<std::string> objs;
            manager->forEachCustomObject([&objs](auto obj) { objs.emplace_back(obj.frame); });
            Mod::get()->setSavedValue<std::vector<std::string>>("custom-objects", objs);

        } else if (m_loadStep == 10) {
            auto imagePath = manager->getCacheDirectory() + manager->getSpritesheetQualityName() + ".png";
            auto texture = CCTextureCache::sharedTextureCache()->addImage(imagePath.c_str(), false);

            auto plistPath = manager->getCacheDirectory() + manager->getSpritesheetQualityName() + ".plist";
            CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(plistPath.c_str(), texture);
        } // if
    } // loadAssets
};