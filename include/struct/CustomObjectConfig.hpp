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
#define GLOW_COLOR_DEFAULT ccColor3B(255,255,255)
#define PARTICLE_COLOR_DEFAULT ccColor3B(255,255,255)
#define PARTICLE_OPACITY_DEFUALT 255

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
    ccColor3B glowColor = GLOW_COLOR_DEFAULT;
    ccColor3B particleColor = PARTICLE_COLOR_DEFAULT;
    GLubyte particleOpacity = PARTICLE_OPACITY_DEFUALT;

    ICustomObjectConfig() {}
    ICustomObjectConfig(std::string mod, int id) : mod(mod), id(id) {}

    inline bool isCustomRender() const { return parentMode != PARENT_MODE_DEFAULT; }
    inline bool hasCustomAnimation() const { return framesCount != FRAMES_COUNT_DEFAULT && mainSprite.isAnimationFrame(); }

    virtual bool hasEditObjectFunction() const { return false; }
    virtual bool hasEditSpecialFunction() const { return false; }

    virtual void customEditObject(GameObject*, CCArray*) const {}
    virtual void customEditSpecial(GameObject*, CCArray*) const {}

    virtual GameObject* create() const { return nullptr; };
};

template <class ObjectType>
struct CustomObjectConfig : public ICustomObjectConfig {
public:
    std::function<void(ObjectType*)> setupCustomObjectFunction;
    std::function<void(ObjectType*)> resetCustomObjectFunction;
    std::function<void(ObjectType*, CCArray*)> editObjectFunction;
    std::function<void(ObjectType*, CCArray*)> editSpecialFunction;
    std::function<void(ObjectType*, GJBaseGameLayer*, PlayerObject*)> activateCustomObjectFunction;

    bool hasEditObjectFunction() const override { return (bool)editObjectFunction; }
    bool hasEditSpecialFunction() const override { return (bool)editSpecialFunction; }

    void customEditObject(GameObject* obj, CCArray* objs) const override { editObjectFunction(static_cast<ObjectType*>(obj), objs); }
    void customEditSpecial(GameObject* obj, CCArray* objs) const override { editSpecialFunction(static_cast<ObjectType*>(obj), objs); }

    CustomObjectConfig(std::string mod, int id) : ICustomObjectConfig(mod, id) {}

    // Set object sprites
    CustomObjectConfig<ObjectType>& setMainSprite(std::string frame, int x, int y, int w, int h) { mainSprite = CustomSpriteConfig(frame, mod, CCRect(x, y, w, h)); return *this; }
    CustomObjectConfig<ObjectType>& setDetailSprite(std::string frame, int x, int y, int w, int h) { detailSprite = CustomSpriteConfig(frame, mod, CCRect(x, y, w, h)); return *this; }
    CustomObjectConfig<ObjectType>& setGlowSprite(std::string frame, int x, int y, int w, int h) { glowSprite = CustomSpriteConfig(frame, mod, CCRect(x, y, w, h)); return *this; }

    // Inline helper functions for sprite frames
    inline CustomObjectConfig<ObjectType>& setMainSprite(std::string frame, int w, int h) { return setMainSprite(frame, 0, 0, w, h); }
    inline CustomObjectConfig<ObjectType>& setMainSprite(std::string frame, int size) { return setMainSprite(frame, 0, 0, size, size); }
    inline CustomObjectConfig<ObjectType>& setMainSprite(std::string frame) { return setMainSprite(frame, 0, 0, 0, 0); }
    inline CustomObjectConfig<ObjectType>& setDetailSprite(std::string frame, int w, int h) { return setDetailSprite(frame, 0, 0, w, h); }
    inline CustomObjectConfig<ObjectType>& setDetailSprite(std::string frame, int size) { return setDetailSprite(frame, 0, 0, size, size); }
    inline CustomObjectConfig<ObjectType>& setDetailSprite(std::string frame) { return setDetailSprite(frame, 0, 0, 0, 0); }
    inline CustomObjectConfig<ObjectType>& setGlowSprite(std::string frame, int w, int h) { return setGlowSprite(frame, 0, 0, w, h); }
    inline CustomObjectConfig<ObjectType>& setGlowSprite(std::string frame, int size) { return setGlowSprite(frame, 0, 0, size, size); }
    inline CustomObjectConfig<ObjectType>& setGlowSprite(std::string frame) { return setGlowSprite(frame, 0, 0, 0, 0); }

    // Set object config values
    CustomObjectConfig<ObjectType>& setBoxSize(int w, int h) { boxSize = CCSize(w, h); return *this; }
    CustomObjectConfig<ObjectType>& setBoxOffset(int x, int y) { boxOffset = CCPoint(x, y); return *this; }
    CustomObjectConfig<ObjectType>& setBoxRadius(int radius) { boxRadius = radius; return *this; }
    CustomObjectConfig<ObjectType>& setCreateOffset(int x, int y) { createOffset = CCPoint(x, y); return *this; }
    CustomObjectConfig<ObjectType>& setObjectType(GameObjectType type) { objectType = type; return *this; }
    CustomObjectConfig<ObjectType>& setCustomRender(int parent = 4) { parentMode = parent; return *this; }
    CustomObjectConfig<ObjectType>& setFramesCount(int frames) { framesCount = frames; return *this; }
    CustomObjectConfig<ObjectType>& setFrameTime(float time) { frameTime = time; return *this; }
    CustomObjectConfig<ObjectType>& setGlowColor(GLubyte r, GLubyte g, GLubyte b) { glowColor = {r,g,b}; return *this; }
    CustomObjectConfig<ObjectType>& setParticleColor(GLubyte r, GLubyte g, GLubyte b) { particleColor = {r,g,b}; return *this; }
    CustomObjectConfig<ObjectType>& setParticleOpacity(GLubyte opacity) { particleOpacity = opacity; return *this; }

    // Set callback functions
    CustomObjectConfig<ObjectType>& onEditObjectButton(std::function<void(ObjectType*, CCArray*)> callback) { editObjectFunction = callback; return *this; }
    CustomObjectConfig<ObjectType>& onEditSpecialButton(std::function<void(ObjectType*, CCArray*)> callback) { editSpecialFunction = callback; return *this; }
    CustomObjectConfig<ObjectType>& onSetupCustomObject(std::function<void(ObjectType*)> callback) { setupCustomObjectFunction = callback; return *this; }
    CustomObjectConfig<ObjectType>& onResetCustomObject(std::function<void(ObjectType*)> callback) { resetCustomObjectFunction = callback; return *this; }
    CustomObjectConfig<ObjectType>& onActivateCustomObject(std::function<void(ObjectType*, GJBaseGameLayer*, PlayerObject*)> callback) { activateCustomObjectFunction = callback; return *this; }

    // Create new object using this config
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
        if (glowColor != GLOW_COLOR_DEFAULT) obj->setGlowColor(glowColor);

        return obj;
    } // create
};