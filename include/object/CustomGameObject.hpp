#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

template <class T>
class CustomGameObjectBase : public GameObject {
public:
    static T* create(int id) {
        auto obj = new T();
        if (obj->init(id)) return obj;

        delete obj;
        return nullptr;
    } // create

    bool init(int id) {
        auto toolbox = ObjectToolbox::sharedState();
        auto spr = toolbox->intKeyToFrame(id);

        if (GameObject::init(spr) && customInit()) {
            m_objectID = id;
            m_parentMode = 10;

            autorelease();
            return true;
        } // if
        return false;
    } // init

    virtual bool customInit() { return true; }
};

class CustomGameObject : public CustomGameObjectBase<CustomGameObject> {};