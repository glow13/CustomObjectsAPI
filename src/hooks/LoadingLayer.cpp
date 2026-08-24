#include <Geode/Geode.hpp>
#include <Geode/modify/LoadingLayer.hpp>
#include "../manager/CustomObjectsManager.hpp"
#include "../manager/CustomObjectsSheet.hpp"

using namespace geode::prelude;

class $modify(LoadingLayer) {
    struct Fields {
        int m_customLoadStep;
        bool m_shouldGenerateSpritesheet;
    };

    void continueLoadAssets() {
        m_fields->m_customLoadStep++;
        Loader::get()->queueInMainThread([this]() { this->loadAssets(); });
    }

    // This basically just copies how geode loads its resources
    void loadAssets() {
        if (m_loadStep == 10) {
            switch (m_fields->m_customLoadStep) {
                case 0: processMods(); break;
                case 1: checkGenerateCustomSpritesheet(); break;
                case 2: generateCustomSpritesheet(); break;
                case 3: loadCustomSpritesheet(); break;
                default: LoadingLayer::loadAssets(); break;
            }
        } else LoadingLayer::loadAssets();
    }

    void processMods() {
        CustomObjectsManager::get()->processRegisteredObjects();
        CustomObjectsManager::get()->printModObjectCount();
        continueLoadAssets();
    }

    void checkGenerateCustomSpritesheet() {
        m_fields->m_shouldGenerateSpritesheet = false;
        int objectCount = CustomObjectsManager::get()->getCustomObjectsCount();

        if (Mod::get()->getSettingValue<bool>("force-generation") && objectCount > 0) {
            log::info("Forced spritesheet generation is enabled!");
        } else if (CustomObjectsManager::get()->isTheSpritesheetCacheUpToDate()) {
            log::info("Cache is up-to-date, skipping spritesheet generation");
            continueLoadAssets();
            return;
        }

        log::info("Cache is outdated, generating the custom objects spritesheets...");
        if (auto smallLabel = getChildByID("geode-small-label")) {
            auto label = static_cast<CCLabelBMFont*>(smallLabel);
            label->setString("Generating custom spritesheet");
        }

        m_fields->m_shouldGenerateSpritesheet = true;
        continueLoadAssets();
    }

    void generateCustomSpritesheet() {
        if (!m_fields->m_shouldGenerateSpritesheet) {
            continueLoadAssets();
            return;
        }

        CustomObjectsManager::get()->generateCustomSpritesheets();

        if (auto smallLabel = getChildByID("geode-small-label")) {
            auto label = static_cast<CCLabelBMFont*>(smallLabel);
            label->setString("Loading game resources");
        }

        continueLoadAssets();
    }

    void loadCustomSpritesheet() {
        if (CustomObjectsManager::get()->getCustomObjectsCount() <= 0) {
            continueLoadAssets();
            return;
        }

        auto cache = CustomObjectsSheet::getCacheDirectory();
        auto png = CustomObjectsSheet::getSpritesheetQualityName() + ".png";
        auto plist = CustomObjectsSheet::getSpritesheetQualityName() + ".plist";

        CCFileUtils::get()->addSearchPath(cache.c_str());
        auto texture = CCTextureCache::get()->addImage(png.c_str(), false);
        CCSpriteFrameCache::get()->addSpriteFramesWithFile(plist.c_str(), texture);
        if (Mod::get()->getSettingValue<bool>("disable-aa")) texture->setAliasTexParameters();

        continueLoadAssets();
    }
};