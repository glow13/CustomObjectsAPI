#pragma once
#include <Geode/Geode.hpp>

#include "CustomObjectsSheet.hpp"
#include "object/CustomGameObject.hpp"

using namespace geode::prelude;

struct ModCustomObject {
    gd::string m_frame;
    gd::string m_spr;
    gd::string m_mod;
    int m_id;
    CCSize m_spriteSize;
    std::function<GameObject*(int)> m_createFunction;

    ModCustomObject() : m_frame(""), m_spr(""), m_mod(""), m_id(0), m_spriteSize(CCSize(30, 30)) {}

    ModCustomObject(gd::string spr, int id, CCSize size, std::function<GameObject*(int)> create) : m_id(id), m_spriteSize(size), m_createFunction(create) {
        int pos = spr.find("/");
        m_spr = spr.substr(pos + 1);
        m_mod = spr.substr(0, pos);
        m_frame = fmt::format("custom-objects/{}/{}/", size.width, size.height) + m_spr;
    } // ModCustomObject

    GameObject* create() { return m_createFunction(m_id); }
};

class CustomObjectsManager : public CCNode {
private:
    static inline CustomObjectsManager* s_manager;

    short m_generationOffsetValue;

    std::map<int, ModCustomObject> customObjectsCache;
    std::map<gd::string, std::vector<ModCustomObject>> modCustomObjectsCache;

public:
    static CustomObjectsManager* get();

    int modToObjectId(gd::string modId);
    void setModCustomObjectGenerationValue(short value) { m_generationOffsetValue = value; }

    gd::string getCacheDirectory();
    gd::string getSpritesheetQualityName();

    int getObjectCount() { return customObjectsCache.size(); }
    int getModObjectCount(gd::string id);
    void printModObjectCount();

    ModCustomObject getCustomObjectByID(int id) { return customObjectsCache[id]; }
    bool containsCustomObject(int id) { return customObjectsCache.contains(id); }
    void forEachCustomObject(std::function<void(ModCustomObject)> operation);

    void addSpritesheetToCache(gd::string name, Quality quality);

    void registerCustomObject(gd::string spr, CCSize size, std::function<GameObject*(int)> create = CustomGameObject::create);
    void registerCustomObject(gd::string spr, std::function<GameObject*(int)> create = CustomGameObject::create) {
        registerCustomObject(spr, CCSize(1, 1), create);
    } // registerCustomObject
};