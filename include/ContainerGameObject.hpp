#pragma once
#include "CustomGameObject.hpp"

class ContainerGameObject : public CustomGameObject {
public:
    static ContainerGameObject* create(int id) {
        auto obj = new ContainerGameObject();
        if (obj->init(id)) return obj;

        delete obj;
        return nullptr;
    } // create

    bool customInit() {
        m_objectType = GameObjectType::Solid;
        return true;
    } // customInit
};