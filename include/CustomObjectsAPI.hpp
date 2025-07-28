#pragma once
#include <Geode/Geode.hpp>

#include "object/CustomGameObject.hpp"

using namespace geode::prelude;

enum Quality {
    LOW = 1,
    MEDIUM = 2,
    HIGH = 4
};

struct ModCustomObject {
    gd::string frame;
    gd::string sourceFrame;
    std::function<GameObject*(int)> createFunction;

    int id;
    CCSize boxSize;
    CCSize spriteSize;

    ModCustomObject() : frame(""), sourceFrame(""), id(0), spriteSize(CCSize(30, 30)), boxSize(CCSize(30, 30)) {}
    ModCustomObject(gd::string frame, int id, CCSize size, std::function<GameObject*(int)> create) : sourceFrame(frame), id(id), spriteSize(size), boxSize(CCSize(30, 30)), createFunction(create) {
        this->frame = fmt::format("custom-objects/{}/{}/", size.width, size.height) + frame.substr(frame.find("/") + 1);
    } // ModCustomObject

    GameObject* create() { return createFunction(id); }
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