#pragma once
#include "../include/CustomObjectBase.hpp"

#include <Geode/binding/EffectGameObject.hpp>

/**
 * This class is a custom object that can be "collected" when touched by the player.
 * Some vanilla examples include the Key object from Deadlocked and the Potion
 * objects from Dash that can be collected and trigger certain events.
 * 
 * This class adds support for the `CustomObjectConfig::onActivateCustomObject` callback.
 * The callback gets called when the player touches and collects the custom collectible object.
 */
class $object(CustomCollectibleObject, EffectGameObject) {
public:
    bool init() override {
        if (!EffectGameObject::init(getConfig().getMainSprite().c_str())) return false;
        this->commonInteractiveSetup();
        this->setDontDraw(true);

        this->m_objectType = GameObjectType::Collectible;
        this->m_particleOffset = cocos2d::CCPoint(0, -5);
        this->m_unk28c = true;
        this->m_width = 25;
        this->m_height = 20;

        auto main = this->addCustomChild(getConfig().getMainSprite(), cocos2d::CCPoint(0, 0), 0);
        auto detail = this->addCustomColorChild(getConfig().getDetailSprite());
        detail->setPosition(main->getPosition());
        detail->removeFromParent();
        main->addChild(detail);
        main->setTag(1); // This enables the hovering animation

        return true;
    }

    void triggerObject(GJBaseGameLayer* level, int playerID, gd::vector<int> const* p2) override final {
        auto player = (level->m_player2->m_uniqueID == playerID) ? level->m_player2 : level->m_player1;
        this->EffectGameObject::triggerObject(level, playerID, p2);
        this->activateCustomObject(level, player);
    }
};