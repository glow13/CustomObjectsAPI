#pragma once
#include <Geode/Geode.hpp>

#include "CustomSpriteConfig.hpp"

using namespace geode::prelude;

// Config defaults
#define BOX_SIZE_DEFAULT CCSize(0, 0)
#define BOX_OFFSET_DEFAULT CCPoint(0, 0)
#define BOX_RADIUS_DEFAULT 0
#define CREATE_OFFSET_DEFAULT CCPoint(0, 0)
#define OBJECT_TYPE_DEFAULT (GameObjectType)(-1)
#define PARENT_MODE_DEFAULT 10

struct CustomObjectConfig {
public:
    int id;
    std::string mod;
    std::function<GameObject*(CustomObjectConfig)> createFunction;

    // Sprite frame variables
    CustomSpriteConfig mainSprite;
    CustomSpriteConfig detailSprite;
    CustomSpriteConfig glowSprite;

    // Set sprite frames
    CustomObjectConfig& setMainSprite(std::string frame, int w, int h) { mainSprite = CustomSpriteConfig(frame, mod, CCSize(w, h)); return *this; }
    CustomObjectConfig& setMainSprite(std::string frame, int size) { mainSprite = CustomSpriteConfig(frame, mod, CCSize(size, size)); return *this; }
    CustomObjectConfig& setMainSprite(std::string frame) { mainSprite = CustomSpriteConfig(frame, mod, mainSprite.size); return *this; }

    CustomObjectConfig& setDetailSprite(std::string frame, int w, int h) { detailSprite = CustomSpriteConfig(frame, mod, CCSize(w, h)); return *this; }
    CustomObjectConfig& setDetailSprite(std::string frame, int size) { detailSprite = CustomSpriteConfig(frame, mod, CCSize(size, size)); return *this; }
    CustomObjectConfig& setDetailSprite(std::string frame) { detailSprite = CustomSpriteConfig(frame, mod, mainSprite.size); return *this; }

    CustomObjectConfig& setGlowSprite(std::string frame, int w, int h) { glowSprite = CustomSpriteConfig(frame, mod, CCSize(w, h)); return *this; }
    CustomObjectConfig& setGlowSprite(std::string frame, int size) { glowSprite = CustomSpriteConfig(frame, mod, CCSize(size, size)); return *this; }
    CustomObjectConfig& setGlowSprite(std::string frame) { glowSprite = CustomSpriteConfig(frame, mod, mainSprite.size); return *this; }

    // Config variables
    CCSize boxSize = BOX_SIZE_DEFAULT;
    CCPoint boxOffset = BOX_OFFSET_DEFAULT;
    int boxRadius = BOX_RADIUS_DEFAULT;
    CCPoint createOffset = CREATE_OFFSET_DEFAULT;
    GameObjectType objectType = OBJECT_TYPE_DEFAULT;
    int parentMode = PARENT_MODE_DEFAULT;

    // Set config options
    CustomObjectConfig& setBoxSize(int w, int h) { boxSize = CCSize(w, h); return *this; }
    CustomObjectConfig& setBoxOffset(int x, int y) { boxOffset = CCPoint(x, y); return *this; }
    CustomObjectConfig& setBoxRadius(int radius) { boxRadius = radius; return *this; }
    CustomObjectConfig& setCreateOffset(int x, int y) { createOffset = CCPoint(x, y); return *this; }
    CustomObjectConfig& setObjectType(GameObjectType type) { objectType = type; return *this; }
    CustomObjectConfig& setCustomRender(int parent = 4) { parentMode = parent; return *this; }

    bool isCustomRender() { return parentMode != PARENT_MODE_DEFAULT; }

    // Config constructors
    CustomObjectConfig() {}
    CustomObjectConfig(std::string mod, int id, std::function<GameObject*(CustomObjectConfig)> create) : mod(mod), id(id), createFunction(create) {}

    // Create an instance of the custom game object represented by this config struct
    GameObject* create() {
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
};