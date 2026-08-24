#pragma once
#include "../include/CustomObjectBase.hpp"

#include <Geode/binding/RingObject.hpp>
#include <Geode/binding/GJSpriteColor.hpp>

/**
 * This class is a custom object that can interact with the player's icon when the player clicks on this object.
 * Some vanilla examples include the pink, yellow, and red jump orbs, as well as the Toggle Orb.
 * 
 * This class adds support for the `CustomObjectConfig::onActivateCustomObject` callback.
 * The callback is called whenever the player clicks when their icon is touching this object.
 */
class $object(CustomRingObject, RingObject) {
public:
    bool init() override {
        if (!CustomObjectBase::init()) return false;

        this->m_objectType = GameObjectType::CustomRing;
        this->m_baseColor->m_defaultColorID = 0;
        this->m_usesAudioScale = true;
        this->m_greenDebugDraw = true;
        this->m_customGlowColor = false;
        this->m_width = 36;
        this->m_height = 36;

        return true;
    }

    // We can do mostly normal activation logic for the custom ring / orb object
    void activatedByPlayer(PlayerObject* player) override final {
        player->m_lastActivatedPortal = this;
        player->m_lastPortalPos = this->getPosition();
        CustomObjectBase::activatedByPlayer(player);
        this->activateCustomObject(player->m_gameLayer, player);
    }

    void customSetup() override {
        if (!this->m_editorEnabled && !this->m_hasNoParticles) {
            this->createAndAddParticle(36, "ringEffect.plist", 4, cocos2d::tCCPositionType::kCCPositionTypeGrouped);
        }
        CustomObjectBase::customSetup();
        this->m_dontIgnoreDuration = false;
    }

    bool canAllowMultiActivate() override {
        return true;
    }
};