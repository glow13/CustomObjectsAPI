#pragma once
#include <Geode/Geode.hpp>

#include "CustomObjectUtils.hpp"

using namespace geode::prelude;

template <class ObjectType>
class CustomGameObject : public CustomObjectUtils<ObjectType, GameObject> {
public:
    bool init(CustomObjectConfig config) {
        if (!GameObject::init(config.frame.c_str())) return false;

        this->m_objectID = config.id;
        this->m_parentMode = 10;
        this->m_objectType = GameObjectType::Solid;

        config.applyBoxSize(this);
        config.applyBoxOffset(this);
        config.applyObjectType(this);
        config.applyCustomRender(this);

        if (!config.detailFrame.empty()) this->addCustomColorChild(config.detailFrame);

        this->setupCustomObject();
        this->autorelease();

        return true;
    } // init
};

class CustomGameObjectBase : public CustomGameObject<CustomGameObjectBase> {};