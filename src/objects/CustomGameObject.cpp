#include "CustomGameObject.hpp"

CustomGameObject* CustomGameObject::create(int id) {
    auto obj = new CustomGameObject();
    if (obj->init(id)) return obj;
 
    delete obj;
    return nullptr;
} // create

bool CustomGameObject::customInit() {
    m_objectType = GameObjectType::Hazard;
    return true;
} // init