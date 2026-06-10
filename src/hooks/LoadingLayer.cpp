#include <Geode/Geode.hpp>
#include <Geode/modify/LoadingLayer.hpp>

#include "CustomObjectsManager.hpp"
#include "CustomSpritesManager.hpp"

using namespace geode::prelude;

class $modify(LoadingLayer) {
    struct Fields {
        int m_customLoadStep;
        bool m_shouldGenerateSpritesheet;
        bool m_spritesheetGenerationFailed;
    };

    void continueLoadAssets() {
        m_fields->m_customLoadStep++;
        Loader::get()->queueInMainThread([this]() { loadAssets(); });
    } // continueLoadAssets

    void loadAssets() {
        if (m_loadStep == 14) {
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

        int objectCount = CustomObjectsManager::get()->getTotalCustomObjectsCount();
        if (Mod::get()->getSettingValue<bool>("force-generation") && objectCount > 0) {
            log::info("Forced spritesheet generation is enabled!");
        } else if (manager->isTheSpritesheetCacheUpToDate()) {
            log::info("Cache is up-to-date, skipping spritesheet generation");
            continueLoadAssets();
            return;
        } // if

        log::info("Cache is outdated, generating the custom objects spritesheets...");
        if (auto smallLabel = getChildByID("geode-small-label")) {
            auto label = static_cast<CCLabelBMFont*>(smallLabel);
            label->setString("Generating custom spritesheet");
        } // if

        m_fields->m_shouldGenerateSpritesheet = true;
        continueLoadAssets();
    } // checkGenerateCustomSpritesheet

    void generateCustomSpritesheet() {
        if (!m_fields->m_shouldGenerateSpritesheet) {
            continueLoadAssets();
            return;
        } // if

        auto manager = CustomSpritesManager::get();
        m_fields->m_spritesheetGenerationFailed = true;

        switch (manager->getTextureQuality()) {
            case Quality::HIGH:
                if (!manager->addSpritesheetToCache("CustomObjects-uhd", Quality::HIGH)) break;
                [[fallthrough]];
            case Quality::MEDIUM:
                if (!manager->addSpritesheetToCache("CustomObjects-hd", Quality::MEDIUM)) break;
                [[fallthrough]];
            case Quality::LOW:
                if (!manager->addSpritesheetToCache("CustomObjects", Quality::LOW)) break;
                [[fallthrough]];
            default: m_fields->m_spritesheetGenerationFailed = false;
        } // switch

        if (auto smallLabel = getChildByID("geode-small-label")) {
            auto label = static_cast<CCLabelBMFont*>(smallLabel);
            label->setString("Loading game resources");
        } // if

        continueLoadAssets();
    } // generateCustomSpritesheet

    void loadCustomSpritesheet() {
        if (m_fields->m_spritesheetGenerationFailed) {
            geode::Notification::create(
                "Failed to generate custom spritesheet! Check logs for more info.",
                geode::NotificationIcon::Error
            )->show();

            continueLoadAssets();
            return;
        } // if

        if (CustomObjectsManager::get()->getTotalCustomObjectsCount() > 0) {
            auto png = CustomSpritesManager::getSpritesheetQualityName() + ".png";
            auto plist = CustomSpritesManager::getSpritesheetQualityName() + ".plist";

            auto texture = CCTextureCache::get()->addImage(png.c_str(), false);
            CCSpriteFrameCache::get()->addSpriteFramesWithFile(plist.c_str());
            if (Mod::get()->getSettingValue<bool>("disable-aa")) texture->setAliasTexParameters();
        } // if

        continueLoadAssets();
    } // loadCustomSpritesheet
};