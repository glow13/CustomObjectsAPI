#pragma once
#include <Geode/Geode.hpp>

#include "CustomObjectBase.hpp"

using namespace geode::prelude;

class $base(CustomGameObject) {
public:
    bool init(const CustomObjectConfig<ObjectType>* config) {
        if (!CustomBase::init(config)) return false;

        this->m_objectType = GameObjectType::Decoration;

        return true;
    } // init
};

class $generic(CustomGameObject);