#pragma once

class CustomObjectsManager final {
    using ObjectConstructor = geode::Function<class CustomObjectInterface*()>; // these should match
    std::unordered_map<int, std::unique_ptr<class CustomObjectConfig>> m_customObjects;
    std::vector<class CustomSpriteConfig*> m_customSprites;
    std::map<std::string, int> m_mods;
public:
    static CustomObjectsManager* get();

    CustomObjectConfig* registerObjectConfig(std::string_view, ObjectConstructor);
    void registerCustomSprite(CustomSpriteConfig*);
    void unregisterCustomSprite(CustomSpriteConfig*);

    int getCustomObjectsCount() const;
    int getCustomSpritesCount() const;

    CustomObjectConfig* getCustomObjectWithID(int) const;
    GameObject* createCustomObjectWithID(int) const;

    bool hasCustomEditObjectForID(int) const;
    bool customEditObjectForID(int, GameObject*, cocos2d::CCArray*) const;
    bool hasCustomEditSpecialForID(int) const;
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