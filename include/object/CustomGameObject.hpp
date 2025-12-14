#pragma once
#include <Geode/Geode.hpp>

#include "CustomObjectBase.hpp"

template <class ObjectType>
class $base(CustomGameObject, GameObject) {
protected:
    using CustomBase = CustomObjectBase<ObjectType, GameObject>::CustomBase;
public:
    bool init(const CustomObjectConfig<ObjectType>* config) {
        if (!CustomBase::init(config)) return false;

        this->m_objectType = GameObjectType::Decoration;

        return true;
    } // init
};

class $generic(CustomGameObject);