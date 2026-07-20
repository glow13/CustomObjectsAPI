#include <Geode/Geode.hpp>
#include "CustomObjectConfig.hpp"
#include "../manager/CustomObjectsManager.hpp"

using namespace geode::prelude;

struct CustomObjectConfig::Impl {
    ObjectCreator ctor;
    int objectID;
};

CustomObjectConfig::~CustomObjectConfig() = default;
CustomObjectConfig::CustomObjectConfig(int objectID, ObjectCreator ctor) : impl(std::make_unique<CustomObjectConfig::Impl>()) {
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

std::shared_ptr<CustomObjectConfig> CustomObjectConfig::registerConfig(std::string_view stringID, ObjectCreator ctor) {
    return CustomObjectsManager::get()->registerObjectConfig(stringID, ctor);
}