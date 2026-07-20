#pragma once
#include <memory>

class CustomObjectsManager {
    std::unordered_map<int, std::shared_ptr<class CustomObjectConfig>> customObjects;
public:
    static CustomObjectsManager* get();
    std::shared_ptr<CustomObjectConfig> registerObjectConfig(std::string_view stringID, GameObject*(*ctor)());
    void forEachCustomObject(std::function<void(const CustomObjectConfig*)> callback) const;
    int getTotalCustomObjectsCount() const;

    const std::shared_ptr<CustomObjectConfig> getCustomObjectByID(int id) const;
    GameObject* createCustomObjectWithID(int id) const;
};