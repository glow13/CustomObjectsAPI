#include <Geode/Geode.hpp>
#include <Geode/modify/ObjectToolbox.hpp>

#include "CustomObjectsAPI.hpp"

using namespace geode::prelude;

class $modify(ObjectToolbox) {
public:
    bool init() {
        if (!ObjectToolbox::init()) return false;

        auto manager = CustomObjectsManager::get();
        manager->forEachCustomObject([this](auto obj) {
            m_allKeys.insert(std::pair(obj.id, obj.frame));
        });

        return true;
    } // init
};