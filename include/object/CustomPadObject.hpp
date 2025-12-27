#pragma once

#include "CustomObjectBase.hpp"

template <class ObjectType>
class $base(CustomPadObject, EffectGameObject) {
protected:
    using CustomObjectBase = CustomObjectBase<ObjectType, EffectGameObject>::Base;
    using CustomPadObject = CustomPadObjectBase<ObjectType>;
public:
    bool init(const CustomObjectConfig<ObjectType>* config) {
        if (!CustomObjectBase::init(config)) return false;

        this->m_objectType = GameObjectType::Modifier;
        this->m_baseColor->m_defaultColorID = 0;
        this->m_greenDebugDraw = true;
        this->m_width = 25;
        this->m_height = 5;

        this->m_isTrigger = false;
        this->m_isSpawnTriggered = false;
        this->m_isTouchTriggered = true;
        this->m_isMultiTriggered = false;

        return true;
    } // init

    // Copies the behaviour of a regular pad, with adjustable bump strength
    void bumpPlayer(PlayerObject* player, float power, GameObjectType effectType = GameObjectType::YellowJumpPad) {
        player->bumpPlayer(power, (int)effectType, this->m_hasNoEffects, this);
    } // bumpPlayer

    void triggerObject(GJBaseGameLayer* level, int playerID, gd::vector<int> const*) override final {
        auto player = (level->m_player2->m_uniqueID == playerID) ? level->m_player2 : level->m_player1;
        level->m_effectManager->removeTriggeredID(this->m_uniqueID, player->m_uniqueID);

        if (level->canBeActivatedByPlayer(player, this)) {
            player->m_lastActivatedPortal = this;
            player->m_lastPortalPos = this->getPosition();
            this->activatedByPlayer(player);
            this->activateCustomObject(level, player);
        } // if
    } // triggerObject

    void customSetup() override final {
        if (!this->m_editorEnabled && !this->m_hasNoParticles) {
            this->createAndAddParticle(9, "bumpEffect.plist", 4, tCCPositionType::kCCPositionTypeGrouped);
            this->claimParticle();
        } // if
        CustomObjectBase::customSetup();
        this->m_dontIgnoreDuration = false;
    } // customSetup

    bool canAllowMultiActivate() override {
        return true;
    } // canAllowMultiActivate
};

class $generic(CustomPadObject);