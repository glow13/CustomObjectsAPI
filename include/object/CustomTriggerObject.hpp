#pragma once
#include <Geode/Geode.hpp>

#include "CustomObjectBase.hpp"

using namespace geode::prelude;

template <class ObjectType>
class CustomTriggerObjectBase : public CustomObjectBase<ObjectType, EffectGameObject> {
public:
    bool init(const CustomObjectConfig<ObjectType>* config) {
        if (!this->commonSetup(config)) return false;

        this->m_objectType = GameObjectType::Modifier;
        this->m_dontIgnoreDuration = true;
        this->m_isTrigger = true;
        this->m_baseColor->m_defaultColorID = 0;

        return true;
    } // init

private:
    void customSetup() override {
        CustomObjectBase<ObjectType, EffectGameObject>::customSetup();
        this->setDontDraw(!this->m_editorEnabled);
        this->m_dontIgnoreDuration = true;
        this->m_isTrigger = true;
    } // customSetup

    void triggerObject(GJBaseGameLayer* level, int playerID, gd::vector<int> const*) override {
        auto player = (level->m_player2->m_uniqueID == playerID) ? level->m_player2 : level->m_player1;
        this->activateCustomObject(level, player);
    } // triggerObject
};

class CustomTriggerObject : public CustomTriggerObjectBase<CustomTriggerObject> {};