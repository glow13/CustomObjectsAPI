#pragma once
#include <Geode/Geode.hpp>

#include "CustomObjectUtils.hpp"

using namespace geode::prelude;

template <class ObjectType>
class CustomCollectibleObjectBase : public CustomObjectUtils<ObjectType, EffectGameObject> {
public:
    bool init(const CustomObjectConfig<ObjectType>* config) {
        if (!this->commonSetup(config, false)) return false;
        this->commonInteractiveSetup();
        this->setDontDraw(true);

        this->m_objectType = GameObjectType::Collectible;
        this->m_unk28c = true;
        this->m_width = 25;
        this->m_height = 20;
        this->m_duration = 0;
        this->m_particleOffset = CCPoint(0, -5);

        auto main = this->addCustomChild(config->mainSprite, CCPoint(0, 0), 0);
        auto detail = this->addCustomColorChild(config->detailSprite);
        detail->setPosition(main->getPosition());
        detail->removeFromParent();
        main->addChild(detail);
        main->setTag(1);

        return true;
    } // init

protected:
    virtual void touchCustomCollectible(GJBaseGameLayer* level, PlayerObject* player) {}

private:
    void triggerObject(GJBaseGameLayer* level, int playerID, gd::vector<int> const* p2) override {
        auto player = (level->m_player2->m_uniqueID == playerID) ? level->m_player2 : level->m_player1;
        this->EffectGameObject::triggerObject(level, playerID, p2);
        this->touchCustomCollectible(level, player);
    } // triggerObject
};

class CustomCollectibleObject : public CustomCollectibleObjectBase<CustomCollectibleObject> {};