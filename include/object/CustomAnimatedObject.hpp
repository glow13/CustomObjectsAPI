#pragma once
#include <Geode/Geode.hpp>

#include "CustomObjectUtils.hpp"

using namespace geode::prelude;

template <class ObjectType>
class CustomAnimatedObjectBase : public CustomObjectUtils<ObjectType, EnhancedGameObject> {
public:
    bool init(const CustomObjectConfig& config) {
        if (!this->commonSetup(config)) return false;

        this->m_objectType = GameObjectType::Decoration;
        this->m_hasCustomAnimation = config.hasCustomAnimation();

        return true;
    } // init
};

class CustomAnimatedObject : public CustomAnimatedObjectBase<CustomAnimatedObject> {};