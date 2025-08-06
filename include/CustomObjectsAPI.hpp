#pragma once
#include <Geode/Geode.hpp>

#include "CustomObjectsStructs.hpp"

using namespace geode::prelude;

class CustomObjectsManager : public CCNode {
private:
    static inline CustomObjectsManager* s_manager;

    std::vector<CustomObjectsMod> registeredMods;

    std::map<int, CustomObject> customObjectsCache;
    std::map<std::string, std::vector<CustomObject>> modCustomObjectsCache;

public:
    static CustomObjectsManager* get();

    CustomObjectsMod* registerCustomObjectsMod(geode::Mod* mod, short offset = 0);
    void processRegisteredMods();

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
};