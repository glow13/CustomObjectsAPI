#pragma once

#include "CustomObjectBase.hpp"

template <class ObjectType>
class $base(CustomCollectibleObject, EffectGameObject) {
protected:
    using CustomObjectBase = CustomObjectBase<ObjectType, EffectGameObject>::Base;
public:
    bool init(const CustomObjectConfig<ObjectType>* config) {
        if (!EffectGameObject::init(config->getMainSprite().c_str())) return false;
        this->commonInteractiveSetup();
        this->setDontDraw(true);

        this->m_objectType = GameObjectType::Collectible;
        this->m_particleOffset = CCPoint(0, -5);
        this->m_unk28c = true;
        this->m_width = 25;
        this->m_height = 20;

        auto main = this->addCustomChild(config->getMainSprite(), CCPoint(0, 0), 0);
        auto detail = this->addCustomColorChild(config->getDetailSprite());
        detail->setPosition(main->getPosition());
        detail->removeFromParent();
        main->addChild(detail);
        main->setTag(1);

        return true;
    } // init

private:
    void triggerObject(GJBaseGameLayer* level, int playerID, gd::vector<int> const* p2) override {
        auto player = (level->m_player2->m_uniqueID == playerID) ? level->m_player2 : level->m_player1;
        this->EffectGameObject::triggerObject(level, playerID, p2);
        this->activateCustomObject(level, player);
    } // triggerObject
};

class $generic(CustomCollectibleObject);