#include "CustomObjectsAPI.hpp"

/*
    For this mod to work you need to make these functions return pointers!!!!
    GameObject::getObjectRect2
    GameObject::getObjectTextureRect
*/

GameObject* GameObjectCO::createWithKey(int key) {
    switch (key) {
        case 5001:
            return CustomGameObject::create(key);
        case 5002:
            return CustomEffectGameObject::create(key);
        case 5003:
            return ContainerGameObject::create(key);
    } // switch
    return GameObject::createWithKey(key);
} // createWithKey

bool CustomObjectToolbox::init() {
    if (!ObjectToolbox::init()) return false;

    m_allKeys.insert(std::pair(5001, "frown-block.png"_spr));
    m_allKeys.insert(std::pair(5002, "smile-block.png"_spr));
    m_allKeys.insert(std::pair(5003, "container.png"_spr));

    return true;
} // init

void GJBaseGameLayerCO::setupLayers() {
    GJBaseGameLayer::setupLayers();

    // @geode-ignore(unknown-resource)
    auto spr = CCSprite::create("CustomObjects.png"_spr)->getTexture();

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

CCNode* GJBaseGameLayerCO::parentForZLayer(int zLayer, bool blending, int parentMode, int uiObject) {
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