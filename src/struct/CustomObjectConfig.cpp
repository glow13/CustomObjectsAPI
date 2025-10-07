#include "struct/CustomObjectConfig.hpp"

CustomObjectConfig& CustomObjectConfig::setMainSprite(std::string frame, int w, int h) { mainSprite = CustomSpriteConfig(frame, mod, CCSize(w, h)); return *this; }
CustomObjectConfig& CustomObjectConfig::setDetailSprite(std::string frame, int w, int h) { detailSprite = CustomSpriteConfig(frame, mod, CCSize(w, h)); return *this; }
CustomObjectConfig& CustomObjectConfig::setGlowSprite(std::string frame, int w, int h) { glowSprite = CustomSpriteConfig(frame, mod, CCSize(w, h)); return *this; }

CustomObjectConfig& CustomObjectConfig::setBoxSize(int w, int h) { boxSize = CCSize(w, h); return *this; }
CustomObjectConfig& CustomObjectConfig::setBoxOffset(int x, int y) { boxOffset = CCPoint(x, y); return *this; }
CustomObjectConfig& CustomObjectConfig::setBoxRadius(int radius) { boxRadius = radius; return *this; }
CustomObjectConfig& CustomObjectConfig::setCreateOffset(int x, int y) { createOffset = CCPoint(x, y); return *this; }
CustomObjectConfig& CustomObjectConfig::setObjectType(GameObjectType type) { objectType = type; return *this; }
CustomObjectConfig& CustomObjectConfig::setCustomRender(int parent) { parentMode = parent; return *this; }
CustomObjectConfig& CustomObjectConfig::setFramesCount(int frames) { framesCount = frames; return *this; }
CustomObjectConfig& CustomObjectConfig::setFrameTime(float time) { frameTime = time; return *this; }

CustomObjectConfig::CustomObjectConfig(std::string mod, int id, std::function<GameObject*(CustomObjectConfig)> create) {
    this->mod = mod;
    this->id = id;
    this->createFunction = create;
} // CustomObjectConfig

GameObject* CustomObjectConfig::create() {
    GameObject* obj = createFunction(*this);

    // Setup custom object values
    obj->m_parentMode = 10;
    obj->m_objectID = id;

    // Apply object config
    if (boxSize != BOX_SIZE_DEFAULT) { obj->m_width = boxSize.width; obj->m_height = boxSize.height; }
    if (boxOffset != BOX_OFFSET_DEFAULT) obj->m_customBoxOffset = boxOffset;
    if (boxRadius != BOX_RADIUS_DEFAULT) obj->m_objectRadius = boxRadius;
    if (createOffset != CREATE_OFFSET_DEFAULT) obj->m_unk464 = createOffset;
    if (objectType != OBJECT_TYPE_DEFAULT) obj->m_objectType = objectType;
    if (parentMode != PARENT_MODE_DEFAULT) obj->m_parentMode = parentMode;

    return obj;
} // create