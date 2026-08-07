#include <Geode/Geode.hpp>
#include <Geode/modify/LoadingLayer.hpp>
#include "../manager/CustomObjectsManager.hpp"

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
        auto manager = CustomObjectsManager::get();
        manager->processRegisteredObjects();
        manager->printModObjectCount();
        continueLoadAssets();
    }

    void checkGenerateCustomSpritesheet() {
        bool upToDate = CustomObjectsManager::get()->isTheSpritesheetCacheUpToDate();
        if (auto smallLabel = getChildByID("geode-small-label"); !upToDate && smallLabel)
            static_cast<CCLabelBMFont*>(smallLabel)->setString("Generating custom spritesheet");
        m_fields->m_shouldGenerateSpritesheet = !upToDate;
        continueLoadAssets();
    }

    void generateCustomSpritesheet() {
        if (!m_fields->m_shouldGenerateSpritesheet) {
            continueLoadAssets();
            return;
        }

        CustomObjectsManager::get()->generateCustomSpritesheets();
        if (auto smallLabel = getChildByID("geode-small-label"))
            static_cast<CCLabelBMFont*>(smallLabel)->setString("Loading game resources");
        continueLoadAssets();
    }

    void loadCustomSpritesheet() {
        log::info("{}", CustomObjectsManager::get()->getCustomSpritesheet());
        continueLoadAssets();
    }
};