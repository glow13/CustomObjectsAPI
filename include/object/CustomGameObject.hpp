#pragma once
#include <Geode/Geode.hpp>

#include "CustomObjectBase.hpp"

class $base(CustomGameObject, GameObject)
public:
    bool init(const CustomObjectConfig<ObjectType>* config) {
        if (!CustomObjectBase::init(config)) return false;

        this->m_objectType = GameObjectType::Decoration;

        return true;
    } // init
};

class $generic(CustomGameObject);