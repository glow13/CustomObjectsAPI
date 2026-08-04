#include "CustomObjectsManager.hpp"

using namespace geode::prelude;

CustomObjectsManager* CustomObjectsManager::get() {
    static CustomObjectsManager manager;
    return &manager;
}

int getHashedObjectID(std::string_view stringID) {
    constexpr uint32_t min = 10000;
    constexpr uint32_t max = INT32_MAX;
    uint64_t hash = geode::utils::hash(stringID);
    return min + (hash * (max - min)) / UINT32_MAX;
}

CustomObjectConfig* CustomObjectsManager::registerObjectConfig(std::string_view stringID, ObjectConstructor ctor) {
    int objectID = getHashedObjectID(stringID);
    return customObjects.emplace(objectID, std::make_unique<CustomObjectConfig>(objectID, ctor)).first->second.get();
}

void CustomObjectsManager::forEachCustomObject(std::function<void(const CustomObjectConfig*)> callback) const {
    for (const auto& [id, config] : customObjects) callback(config.get());
}

int CustomObjectsManager::getTotalCustomObjectsCount() const {
    return customObjects.size();
}

CustomObjectConfig* CustomObjectsManager::getCustomObjectByID(int id) const {
    auto it = customObjects.find(id);
    return it != customObjects.end() ? it->second.get() : nullptr;
}

GameObject* CustomObjectsManager::createCustomObjectWithID(int id) const {
    auto it = customObjects.find(id);
    return it != customObjects.end() ? it->second->createCustomObject() : nullptr;
}