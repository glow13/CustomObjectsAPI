#include <Geode/Geode.hpp>
#include <Geode/modify/LoadingLayer.hpp>

#include "CustomObjectsAPI.hpp"

using namespace geode::prelude;

class $modify(LoadingLayer) {
    void loadAssets() {
        LoadingLayer::loadAssets();

        if (m_loadStep == 1) {
            auto manager = CustomObjectsManager::get();
            manager->printModObjectCount();

            manager->addSpritesheetToCache("CustomObjects", Quality::LOW);
            manager->addSpritesheetToCache("CustomObjects-hd", Quality::MEDIUM);
            manager->addSpritesheetToCache("CustomObjects-uhd", Quality::HIGH);

            auto imagePath = manager->getCacheDirectory() + manager->getSpritesheetQualityName() + ".png";
            auto texture = CCTextureCache::sharedTextureCache()->addImage(imagePath.c_str(), false);

            auto plistPath = manager->getCacheDirectory() + manager->getSpritesheetQualityName() + ".plist";
            CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(plistPath.c_str(), texture);
        } // if
    } // loadAssets
};