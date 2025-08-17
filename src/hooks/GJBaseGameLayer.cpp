#include <Geode/Geode.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>

#include "CustomObjectsAPI.hpp"

using namespace geode::prelude;

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
        log::info("GJBaseGameLayer::setupLayers");
        GJBaseGameLayer::setupLayers();

        auto manager = CustomObjectsManager::get();
        auto path = manager->getCacheDirectory() + manager->getSpritesheetQualityName() + ".png";
        auto spr = CCTextureCache::sharedTextureCache()->addImage(path.c_str(), false);

        auto fields = m_fields.self();

        fields->m_customLayerT4 = CCSpriteBatchNode::createWithTexture(spr);
        fields->m_customLayerT4->setZOrder(m_gameLayerT4->getZOrder());
        fields->m_customLayerT4->setUseChildIndex(true);
        m_objectLayer->addChild(fields->m_customLayerT4);

        fields->m_customLayerT3 = CCSpriteBatchNode::createWithTexture(spr);
        fields->m_customLayerT3->setZOrder(m_gameLayerT3->getZOrder());
        fields->m_customLayerT3->setUseChildIndex(true);
        m_objectLayer->addChild(fields->m_customLayerT3);

        fields->m_customLayerT2 = CCSpriteBatchNode::createWithTexture(spr);
        fields->m_customLayerT2->setZOrder(m_gameLayerT2->getZOrder());
        fields->m_customLayerT2->setUseChildIndex(true);
        m_objectLayer->addChild(fields->m_customLayerT2);

        fields->m_customLayerT1 = CCSpriteBatchNode::createWithTexture(spr);
        fields->m_customLayerT1->setZOrder(m_gameLayerT1->getZOrder());
        fields->m_customLayerT1->setUseChildIndex(true);
        m_objectLayer->addChild(fields->m_customLayerT1);

        fields->m_customLayerB1 = CCSpriteBatchNode::createWithTexture(spr);
        fields->m_customLayerB1->setZOrder(m_gameLayerB1->getZOrder());
        fields->m_customLayerB1->setUseChildIndex(true);
        m_objectLayer->addChild(fields->m_customLayerB1);

        fields->m_customLayerB2 = CCSpriteBatchNode::createWithTexture(spr);
        fields->m_customLayerB2->setZOrder(m_gameLayerB2->getZOrder());
        fields->m_customLayerB2->setUseChildIndex(true);
        m_objectLayer->addChild(fields->m_customLayerB2);

        fields->m_customLayerB3 = CCSpriteBatchNode::createWithTexture(spr);
        fields->m_customLayerB3->setZOrder(m_gameLayerB3->getZOrder());
        fields->m_customLayerB3->setUseChildIndex(true);
        m_objectLayer->addChild(fields->m_customLayerB3);

        fields->m_customLayerB4 = CCSpriteBatchNode::createWithTexture(spr);
        fields->m_customLayerB4->setZOrder(m_gameLayerB4->getZOrder());
        fields->m_customLayerB4->setUseChildIndex(true);
        m_objectLayer->addChild(fields->m_customLayerB4);

        fields->m_customLayerB5 = CCSpriteBatchNode::createWithTexture(spr);
        fields->m_customLayerB5->setZOrder(m_gameLayerB5->getZOrder());
        fields->m_customLayerB5->setUseChildIndex(true);
        m_objectLayer->addChild(fields->m_customLayerB5);
    } // setupLayers

    CCNode* parentForZLayer(int zLayer, bool blending, int parentMode, int uiObject) {
        if (parentMode == 10) {
            switch(static_cast<ZLayer>(zLayer)) {
                case ZLayer::T4: return m_fields->m_customLayerT4;
                case ZLayer::T3: return m_fields->m_customLayerT3;
                case ZLayer::T2: return m_fields->m_customLayerT2;
                case ZLayer::T1: return m_fields->m_customLayerT1;
                case ZLayer::B1: return m_fields->m_customLayerB1;
                case ZLayer::B2: return m_fields->m_customLayerB2;
                case ZLayer::B3: return m_fields->m_customLayerB3;
                case ZLayer::B4: return m_fields->m_customLayerB4;
                case ZLayer::B5: return m_fields->m_customLayerB5;
                default: return nullptr;
            } // switch
        } // if
        return GJBaseGameLayer::parentForZLayer(zLayer, blending, parentMode, uiObject);
    } // parentForZLayer
};