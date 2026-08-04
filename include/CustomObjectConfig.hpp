#pragma once
#include <memory>

using geode::geode_internal::StringConcatModIDSlash;

class CustomObjectConfig final {
public:
    using ObjectConstructor = GameObject*(*)(const CustomObjectConfig*);
    using EditObjectCallback = void(*)(GameObject*, cocos2d::CCArray*);

    CustomObjectConfig(int, ObjectConstructor);
    ~CustomObjectConfig();

    int getObjectID() const;

    void onEditObjectButton(EditObjectCallback);
    void onEditSpecialButton(EditObjectCallback);

    bool hasEditObjectCallback() const;
    bool hasEditSpecialCallback() const;

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;

    GameObject* createCustomObject() const;
    static CustomObjectConfig* registerConfig(std::string_view, ObjectConstructor);

    void customEditObject(GameObject*, cocos2d::CCArray*) const;
    void customEditSpecial(GameObject*, cocos2d::CCArray*) const;

    template <class, StringConcatModIDSlash> friend class ConfigGameObject;
    friend class CustomObjectsManager;
};

template <class ObjectType, StringConcatModIDSlash StringID>
class ConfigGameObject {
    static inline struct {
        CustomObjectConfig* config = CustomObjectConfig::registerConfig(StringID.buffer,
            +[](const CustomObjectConfig* config) -> GameObject* { return createWithConfig(config); });
        bool initialized = +[](){ ObjectType::onRegisterConfig((CustomObjectConfig&&)*data.config); return true; }();
    } data;
    static inline auto dataRef = &data;
protected:
    static void onRegisterConfig(CustomObjectConfig&&) { /* do nothing by default */ }
    static const CustomObjectConfig* getConfig() { return data.config; }
    static bool isInitialized() { return data.initialized; }
public:
    static ObjectType* createWithConfig(const CustomObjectConfig* config) {
        auto obj = new ObjectType();
        if (obj->init(config)) {
            obj->autorelease();
            return obj;
        }
        delete obj;
        return nullptr;
    }
};