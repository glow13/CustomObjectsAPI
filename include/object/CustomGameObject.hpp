#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

class CustomGameObject : public GameObject {
public:
    static CustomGameObject* create(int id);
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