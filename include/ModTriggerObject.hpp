#pragma once

#include "object/CustomTriggerObject.hpp"

class $object(ModTriggerObject, CustomTriggerObject) {
public:
    std::string mod = "";

    bool init(const CustomObjectConfig* config) {
        auto modTriggerFrame = fmt::format("custom-objects/{}/0.0.0.0/mod-trigger.png", config->getModID());
        if (!EffectGameObject::init(modTriggerFrame.c_str())) return false;

        this->m_objectType = GameObjectType::Modifier;
        this->m_baseColor->m_defaultColorID = 0;
        this->m_isInvisible = !this->m_editorEnabled;

        mod = config->getModID();
        return true;
    } // init

    void setupCustomObject() override {
        if (auto lel = LevelEditorLayer::get()) {
            auto objectID = m_objectID;
            m_objectID = 901;
            lel->updateObjectLabel(this);
            m_objectID = objectID;
        } // if
    } // setupCustomObject
};