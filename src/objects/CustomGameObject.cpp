#include "CustomGameObject.hpp"

CustomGameObject* CustomGameObject::create(int id) {
    auto obj = new CustomGameObject();

    auto toolbox = ObjectToolbox::sharedState();
    auto spr = toolbox->intKeyToFrame(id);

    if (obj->init(spr) && obj->customInit()) {
        obj->m_objectID = id;
        obj->m_parentMode = 10;
        obj->autorelease();
        return obj;
    } // if
 
    delete obj;
    return nullptr;
} // create

bool CustomGameObject::customInit() {
    m_objectType = GameObjectType::Hazard;
    return true;
} // init