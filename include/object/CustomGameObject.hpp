#pragma once
#include <Geode/Geode.hpp>

#include "CustomObjectUtils.hpp"

using namespace geode::prelude;

template <class ObjectType>
class CustomGameObjectBase : public CustomObjectUtils<ObjectType, GameObject> {
public:
    bool init(CustomObjectConfig config) {
        if (!this->commonSetup(config)) return false;
        this->m_objectType = GameObjectType::Solid;

        return this->applyConfig(config, BOX_SIZE, BOX_OFFSET, BOX_RADIUS, OBJECT_TYPE, CUSTOM_RENDER);
    } // init
};

class CustomGameObject : public CustomGameObjectBase<CustomGameObject> {};