#include <Geode/Geode.hpp>
#include <Geode/modify/LoadingLayer.hpp>

#include "CustomObjectsManager.hpp"
#include "CustomSpritesManager.hpp"
#include "data/CustomObjectConfig.hpp"

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
        auto objManager = CustomObjectsManager::get();
        auto sprManager = CustomSpritesManager::get();

        if (!objManager->areAllRegisteredModsProcessed()) {
            objManager->processRegisteredMods();
            sprManager->processRegisteredSprites();
            objManager->printModObjectCount();
        } // if

        continueLoadAssets();
    } // processMods

    void checkGenerateCustomSpritesheet() {
        m_fields->m_shouldGenerateSpritesheet = false;

        auto manager = CustomSpritesManager::get();
        auto cache = manager->getCacheDirectory();
        CCFileUtils::get()->addSearchPath(cache.c_str());

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
            auto manager = CustomSpritesManager::get();
            auto sheetQuality = manager->getTextureQuality();
            auto sheetName = manager->getSpritesheetQualityName();
            manager->addSpritesheetToCache(sheetName, sheetQuality);
        } // if

        if (auto smallLabel = getChildByID("geode-small-label")) {
            auto label = static_cast<CCLabelBMFont*>(smallLabel);
            label->setString("Loading game resources");
        } // if

        continueLoadAssets();
    } // generateCustomSpritesheet

    void loadCustomSpritesheet() {
        auto manager = CustomSpritesManager::get();

        auto png = manager->getSpritesheetQualityName() + ".png";
        auto plist = manager->getSpritesheetQualityName() + ".plist";

        auto texture = CCTextureCache::get()->addImage(png.c_str(), false);
        CCSpriteFrameCache::get()->addSpriteFramesWithFile(plist.c_str());
        if (Mod::get()->getSettingValue<bool>("disable-aa")) texture->setAliasTexParameters();

        auto toolbox = ObjectToolbox::sharedState();
        auto objectManager = CustomObjectsManager::get();
        objectManager->forEachCustomObject([this, toolbox](auto obj) {
            toolbox->m_allKeys.insert(std::pair(obj->getObjectID(), obj->getMainSprite()));
        });

        continueLoadAssets();
    } // loadCustomSpritesheet
};