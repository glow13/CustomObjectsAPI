#pragma once
#include "CustomObjectConfig.hpp"

class CustomObjectsManager {
    using ObjectConstructor = GameObject*(*)(const CustomObjectConfig*);
    std::unordered_map<int, std::unique_ptr<CustomObjectConfig>> customObjects;
public:
    static CustomObjectsManager* get();
    CustomObjectConfig* registerObjectConfig(std::string_view, ObjectConstructor);
    void forEachCustomObject(std::function<void(const CustomObjectConfig*)>) const;
    int getTotalCustomObjectsCount() const;

    CustomObjectConfig* getCustomObjectByID(int) const;
    GameObject* createCustomObjectWithID(int) const;
};