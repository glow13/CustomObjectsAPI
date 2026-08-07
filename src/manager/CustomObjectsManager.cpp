#include <Geode/Geode.hpp>
#include "CustomObjectsManager.hpp"

using namespace geode::prelude;

CustomObjectsManager* CustomObjectsManager::get() {
    static CustomObjectsManager manager;
    return &manager;
}

int getHashedObjectID(std::string_view stringID) {
    uint32_t hash = geode::utils::hash(stringID);
    constexpr uint32_t range = INT32_MAX - BASE_OBJECT_ID + 1;
    return int(BASE_OBJECT_ID + static_cast<uint32_t>((static_cast<uint64_t>(hash) * range) >> 32));
}

CustomObjectConfig* CustomObjectsManager::registerObjectConfig(std::string_view stringID, ObjectConstructor ctor) {
    int objectID = getHashedObjectID(stringID);
    return m_customObjects.emplace(objectID, std::make_unique<CustomObjectConfig>(stringID, objectID, ctor)).first->second.get();
}

void CustomObjectsManager::forEachCustomObject(std::function<void(const CustomObjectConfig*)> callback) const {
    for (const auto& [id, config] : m_customObjects) callback(config.get());
}

int CustomObjectsManager::getCustomObjectsCount() const {
    return m_customObjects.size();
}

CustomObjectConfig* CustomObjectsManager::getCustomObjectWithID(int id) const {
    auto it = m_customObjects.find(id);
    return it != m_customObjects.end() ? it->second.get() : nullptr;
}

GameObject* CustomObjectsManager::createCustomObjectWithID(int id) const {
    auto it = m_customObjects.find(id);
    return it != m_customObjects.end() ? it->second->createCustomObject() : nullptr;
}

bool CustomObjectsManager::customEditObjectForID(int id, GameObject* obj, CCArray* objs) const {
    auto config = CustomObjectsManager::get()->getCustomObjectWithID(id);
    if (config && config->hasEditObjectCallback()) {
        config->customEditObject(obj, objs);
        return true;
    } else return false;
}

bool CustomObjectsManager::customEditSpecialForID(int id, GameObject* obj, CCArray* objs) const {
    auto config = CustomObjectsManager::get()->getCustomObjectWithID(id);
    if (config && config->hasEditSpecialCallback()) {
        config->customEditSpecial(obj, objs);
        return true;
    } else return false;
}

std::map<std::string, CustomObjectsManager::ModObjects> CustomObjectsManager::getEditorTabLayout() const {
    std::map<std::string, ModObjects> mods;
    for (auto& [id, obj] : m_customObjects) {
        if (obj->getObjectID() < 0) continue;
        mods[obj->getModID()].emplace_back(obj->getEditorPriority(), id);
    }

    for (auto& [mod, objs] : mods) {
        std::sort(objs.begin(), objs.end(), [](const auto& a, const auto& b) {
            return a.first == b.first ? a.second < b.second : a.first < b.first;
        });
    }

    return mods;

void CustomObjectsManager::registerSprite(CustomSpriteConfig* config) {
    m_customSprites.push_back(config);
}

int CustomObjectsManager::getCustomSpritesCount() const {
    return m_customSprites.size();
}

void CustomObjectsManager::forEachCustomSprite(std::function<void(const CustomSpriteConfig*)> callback) const {
    for (const auto config : m_customSprites) callback(config);
}
}