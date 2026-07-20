#pragma once
#include <memory>

using geode::geode_internal::StringConcatModIDSlash;
using ObjectCreator = GameObject*(*)();

class CustomObjectConfig final {
public:
    CustomObjectConfig(int, ObjectCreator);
    ~CustomObjectConfig();

    int getObjectID() const;

private:
    struct Impl;
    std::unique_ptr<Impl> impl;

    GameObject* createCustomObject() const;
    static std::shared_ptr<CustomObjectConfig> registerConfig(std::string_view, ObjectCreator);
    template <class Object> static GameObject* objectFactory() { return new Object(); }

    template <class, StringConcatModIDSlash> friend class ConfigObject;
    friend class CustomObjectsManager;
};

template <class ObjectType, StringConcatModIDSlash StringID>
class ConfigObject {
    static inline auto config = CustomObjectConfig::registerConfig(StringID.buffer, &CustomObjectConfig::objectFactory<ObjectType>);
    static inline auto configRef = &config;
protected:
    const CustomObjectConfig* getConfig() const { return config; }
};