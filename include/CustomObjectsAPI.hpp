#pragma once
#include <Geode/Geode.hpp>
#include <Geode/modify/GameObject.hpp>
#include <Geode/modify/ObjectToolbox.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>

#include "CustomGameObject.hpp"
#include "CustomEffectGameObject.hpp"
#include "ContainerGameObject.hpp"

using namespace geode::prelude;

class $modify(GameObjectCO, GameObject) {
    static GameObject* createWithKey(int key);
};

class $modify(CustomObjectToolbox, ObjectToolbox) {
    bool init();
};

class $modify(GJBaseGameLayerCO, GJBaseGameLayer) {
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

    void setupLayers();
    CCNode* parentForZLayer(int zLayer, bool blending, int parentMode, int uiObject);
};