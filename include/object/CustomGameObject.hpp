#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

template <class T>
class CustomGameObject : public GameObject {
public:
    static T* create(CustomObject config) {
        auto obj = new T();
        if (obj->init(config)) return obj;

        delete obj;
        return nullptr;
    } // create

protected:
    virtual void setupCustomObject() {}

    bool init(CustomObject config) {
        if (!GameObject::init(config.frame.c_str())) return false;

        m_objectID = config.id;
        m_parentMode = 10;
        m_objectType = GameObjectType::Solid;

        config.applyBoxSize(this);
        config.applyObjectType(this);

        setupCustomObject();
        autorelease();

        return true;
    } // init

private:
    void customObjectSetup(gd::vector<gd::string>& p0, gd::vector<void*>& p1) override {
        GameObject::customObjectSetup(p0, p1);
        setupCustomObject();
    } // customObjectSetup
};

class CustomGameObjectBase : public CustomGameObject<CustomGameObjectBase> {};