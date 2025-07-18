#pragma once
#include <Geode/Geode.hpp>
#include <Geode/modify/LoadingLayer.hpp>
#include <Geode/modify/ObjectToolbox.hpp>
#include <Geode/modify/GameObject.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>

#include "CustomObjectsAPI.hpp"

using namespace geode::prelude;

/*
    For this mod to work you need to make these functions return pointers!!!!
    GameObject::getObjectRect2
    GameObject::getObjectTextureRect
*/

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

class $modify(ObjectToolbox) {
public:
    bool init() {
        if (!ObjectToolbox::init()) return false;

        auto manager = CustomObjectsManager::get();
        for (int i = 0; i < manager->getObjectCount(); i++) {
            auto obj = manager->getCustomObject(i);
            gd::string frame = fmt::format("custom-objects/{}", obj->m_spr);
            m_allKeys.insert(std::pair(obj->m_id, frame));
        } // for

        return true;
    } // init
};

class $modify(GameObject) {
public:
    static GameObject* createWithKey(int key) {
        if (key >= 4600) {
            auto manager = CustomObjectsManager::get();
            if (auto obj = manager->getCustomObjectByID(key)) return obj->create();
        } // if
        return GameObject::createWithKey(key);
    } // createWithKey
};

class $modify(GJBaseGameLayer) {
public:
    struct Fields {
        CCSpriteBatchNode* m_customLayerT4;
        CCSpriteBatchNode* m_customLayerT3;
        CCSpriteBatchNode* m_customLayerT2;
        CCSpriteBatchNode* m_customLayerT1;
        CCSpriteBatchNode* m_customLayerB1;
        CCSpriteBatchNode* m_customLayerB2;
        CCSpriteBatchNode* m_customLayerB3;
        CCSpriteBatchNode* m_customLayerB4;
        CCSpriteBatchNode* m_customLayerB5;
    };

    void setupLayers() {
        GJBaseGameLayer::setupLayers();

        auto manager = CustomObjectsManager::get();
        auto path = manager->getCacheDirectory() + manager->getSpritesheetQualityName() + ".png";
        auto spr = CCTextureCache::sharedTextureCache()->addImage(path.c_str(), false);

        m_fields->m_customLayerT4 = CCSpriteBatchNode::createWithTexture(spr);
        m_fields->m_customLayerT4->setZOrder(m_gameLayerT4->getZOrder());
        m_fields->m_customLayerT4->setUseChildIndex(true);
        m_objectLayer->addChild(m_fields->m_customLayerT4);

        m_fields->m_customLayerT3 = CCSpriteBatchNode::createWithTexture(spr);
        m_fields->m_customLayerT3->setZOrder(m_gameLayerT3->getZOrder());
        m_fields->m_customLayerT3->setUseChildIndex(true);
        m_objectLayer->addChild(m_fields->m_customLayerT3);

        m_fields->m_customLayerT2 = CCSpriteBatchNode::createWithTexture(spr);
        m_fields->m_customLayerT2->setZOrder(m_gameLayerT2->getZOrder());
        m_fields->m_customLayerT2->setUseChildIndex(true);
        m_objectLayer->addChild(m_fields->m_customLayerT2);

        m_fields->m_customLayerT1 = CCSpriteBatchNode::createWithTexture(spr);
        m_fields->m_customLayerT1->setZOrder(m_gameLayerT1->getZOrder());
        m_fields->m_customLayerT1->setUseChildIndex(true);
        m_objectLayer->addChild(m_fields->m_customLayerT1);

        m_fields->m_customLayerB1 = CCSpriteBatchNode::createWithTexture(spr);
        m_fields->m_customLayerB1->setZOrder(m_gameLayerB1->getZOrder());
        m_fields->m_customLayerB1->setUseChildIndex(true);
        m_objectLayer->addChild(m_fields->m_customLayerB1);

        m_fields->m_customLayerB2 = CCSpriteBatchNode::createWithTexture(spr);
        m_fields->m_customLayerB2->setZOrder(m_gameLayerB2->getZOrder());
        m_fields->m_customLayerB2->setUseChildIndex(true);
        m_objectLayer->addChild(m_fields->m_customLayerB2);

        m_fields->m_customLayerB3 = CCSpriteBatchNode::createWithTexture(spr);
        m_fields->m_customLayerB3->setZOrder(m_gameLayerB3->getZOrder());
        m_fields->m_customLayerB3->setUseChildIndex(true);
        m_objectLayer->addChild(m_fields->m_customLayerB3);

        m_fields->m_customLayerB4 = CCSpriteBatchNode::createWithTexture(spr);
        m_fields->m_customLayerB4->setZOrder(m_gameLayerB4->getZOrder());
        m_fields->m_customLayerB4->setUseChildIndex(true);
        m_objectLayer->addChild(m_fields->m_customLayerB4);

        m_fields->m_customLayerB5 = CCSpriteBatchNode::createWithTexture(spr);
        m_fields->m_customLayerB5->setZOrder(m_gameLayerB5->getZOrder());
        m_fields->m_customLayerB5->setUseChildIndex(true);
        m_objectLayer->addChild(m_fields->m_customLayerB5);
    } // setupLayers

    CCNode* parentForZLayer(int zLayer, bool blending, int parentMode, int uiObject) {
        if (parentMode == 10) {
            switch(static_cast<ZLayer>(zLayer)) {
                case ZLayer::T4:
                    return m_fields->m_customLayerT4;
                case ZLayer::T3:
                    return m_fields->m_customLayerT3;
                case ZLayer::T2:
                    return m_fields->m_customLayerT2;
                case ZLayer::T1:
                    return m_fields->m_customLayerT1;
                case ZLayer::B1:
                    return m_fields->m_customLayerB1;
                case ZLayer::B2:
                    return m_fields->m_customLayerB2;
                case ZLayer::B3:
                    return m_fields->m_customLayerB3;
                case ZLayer::B4:
                    return m_fields->m_customLayerB4;
                case ZLayer::B5:
                    return m_fields->m_customLayerB5;
            } // switch
        } // if
    return GJBaseGameLayer::parentForZLayer(zLayer, blending, parentMode, uiObject);
    } // parentForZLayer
};