#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

template <class ObjectType, class ObjectBase>
class CustomObjectUtils : public ObjectBase {
public:
    static ObjectType* create(CustomObjectConfig config) {
        auto obj = new ObjectType();
        if (obj->init(config)) return obj;

        delete obj;
        return nullptr;
    } // create
};