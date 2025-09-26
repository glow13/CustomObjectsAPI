#pragma once
#include <Geode/Geode.hpp>

#include "CustomObjectUtils.hpp"

using namespace geode::prelude;

template <class ObjectType>
class CustomCollectibleObjectBase : public CustomObjectUtils<ObjectType, EffectGameObject> {
public:
    bool init(const CustomObjectConfig& config) {
        if (!this->commonSetup(config, false)) return false;
        this->commonInteractiveSetup();
        this->setDontDraw(true);

        this->m_objectType = GameObjectType::Collectible;
        this->m_unk28c = true;
        this->m_width = 25;
        this->m_height = 20;
        this->m_duration = 0;
        this->m_unk31c = CCPoint(0, -5); // particle offset

        auto main = this->addCustomChild(config.frame, CCPoint(0, 0), 0);
        auto detail = this->addCustomColorChild(config.detailFrame);
        detail->setPosition(main->getPosition());
        detail->removeFromParent();
        main->addChild(detail);
        main->setTag(1);

        return true;
    } // init

protected:
    virtual void touchCustomCollectible(PlayerObject* player) {}

private:
    void triggerObject(GJBaseGameLayer* level, int playerID, gd::vector<int> const* p2) override {
        auto player = (level->m_player2->m_uniqueID == playerID) ? level->m_player2 : level->m_player1;
        this->EffectGameObject::triggerObject(level, playerID, p2);
        this->touchCustomCollectible(player);
    } // triggerObject
};

class CustomCollectibleObject : public CustomCollectibleObjectBase<CustomCollectibleObject> {};