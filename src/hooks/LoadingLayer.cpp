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

            auto sheetName = manager->getSpritesheetQualityName();
            manager->addSpritesheetToCache(sheetName, manager->getTextureQuality());            

        } else if (m_loadStep == 10) {
            auto imagePath = manager->getCacheDirectory() + manager->getSpritesheetQualityName() + ".png";
            auto texture = CCTextureCache::sharedTextureCache()->addImage(imagePath.c_str(), false);
            // texture->setAliasTexParameters();

            auto plistPath = manager->getCacheDirectory() + manager->getSpritesheetQualityName() + ".plist";
            CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(plistPath.c_str(), texture);

            auto toolbox = ObjectToolbox::sharedState();
            manager->forEachCustomObject([this, toolbox](auto obj) {
                toolbox->m_allKeys.insert(std::pair(obj.id, obj.mainSprite.frame));
            });
        } // if
    } // loadAssets
};