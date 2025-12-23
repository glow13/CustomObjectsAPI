#pragma once

#include "CustomObjectBase.hpp"

template <class ObjectType>
class $base(CustomTriggerObject, EffectGameObject) {
protected:
    using CustomObjectBase = CustomObjectBase<ObjectType, EffectGameObject>::Base;
public:
    bool init(const CustomObjectConfig<ObjectType>* config) {
        if (!CustomObjectBase::init(config)) return false;

        this->m_objectType = GameObjectType::Modifier;
        this->m_baseColor->m_defaultColorID = 0;
        this->m_isInvisible = !this->m_editorEnabled;

        return true;
    } // init

    void updateObjectLabel() {
        auto lel = LevelEditorLayer::get();
        if (!lel) return;

        auto objectID = this->m_objectID;
        this->m_objectID = 901;
        lel->updateObjectLabel(this);
        this->m_objectID = objectID;
    } // updateObjectLabel

    void customSetup() override final {
        auto ignoreDuration = this->m_dontIgnoreDuration;
        CustomObjectBase::customSetup();

        this->m_dontIgnoreDuration = ignoreDuration;
        this->m_activateTriggerInEditor = true;
        this->m_canBeControlled = true;
        this->m_isTrigger = true;
    } // customSetup

    void triggerObject(GJBaseGameLayer* level, int playerID, gd::vector<int> const*) override final {
        auto player = (level->m_player2->m_uniqueID == playerID) ? level->m_player2 : level->m_player1;
        this->activateCustomObject(level, player);
    } // triggerObject
};

class $generic(CustomTriggerObject);