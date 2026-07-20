#pragma once
#include <memory>

using geode::geode_internal::StringConcatModIDSlash;

class CustomObjectConfig final {
public:
    using ObjectConstructor = GameObject*(*)();
    CustomObjectConfig(int, ObjectConstructor);
    ~CustomObjectConfig();

    int getObjectID() const;

private:
    struct Impl;
    std::unique_ptr<Impl> impl;

    GameObject* createCustomObject() const;
    static CustomObjectConfig* registerConfig(std::string_view, ObjectConstructor);
    template <class Object> static GameObject* objectFactory() { return new Object(); }

    template <class, StringConcatModIDSlash> friend class ConfigObject;
    friend class CustomObjectsManager;
};

template <class ObjectType, StringConcatModIDSlash StringID>
class ConfigObject {
    static inline struct {
        CustomObjectConfig* config = CustomObjectConfig::registerConfig(StringID.buffer, &CustomObjectConfig::objectFactory<ObjectType>);
        bool initialized = +[](){ ObjectType::onLoad(); return true; }();
    } data;
    static inline auto dataRef = &data;
protected:
    static CustomObjectConfig* getConfig() { return data.config; }
    static bool isInitialized() { return data.initialized; }
};