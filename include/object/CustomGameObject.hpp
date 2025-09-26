#pragma once
#include <Geode/Geode.hpp>

#include "CustomObjectUtils.hpp"

using namespace geode::prelude;

template <class ObjectType>
class CustomGameObjectBase : public CustomObjectUtils<ObjectType, GameObject> {
public:
    bool init(const CustomObjectConfig& config) {
        if (!this->commonSetup(config)) return false;

        this->m_objectType = GameObjectType::Decoration;

        return true;
    } // init
};

class CustomGameObject : public CustomGameObjectBase<CustomGameObject> {};