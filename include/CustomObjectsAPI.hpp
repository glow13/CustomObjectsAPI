#pragma once
#include <Geode/Geode.hpp>

#include "object/CustomGameObject.hpp"

using namespace geode::prelude;

enum Quality : unsigned int {
    LOW = 1,
    MEDIUM = 2,
    HIGH = 4
};

struct ModCustomObject {
    std::string frame;
    std::string sourceFrame;
    std::function<GameObject*(int)> createFunction;

    int id;
    CCSize boxSize;
    CCSize spriteSize;

    ModCustomObject() : frame(""), sourceFrame(""), id(0), spriteSize(CCSize(30, 30)), boxSize(CCSize(30, 30)) {}
    ModCustomObject(std::string frame, int id, CCSize size, std::function<GameObject*(int)> create) : sourceFrame(frame), id(id), spriteSize(size), boxSize(CCSize(30, 30)), createFunction(create) {
        auto mod = frame.substr(0, frame.find("/"));
        auto spr = frame.substr(frame.find("/") + 1);
        this->frame = fmt::format("custom-objects/{}/{}/{}/{}", mod, size.width, size.height, spr);
    } // ModCustomObject

    GameObject* create() { return createFunction(id); }
};

class CustomObjectsManager : public CCNode {
private:
    static inline CustomObjectsManager* s_manager;

    short m_generationOffsetValue;

    std::map<int, ModCustomObject> customObjectsCache;
    std::map<std::string, std::vector<ModCustomObject>> modCustomObjectsCache;

public:
    static CustomObjectsManager* get();

    int modToObjectId(std::string modId);
    void setModCustomObjectGenerationValue(short value) { m_generationOffsetValue = value; }

    std::string getCacheDirectory();
    std::string getSpritesheetQualityName();

    int getObjectCount() { return customObjectsCache.size(); }
    int getModObjectCount(std::string id);
    void printModObjectCount();

    ModCustomObject getCustomObjectByID(int id) { return customObjectsCache[id]; }
    bool containsCustomObject(int id) { return customObjectsCache.contains(id); }
    void forEachCustomObject(std::function<void(const ModCustomObject)> operation) const;

    bool isTheSpritesheetCacheUpToDate();
    void addSpritesheetToCache(std::string name, Quality quality);

    void registerCustomObject(std::string spr, CCSize size, std::function<GameObject*(int)> create = CustomGameObject::create);
    void registerCustomObject(std::string spr, std::function<GameObject*(int)> create = CustomGameObject::create) {
        registerCustomObject(spr, CCSize(1, 1), create);
    } // registerCustomObject
};