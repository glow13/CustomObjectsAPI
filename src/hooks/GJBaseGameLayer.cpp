#include <Geode/Geode.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>

#include "CustomObjectsAPI.hpp"

using namespace geode::prelude;

class $modify(GJBaseGameLayer) {
public:
    struct Fields {
        CCSpriteBatchNode* m_customLayerT4;
        CCSpriteBatchNode* m_customBlendingLayerT4;
        CCSpriteBatchNode* m_customLayerT3;
        CCSpriteBatchNode* m_customBlendingLayerT3;
        CCSpriteBatchNode* m_customLayerT2;
        CCSpriteBatchNode* m_customBlendingLayerT2;
        CCSpriteBatchNode* m_customLayerT1;
        CCSpriteBatchNode* m_customBlendingLayerT1;
        CCSpriteBatchNode* m_customLayerB1;
        CCSpriteBatchNode* m_customBlendingLayerB1;
        CCSpriteBatchNode* m_customLayerB2;
        CCSpriteBatchNode* m_customBlendingLayerB2;
        CCSpriteBatchNode* m_customLayerB3;
        CCSpriteBatchNode* m_customBlendingLayerB3;
        CCSpriteBatchNode* m_customLayerB4;
        CCSpriteBatchNode* m_customBlendingLayerB4;
        CCSpriteBatchNode* m_customLayerB5;
        CCSpriteBatchNode* m_customBlendingLayerB5;
    };

    CCSpriteBatchNode* createAndAddBatchLayer(CCTexture2D* spr, CCSpriteBatchNode* copy, bool blend) {
        auto batch = CCSpriteBatchNode::createWithTexture(spr);
        m_objectLayer->addChild(batch);

        batch->setUseChildIndex(true);
        batch->setZOrder(copy->getZOrder());
        if (blend) batch->setBlendFunc({ GL_SRC_ALPHA, GL_ONE });

        return batch;
    } // createAndAddBatchLayer

    void setupLayers() {
        GJBaseGameLayer::setupLayers();

        auto manager = CustomObjectsManager::get();
        auto path = manager->getCacheDirectory() + manager->getSpritesheetQualityName() + ".png";
        auto spr = CCTextureCache::sharedTextureCache()->addImage(path.c_str(), false);

        auto fields = m_fields.self();

        fields->m_customLayerT4 = createAndAddBatchLayer(spr, m_gameLayerT4, false);
        fields->m_customBlendingLayerT4 = createAndAddBatchLayer(spr, m_gameBlendingLayerT4, true);

        fields->m_customLayerT3 = createAndAddBatchLayer(spr, m_gameLayerT3, false);
        fields->m_customBlendingLayerT3 = createAndAddBatchLayer(spr, m_gameBlendingLayerT3, true);

        fields->m_customLayerT2 = createAndAddBatchLayer(spr, m_gameLayerT2, false);
        fields->m_customBlendingLayerT2 = createAndAddBatchLayer(spr, m_gameBlendingLayerT2, true);

        fields->m_customLayerT1 = createAndAddBatchLayer(spr, m_gameLayerT1, false);
        fields->m_customBlendingLayerT1 = createAndAddBatchLayer(spr, m_gameBlendingLayerT1, true);

        fields->m_customLayerB1 = createAndAddBatchLayer(spr, m_gameLayerB1, false);
        fields->m_customBlendingLayerB1 = createAndAddBatchLayer(spr, m_gameBlendingLayerB1, true);

        fields->m_customLayerB2 = createAndAddBatchLayer(spr, m_gameLayerB2, false);
        fields->m_customBlendingLayerB2 = createAndAddBatchLayer(spr, m_gameBlendingLayerB2, true);

        fields->m_customLayerB3 = createAndAddBatchLayer(spr, m_gameLayerB3, false);
        fields->m_customBlendingLayerB3 = createAndAddBatchLayer(spr, m_gameBlendingLayerB3, true);

        fields->m_customLayerB4 = createAndAddBatchLayer(spr, m_gameLayerB4, false);
        fields->m_customBlendingLayerB4 = createAndAddBatchLayer(spr, m_gameBlendingLayerB4, true);

        fields->m_customLayerB5 = createAndAddBatchLayer(spr, m_gameLayerB5, false);
        fields->m_customBlendingLayerB5 = createAndAddBatchLayer(spr, m_gameBlendingLayerB5, true);
    } // setupLayers

    CCNode* parentForZLayer(int zLayer, bool blend, int parent, int ui) {
        if (parent != 10) return GJBaseGameLayer::parentForZLayer(zLayer, (blend && parent != 4), parent, ui);

        if (!blend) switch(static_cast<ZLayer>(zLayer)) {
            case ZLayer::T4: return m_fields->m_customLayerT4;
            case ZLayer::T3: return m_fields->m_customLayerT3;
            case ZLayer::T2: return m_fields->m_customLayerT2;
            case ZLayer::T1: return m_fields->m_customLayerT1;
            case ZLayer::B1: return m_fields->m_customLayerB1;
            case ZLayer::B2: return m_fields->m_customLayerB2;
            case ZLayer::B3: return m_fields->m_customLayerB3;
            case ZLayer::B4: return m_fields->m_customLayerB4;
            case ZLayer::B5: return m_fields->m_customLayerB5;
            default: return m_fields->m_customLayerB1;
        } // switch

        switch(static_cast<ZLayer>(zLayer)) {
            case ZLayer::T4: return m_fields->m_customBlendingLayerT4;
            case ZLayer::T3: return m_fields->m_customBlendingLayerT3;
            case ZLayer::T2: return m_fields->m_customBlendingLayerT2;
            case ZLayer::T1: return m_fields->m_customBlendingLayerT1;
            case ZLayer::B1: return m_fields->m_customBlendingLayerB1;
            case ZLayer::B2: return m_fields->m_customBlendingLayerB2;
            case ZLayer::B3: return m_fields->m_customBlendingLayerB3;
            case ZLayer::B4: return m_fields->m_customBlendingLayerB4;
            case ZLayer::B5: return m_fields->m_customBlendingLayerB5;
            default: return m_fields->m_customBlendingLayerB1;
        } // switch
    } // parentForZLayer
};