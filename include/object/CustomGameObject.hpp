#pragma once
#include <Geode/Geode.hpp>

#include "CustomObjectBase.hpp"

template <class ObjectType>
class $base(CustomGameObject, GameObject) {
protected:
    using CustomObjectBase = CustomObjectBase<ObjectType, GameObject>::Base;
public:
    bool init(const CustomObjectConfig<ObjectType>* config) {
        if (!CustomObjectBase::init(config)) return false;

        this->m_objectType = GameObjectType::Decoration;

        return true;
    } // init
};

class $generic(CustomGameObject);