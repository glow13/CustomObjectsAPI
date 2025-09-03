#pragma once
#include <Geode/Geode.hpp>

#include "CustomObjectUtils.hpp"

using namespace geode::prelude;

template <class ObjectType>
class CustomRotateObjectBase : public CustomObjectUtils<ObjectType, EnhancedGameObject> {
public:
    bool init(CustomObjectConfig config) {
        if (!this->commonSetup(config)) return false;
        this->m_objectType = GameObjectType::Decoration;

        this->m_objectRadius = this->m_width / 2;
        this->m_hasCustomRotation = true;
        this->m_rotationDelta = 360;
        this->m_rotationAngle = 360;
        this->m_unk421 = true; // glow match background

        config.applyBoxRadius(this);
        config.applyObjectType(this);
        config.applyCustomRender(this);

        this->setupCustomObject();
        return true;
    } // init
};

class CustomRotateObject : public CustomRotateObjectBase<CustomRotateObject> {};