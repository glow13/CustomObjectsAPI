#pragma once
#include <memory>

class CustomObjectsManager {
    std::unordered_map<int, std::unique_ptr<class CustomObjectConfig>> customObjects;
public:
    static CustomObjectsManager* get();
    CustomObjectConfig* registerObjectConfig(std::string_view stringID, GameObject*(*ctor)());
    void forEachCustomObject(std::function<void(const CustomObjectConfig*)> callback) const;
    int getTotalCustomObjectsCount() const;

    CustomObjectConfig* getCustomObjectByID(int id) const;
    GameObject* createCustomObjectWithID(int id) const;
};