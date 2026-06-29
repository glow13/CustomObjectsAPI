#pragma once
#include "../include/object/CustomObjectBase.hpp"

#include <Geode/binding/GameObject.hpp>

/**
 * The most basic custom object class with no extra features.
 * Some vanilla examples include LITERALLY every single object lol.
 * 
 * This is the default class that `CustomObjectsAPI::registerCustomObject` uses.
 */
template <class ObjectType>
class $base(CustomGameObject, GameObject) {
protected:
    using CustomObjectBase = CustomObjectBase<ObjectType, GameObject>;
    using CustomGameObject = CustomGameObjectBase<ObjectType>;
public:
    bool init(const CustomObjectConfig<ObjectType>* config) {
        if (!CustomObjectBase::init(config)) return false;

        this->m_objectType = GameObjectType::Decoration;

        return true;
    } // init
};

class $generic(CustomGameObject);