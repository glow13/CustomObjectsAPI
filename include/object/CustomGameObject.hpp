#pragma once
#include <Geode/Geode.hpp>

#include "CustomObjectUtils.hpp"

using namespace geode::prelude;

template <class ObjectType>
class CustomGameObjectBase : public CustomObjectUtils<ObjectType, GameObject> {
public:
    bool init(const CustomObjectConfig& config) {
        if (!this->commonSetup(config)) return false;
        return true;
    } // init
};

class CustomGameObject : public CustomGameObjectBase<CustomGameObject> {};