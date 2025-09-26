#pragma once
#include <Geode/Geode.hpp>

#include "CustomObjectUtils.hpp"

using namespace geode::prelude;

template <class ObjectType>
class CustomTriggerObjectBase : public CustomObjectUtils<ObjectType, EffectGameObject> {
public:
    bool init(const CustomObjectConfig& config) {
        if (!this->commonSetup(config)) return false;

        this->m_objectType = GameObjectType::Modifier;
        this->m_dontIgnoreDuration = true;
        this->m_isTrigger = true;

        return true;
    } // init

protected:
    virtual void activateCustomTrigger(PlayerObject* player) {}

private:
    void customSetup() override {
        EffectGameObject::customSetup();
        this->setDontDraw(!this->m_editorEnabled);
        this->m_dontIgnoreDuration = true;
        this->m_isTrigger = true;
    } // customSetup

    void triggerObject(GJBaseGameLayer* level, int playerID, gd::vector<int> const*) override {
        activateCustomTrigger((level->m_player2->m_uniqueID == playerID) ? level->m_player2 : level->m_player1);
    } // triggerObject
};

class CustomTriggerObject : public CustomTriggerObjectBase<CustomTriggerObject> {};