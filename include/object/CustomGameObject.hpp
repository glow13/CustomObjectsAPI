#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

template <class T>
class CustomGameObject : public GameObject {
public:
    static T* create(int id) {
        auto obj = new T();
        if (obj->init(id)) return obj;

        delete obj;
        return nullptr;
    } // create

protected:
    virtual void setupCustomObject() {}

    bool init(int id) {
        auto toolbox = ObjectToolbox::sharedState();
        auto spr = toolbox->intKeyToFrame(id);

        if (GameObject::init(spr)) {
            m_objectID = id;
            m_parentMode = 10;

            setupCustomObject();
            autorelease();
            return true;
        } // if
        return false;
    } // init

private:
    void customObjectSetup(gd::vector<gd::string>& p0, gd::vector<void*>& p1) override {
        GameObject::customObjectSetup(p0, p1);
        setupCustomObject();
    } // customObjectSetup
};

class CustomGameObjectBase : public CustomGameObject<CustomGameObjectBase> {};