#pragma once
#include <Geode/Geode.hpp>

#include "CustomObjectBase.hpp"

using namespace geode::prelude;

template <class ObjectType>
class CustomAnimatedObjectBase : public CustomObjectBase<ObjectType, EnhancedGameObject> {
public:
    bool init(const CustomObjectConfig<ObjectType>* config) {
        if (!this->commonSetup(config)) return false;

        this->m_objectType = GameObjectType::Decoration;
        this->m_hasCustomAnimation = config->hasCustomAnimation();

        return true;
    } // init
};

class CustomAnimatedObject : public CustomAnimatedObjectBase<CustomAnimatedObject> {};