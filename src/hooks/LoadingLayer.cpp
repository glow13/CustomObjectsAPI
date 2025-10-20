#include <Geode/Geode.hpp>
#include <Geode/modify/LoadingLayer.hpp>

#include "CustomObjectsManager.hpp"

using namespace geode::prelude;

class $modify(LoadingLayer) {
    struct Fields {
        int m_customLoadStep;
        bool m_shouldGenerateSpritesheet;
    };

    void continueLoadAssets() {
        m_fields->m_customLoadStep++;
        Loader::get()->queueInMainThread([this]() { this->loadAssets(); });
    } // continueLoadAssets

    void loadAssets() {
        if (m_loadStep == 10) {
            switch (m_fields->m_customLoadStep) {
                case 0: processMods(); break;
                case 1: checkGenerateCustomSpritesheet(); break;
                case 2: generateCustomSpritesheet(); break;
                case 3: loadCustomSpritesheet(); break;
                default: LoadingLayer::loadAssets(); break;
            } // switch
        } else LoadingLayer::loadAssets();
    } // loadAssets

    void processMods() {
        auto manager = CustomObjectsManager::get();
        manager->processRegisteredMods();
        manager->printModObjectCount();
        continueLoadAssets();
    } // processMods

    void checkGenerateCustomSpritesheet() {
        m_fields->m_shouldGenerateSpritesheet = false;

        auto manager = CustomObjectsManager::get();
        if (Mod::get()->getSettingValue<bool>("force-generation")) {
            log::info("Forced spritesheet generation is enabled!");
        } else if (manager->isTheSpritesheetCacheUpToDate()) {
            log::info("Cache is up-to-date, skipping spritesheet generation");
            continueLoadAssets();
            return;
        } // if

        log::info("Cache is outdated, generating the custom objects spritesheet...");
        if (auto smallLabel = getChildByID("geode-small-label")) {
            auto label = static_cast<CCLabelBMFont*>(smallLabel);
            label->setString("Generating custom spritesheet");
        } // if

        m_fields->m_shouldGenerateSpritesheet = true;
        continueLoadAssets();
    } // checkGenerateCustomSpritesheet

    void generateCustomSpritesheet() {
        if (m_fields->m_shouldGenerateSpritesheet) {
            auto manager = CustomObjectsManager::get();
            auto sheetQuality = manager->getTextureQuality();
            auto sheetName = manager->getSpritesheetQualityName();
            manager->addSpritesheetToCache(sheetName, sheetQuality);
        } // if
        continueLoadAssets();
    } // generateCustomSpritesheet

    void loadCustomSpritesheet() {
        auto manager = CustomObjectsManager::get();
        auto imagePath = manager->getCacheDirectory() + manager->getSpritesheetQualityName() + ".png";
        auto texture = CCTextureCache::sharedTextureCache()->addImage(imagePath.c_str(), false);
        // texture->setAliasTexParameters();

        auto plistPath = manager->getCacheDirectory() + manager->getSpritesheetQualityName() + ".plist";
        CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(plistPath.c_str(), texture);

        auto toolbox = ObjectToolbox::sharedState();
        manager->forEachCustomObject([this, toolbox](auto obj) {
            toolbox->m_allKeys.insert(std::pair(obj.id, obj.mainSprite.frame));
        });
        continueLoadAssets();
    } // loadCustomSpritesheet
};