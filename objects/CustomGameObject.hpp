#pragma once
#include "../include/CustomObjectBase.hpp"

#include <Geode/binding/GameObject.hpp>

/**
 * The most basic custom object class with no extra features.
 * Some vanilla examples include LITERALLY every single object lol.
 * 
 * This is the default class that `CustomObjectsAPI::registerCustomObject` uses.
 */
class $object(CustomGameObject, GameObject) {
public:
    bool init() override {
        if (!CustomObjectBase::init()) return false;

        this->m_objectType = GameObjectType::Decoration;

        return true;
    }
};