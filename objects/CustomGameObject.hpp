#pragma once
#include "../include/CustomObjectBase.hpp"

#include <Geode/binding/GameObject.hpp>

/**
 * The most basic custom object class with no extra features.
 * Some vanilla examples include LITERALLY every single object lol.
 * 
 * This is the default class that `CustomObjectsAPI::registerCustomObject` uses.
 */
class $base(CustomGameObject, GameObject) {
public:
    bool init(const CustomObjectConfig&& config) override {
        if (!CustomObjectBase::init(std::move(config))) return false;

        this->m_objectType = GameObjectType::Decoration;

        return true;
    }
};