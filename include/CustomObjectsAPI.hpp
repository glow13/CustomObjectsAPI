#pragma once
#include <Geode/Geode.hpp>

#include "CustomObjectsStructs.hpp"
#include "object/CustomGameObject.hpp"

using namespace geode::prelude;

class CustomObjectsManager : public CCNode {
private:
    static inline CustomObjectsManager* s_manager;

    short m_generationOffsetValue;

    std::map<int, CustomObject> customObjectsCache;
    std::map<std::string, std::vector<CustomObject>> modCustomObjectsCache;

public:
    static CustomObjectsManager* get();

    int modToObjectId(std::string modId);
    void setModCustomObjectGenerationValue(short value) { m_generationOffsetValue = value; }

    std::string getCacheDirectory();
    std::string getSpritesheetQualityName();

    int getObjectCount() { return customObjectsCache.size(); }
    int getModObjectCount(std::string id);
    void printModObjectCount();

    CustomObject getCustomObjectByID(int id) { return customObjectsCache[id]; }
    bool containsCustomObject(int id) { return customObjectsCache.contains(id); }
    void forEachCustomObject(std::function<void(const CustomObject)> operation) const;

    bool isTheSpritesheetCacheUpToDate();
    void addSpritesheetToCache(std::string name, Quality quality);

    void registerCustomObject(std::string spr, CCSize size, std::function<GameObject*(int)> create = CustomGameObject::create);
    void registerCustomObject(std::string spr, std::function<GameObject*(int)> create = CustomGameObject::create) {
        registerCustomObject(spr, CCSize(1, 1), create);
    } // registerCustomObject
};