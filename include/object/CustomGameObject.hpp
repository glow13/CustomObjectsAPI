#pragma once
#include <Geode/Geode.hpp>

#include "CustomObjectUtils.hpp"

using namespace geode::prelude;

template <class ObjectType>
class CustomGameObjectBase : public CustomObjectUtils<ObjectType, GameObject> {
public:
    bool init(CustomObjectConfig config) {
        if (!this->commonSetup(config)) return false;
        this->m_objectType = GameObjectType::Solid;

        config.applyBoxSize(this);
        config.applyBoxOffset(this);
        config.applyBoxRadius(this);
        config.applyObjectType(this);
        config.applyCustomRender(this);

        this->setupCustomObject();
        return true;
    } // init
};

class CustomGameObject : public CustomGameObjectBase<CustomGameObject> {};