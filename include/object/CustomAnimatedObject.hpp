#pragma once
#include <Geode/Geode.hpp>

#include "CustomObjectBase.hpp"

using namespace geode::prelude;

class $base(CustomAnimatedObject, EnhancedGameObject) {
public:
    bool init(const CustomObjectConfig<ObjectType>* config) {
        if (!CustomBase::init(config)) return false;

        this->m_objectType = GameObjectType::Decoration;
        this->m_hasCustomAnimation = config->hasCustomAnimation();

        return true;
    } // init
};

class $generic(CustomAnimatedObject);