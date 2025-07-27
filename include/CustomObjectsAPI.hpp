#pragma once
#include <Geode/Geode.hpp>

#include "CustomObjectsSheet.hpp"
#include "object/CustomGameObject.hpp"

using namespace geode::prelude;

struct ModCustomObject : public CCObject {
    gd::string m_frame;
    gd::string m_spr;
    gd::string m_mod;
    int m_id;
    CCSize m_spriteSize;
    std::function<GameObject*(int)> m_createFunction;

    ModCustomObject(gd::string spr, int id, CCSize size, std::function<GameObject*(int)> create) {
        int pos = spr.find("/");
        this->m_spr = spr.substr(pos + 1);
        this->m_mod = spr.substr(0, pos);
        this->m_frame = "custom-objects" + fmt::format("/{}/{}/", size.width, size.height) + m_spr;
        this->m_id = id;
        this->m_spriteSize = size;
        this->m_createFunction = create;
        this->autorelease();
    } // ModCustomObject

    GameObject* create() {
        return m_createFunction(m_id);
    } // create
};

class CustomObjectsManager : public CCNode {
private:
    static inline CustomObjectsManager* s_manager;

    // Dictionary of every registered custom object, the object id is the key and a ModCustomObject is the object
    Ref<CCDictionary> m_customObjectsDict;

    // Dictionary of every registered mod and its objects, the mod id is the key and a CCArray is the object
    Ref<CCDictionary> m_modCustomObjectsDict;

public:
    static CustomObjectsManager* get();

    static int modToObjectId(gd::string modId, int offset = 0);

    gd::string getCacheDirectory();
    gd::string getSpritesheetQualityName();

    int getObjectCount() { return m_customObjectsDict->count(); }
    int getModObjectCount(gd::string id);
    void printModObjectCount();

    ModCustomObject* getCustomObject(int index);
    ModCustomObject* getCustomObjectByID(int id);

    void addSpritesheetToCache(gd::string name, Quality quality);

    void registerCustomObject(gd::string spr, CCSize size, std::function<GameObject*(int)> create = CustomGameObject::create);
    void registerCustomObject(gd::string spr, std::function<GameObject*(int)> create = CustomGameObject::create) {
        registerCustomObject(spr, CCSize(1, 1), create);
    } // registerCustomObject
};