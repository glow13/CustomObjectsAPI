#include <Geode/Geode.hpp>
#include "CustomObjectConfig.hpp"
#include "../manager/CustomObjectsManager.hpp"

using namespace geode::prelude;

struct CustomObjectConfig::Impl {
    ObjectConstructor ctor;
    int objectID;
};

CustomObjectConfig::~CustomObjectConfig() = default;
CustomObjectConfig::CustomObjectConfig(int objectID, ObjectConstructor ctor) : impl(std::make_unique<CustomObjectConfig::Impl>()) {
    impl->objectID = objectID;
    impl->ctor = ctor;
}

GameObject* CustomObjectConfig::createCustomObject() const {
    if (impl->ctor) {
        auto obj = impl->ctor();
        obj->autorelease();
        return obj;
    }
    return nullptr;
}

int CustomObjectConfig::getObjectID() const {
    return impl->objectID;
}

CustomObjectConfig* CustomObjectConfig::registerConfig(std::string_view stringID, ObjectConstructor ctor) {
    return CustomObjectsManager::get()->registerObjectConfig(stringID, ctor);
}