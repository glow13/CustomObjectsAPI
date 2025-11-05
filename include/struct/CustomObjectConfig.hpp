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
#define FRAMES_COUNT_DEFAULT 0
#define FRAME_TIME_DEFAULT 1

struct ICustomObjectConfig {
public:
    int id;
    std::string mod;

    CustomSpriteConfig mainSprite;
    CustomSpriteConfig detailSprite;
    CustomSpriteConfig glowSprite;

    CCSize boxSize = BOX_SIZE_DEFAULT;
    CCPoint boxOffset = BOX_OFFSET_DEFAULT;
    int boxRadius = BOX_RADIUS_DEFAULT;
    CCPoint createOffset = CREATE_OFFSET_DEFAULT;
    GameObjectType objectType = OBJECT_TYPE_DEFAULT;
    int parentMode = PARENT_MODE_DEFAULT;
    int framesCount = FRAMES_COUNT_DEFAULT;
    float frameTime = FRAME_TIME_DEFAULT;

    ICustomObjectConfig() {}
    ICustomObjectConfig(std::string mod, int id) : mod(mod), id(id) {}

    inline bool isCustomRender() const { return parentMode != PARENT_MODE_DEFAULT; }
    inline bool hasCustomAnimation() const { return framesCount != FRAMES_COUNT_DEFAULT && mainSprite.isAnimationFrame(); }

    virtual GameObject* create() const { return nullptr; };
};

template <class ObjectType>
struct CustomObjectConfig : public ICustomObjectConfig {
public:
    std::function<void(ObjectType*)> setupCustomObjectFunction;
    std::function<void(ObjectType*)> resetCustomObjectFunction;
    std::function<FLAlertLayer*(ObjectType*, CCArray*)> editObjectFunction;
    std::function<FLAlertLayer*(ObjectType*, CCArray*)> editSpecialFunction;

    CustomObjectConfig(std::string mod, int id) : ICustomObjectConfig(mod, id) {}

    CustomObjectConfig<ObjectType>& setMainSprite(std::string frame, int w, int h) { mainSprite = CustomSpriteConfig(frame, mod, CCSize(w, h)); return *this; }
    CustomObjectConfig<ObjectType>& setDetailSprite(std::string frame, int w, int h) { detailSprite = CustomSpriteConfig(frame, mod, CCSize(w, h)); return *this; }
    CustomObjectConfig<ObjectType>& setGlowSprite(std::string frame, int w, int h) { glowSprite = CustomSpriteConfig(frame, mod, CCSize(w, h)); return *this; }

    CustomObjectConfig<ObjectType>& setBoxSize(int w, int h) { boxSize = CCSize(w, h); return *this; }
    CustomObjectConfig<ObjectType>& setBoxOffset(int x, int y) { boxOffset = CCPoint(x, y); return *this; }
    CustomObjectConfig<ObjectType>& setBoxRadius(int radius) { boxRadius = radius; return *this; }
    CustomObjectConfig<ObjectType>& setCreateOffset(int x, int y) { createOffset = CCPoint(x, y); return *this; }
    CustomObjectConfig<ObjectType>& setObjectType(GameObjectType type) { objectType = type; return *this; }
    CustomObjectConfig<ObjectType>& setCustomRender(int parent = 4) { parentMode = parent; return *this; }
    CustomObjectConfig<ObjectType>& setFramesCount(int frames) { framesCount = frames; return *this; }
    CustomObjectConfig<ObjectType>& setFrameTime(float time) { frameTime = time; return *this; }

    // Inline helper functions for sprite frames
    inline CustomObjectConfig<ObjectType>& setMainSprite(std::string frame, int size) { return setMainSprite(frame, size, size); }
    inline CustomObjectConfig<ObjectType>& setMainSprite(std::string frame) { return setMainSprite(frame, 0, 0); }
    inline CustomObjectConfig<ObjectType>& setDetailSprite(std::string frame, int size) { return setDetailSprite(frame, size, size); }
    inline CustomObjectConfig<ObjectType>& setDetailSprite(std::string frame) { return setDetailSprite(frame, 0, 0); }
    inline CustomObjectConfig<ObjectType>& setGlowSprite(std::string frame, int size) { return setGlowSprite(frame, size, size); }
    inline CustomObjectConfig<ObjectType>& setGlowSprite(std::string frame) { return setGlowSprite(frame, 0, 0); }

    ObjectType* create() const override {
        ObjectType* obj = ObjectType::create(this);

        // Setup custom object values
        obj->m_parentMode = 10;
        obj->m_objectID = id;

        // Apply object config
        if (boxSize != BOX_SIZE_DEFAULT) { obj->m_width = boxSize.width; obj->m_height = boxSize.height; }
        if (boxOffset != BOX_OFFSET_DEFAULT) obj->m_customBoxOffset = boxOffset;
        if (boxRadius != BOX_RADIUS_DEFAULT) obj->m_objectRadius = boxRadius;
        if (objectType != OBJECT_TYPE_DEFAULT) obj->m_objectType = objectType;
        if (parentMode != PARENT_MODE_DEFAULT) obj->m_parentMode = parentMode;

        return obj;
    } // create
};