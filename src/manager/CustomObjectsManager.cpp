#include <Geode/Geode.hpp>
#include "CustomObjectsManager.hpp"
#include "CustomObjectsSheet.hpp"
#include "../config/CustomSpriteConfig.hpp"

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
    auto it = m_customObjects.try_emplace(objectID, std::make_unique<CustomObjectConfig>(stringID, objectID, ctor));

    if (!it.second) {
        log::error("Duplicate object ID \"{}\"!", stringID);
        return nullptr;
    }
    return it.first->second.get();
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
}

void CustomObjectsManager::processRegisteredObjects() {
    auto toolbox = ObjectToolbox::sharedState();
    for (auto& [id, obj] : m_customObjects) {
        toolbox->m_allKeys.emplace(obj->getObjectID(), obj->getMainSprite());
        if (obj->hasCustomAnimation()) {
            auto mainAnimSprite = obj->getMainSprite();
            auto detailAnimSprite = obj->hasDetailSprite() ? obj->getDetailSprite() : obj->getMainSprite();

            mainAnimSprite = mainAnimSprite.substr(0, mainAnimSprite.find("_001"));
            detailAnimSprite = detailAnimSprite.substr(0, detailAnimSprite.find("_001"));

            auto manager = GameManager::sharedState();
            manager->addGameAnimation(obj->getObjectID(), obj->getFramesCount(), obj->getFrameTime(), mainAnimSprite, detailAnimSprite, 1);
        }
    }
}

void CustomObjectsManager::printModObjectCount() const {
    std::map<std::string, int> mods;
    for (const auto& [id, obj] : m_customObjects) mods[obj->getModID()]++;
    log::info("A total of {} mods registered {} total custom objects", mods.size(), m_customObjects.size());
}

void CustomObjectsManager::registerCustomSprite(CustomSpriteConfig* config) {
    m_customSprites.push_back(config);
}

void CustomObjectsManager::unregisterCustomSprite(CustomSpriteConfig* config) {
    std::erase(m_customSprites, config);
}

int CustomObjectsManager::getCustomSpritesCount() const {
    return m_customSprites.size();
}

void CustomObjectsManager::forEachCustomSprite(std::function<void(const CustomSpriteConfig*)> callback) const {
    for (const auto config : m_customSprites) callback(config);
}

bool CustomObjectsManager::isTheSpritesheetCacheUpToDate() const {
    auto sheetName = CustomObjectsSheet::getSpritesheetQualityName();
    auto cache = Mod::get()->getSavedValue<std::vector<std::string>>(sheetName);
    CCFileUtils::get()->addSearchPath(CustomObjectsSheet::getCacheDirectory().c_str());

    if (m_customSprites.size() == 0) return true;
    else if (m_customSprites.size() > cache.size()) return false;

    for (int i = 0; i < m_customSprites.size(); i++) {
        if (m_customSprites[i]->getFrameName() == cache[i]) continue;
        if (std::find(cache.begin(), cache.end(), m_customSprites[i]->getFrameName()) != cache.end()) continue;
        return false;
    }

    auto png = sheetName + ".png";
    if (png == CCFileUtils::get()->fullPathForFilename(png.c_str(), false)) return false;

    auto plist = sheetName + ".plist";
    if (plist == CCFileUtils::get()->fullPathForFilename(plist.c_str(), false)) return false;

    return true;
}

void CustomObjectsManager::generateCustomSpritesheets() const {
    switch (CustomObjectsSheet::getTextureQuality()) {
        case Quality::HIGH:
            CustomObjectsSheet::addSpritesheetToCache(m_customSprites, Quality::HIGH);
            [[fallthrough]];
        case Quality::MEDIUM:
            CustomObjectsSheet::addSpritesheetToCache(m_customSprites, Quality::MEDIUM);
            [[fallthrough]];
        case Quality::LOW:
            CustomObjectsSheet::addSpritesheetToCache(m_customSprites, Quality::LOW);
            [[fallthrough]];
        default: return;
    }
}

cocos2d::CCTexture2D* CustomObjectsManager::getCustomSpritesheetTexture() const {
    if (getCustomObjectsCount() == 0) return nullptr;

    static CCTexture2D* texture = nullptr;
    if (texture) return texture;

    CCFileUtils::get()->addSearchPath(CustomObjectsSheet::getCacheDirectory().c_str());
    auto png = CustomObjectsSheet::getSpritesheetQualityName() + ".png";
    texture = CCTextureCache::get()->addImage(png.c_str(), false);
    return texture;
}