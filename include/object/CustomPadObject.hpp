#pragma once

#include "CustomObjectBase.hpp"

template <class ObjectType>
class $base(CustomPadObject, EffectGameObject) {
protected:
    using CustomObjectBase = CustomObjectBase<ObjectType, EffectGameObject>::Base;
public:
    bool init(const CustomObjectConfig<ObjectType>* config) {
        if (!CustomObjectBase::init(config)) return false;

        this->m_objectType = GameObjectType::Modifier;
        this->m_baseColor->m_defaultColorID = 0;
        this->m_greenDebugDraw = true;
        this->m_width = 25;
        this->m_height = 5;

        return true;
    } // init

    // Copies the behaviour of a regular pad, with adjustable bump strength
    void bumpPlayer(PlayerObject* player, float power, GameObjectType effectType = GameObjectType::YellowJumpPad) {
        player->bumpPlayer(power, (int)effectType, this->m_hasNoEffects, this);
    } // bumpPlayer

private:
    void triggerObject(GJBaseGameLayer* level, int playerID, gd::vector<int> const*) override {
        auto player = (level->m_player2->m_uniqueID == playerID) ? level->m_player2 : level->m_player1;
        level->m_effectManager->removeTriggeredID(this->m_uniqueID, player->m_uniqueID);

        if (level->canBeActivatedByPlayer(player, this)) {
            player->m_lastActivatedPortal = this;
            player->m_lastPortalPos = this->getPosition();
            this->activatedByPlayer(player);
            this->activateCustomObject(level, player);
        } // if
    } // triggerObject

    void customSetup() override {
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