#pragma once
#include "CustomObjectConfig.hpp"

constexpr int BASE_OBJECT_ID = 100000;

class CustomObjectsManager {
    using ObjectConstructor = GameObject*(*)(const CustomObjectConfig*);
    std::unordered_map<int, std::unique_ptr<CustomObjectConfig>> m_customObjects;
    std::vector<class CustomSpriteConfig*> m_customSprites;
public:
    static CustomObjectsManager* get();

    CustomObjectConfig* registerObjectConfig(std::string_view, ObjectConstructor);
    void registerCustomSprite(CustomSpriteConfig*);
    void unregisterCustomSprite(CustomSpriteConfig*);

    int getCustomObjectsCount() const;
    int getCustomSpritesCount() const;

    CustomObjectConfig* getCustomObjectWithID(int) const;
    GameObject* createCustomObjectWithID(int) const;

    bool customEditObjectForID(int, GameObject*, cocos2d::CCArray*) const;
    bool customEditSpecialForID(int, GameObject*, cocos2d::CCArray*) const;

    void forEachCustomObject(std::function<void(const CustomObjectConfig*)>) const;
    void forEachCustomSprite(std::function<void(const CustomSpriteConfig*)>) const;

    void processRegisteredObjects();
    void printModObjectCount() const;

    bool isTheSpritesheetCacheUpToDate() const;
    void generateCustomSpritesheets() const;

    using ModObjects = std::vector<std::pair<int, int>>;
    std::map<std::string, ModObjects> getEditorTabLayout() const;
};