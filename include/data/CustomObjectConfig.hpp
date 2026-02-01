#pragma once
#include <Geode/Geode.hpp>

#include "CustomSpriteConfig.hpp"

class CustomObjectsMod;

constexpr cocos2d::CCSize BOX_SIZE_DEFAULT = {0, 0};
constexpr cocos2d::CCPoint BOX_OFFSET_DEFAULT = {0, 0};
constexpr int BOX_RADIUS_DEFAULT = 0;
constexpr cocos2d::CCPoint CREATE_OFFSET_DEFAULT = {0, 0};
constexpr GameObjectType OBJECT_TYPE_DEFAULT = (GameObjectType)(-1);
constexpr int BATCH_MODE_DEFAULT = 10;
constexpr bool DISABLE_BATCH_DEFAULT = false;
constexpr int FRAMES_COUNT_DEFAULT = 0;
constexpr float FRAME_TIME_DEFAULT = 1.0f;
constexpr cocos2d::ccColor3B GLOW_COLOR_DEFAULT = {255, 255, 255};
constexpr cocos2d::ccColor3B PARTICLE_COLOR_DEFAULT = {255, 255, 255};
constexpr GLubyte PARTICLE_OPACITY_DEFUALT = 255;
constexpr bool PARTICLE_BLENDING_DEFAULT = true;
constexpr int EDITOR_PRIORITY_DEFAULT = 0;

struct CUSTOM_OBJECTS_DLL CustomObjectConfigBase {
#if __INTELLISENSE__ != 1 || CUSTOM_OBJECTS_EXPORTING
protected:
    CustomObjectsMod* mod;
    int objectID;

    CustomSpriteConfig mainSprite;
    CustomSpriteConfig detailSprite;
    CustomSpriteConfig glowSprite;

    cocos2d::CCSize boxSize = BOX_SIZE_DEFAULT;
    cocos2d::CCPoint boxOffset = BOX_OFFSET_DEFAULT;
    int boxRadius = BOX_RADIUS_DEFAULT;
    cocos2d::CCPoint createOffset = CREATE_OFFSET_DEFAULT;
    GameObjectType objectType = OBJECT_TYPE_DEFAULT;
    int batchMode = BATCH_MODE_DEFAULT;
    bool disableBatch = DISABLE_BATCH_DEFAULT;
    int framesCount = FRAMES_COUNT_DEFAULT;
    float frameTime = FRAME_TIME_DEFAULT;
    cocos2d::ccColor3B glowColor = GLOW_COLOR_DEFAULT;
    cocos2d::ccColor3B particleColor = PARTICLE_COLOR_DEFAULT;
    GLubyte particleOpacity = PARTICLE_OPACITY_DEFUALT;
    bool particleBlending = PARTICLE_BLENDING_DEFAULT;
    int editorPriority = EDITOR_PRIORITY_DEFAULT;

    template <class ObjectType, class ObjectBase>
    friend class CustomObjectBase;
    friend class CustomObjectsManager;
    friend class CustomEditorUI;

public:
    CustomObjectConfigBase(CustomObjectsMod* mod, int id);

    std::string getModID() const;
    std::string getModName() const;
    int getObjectID() const;

    std::string getMainSprite() const;
    std::string getDetailSprite() const;
    std::string getGlowSprite() const;

    bool hasMainSprite() const;
    bool hasDetailSprite() const;
    bool hasGlowSprite() const;

    bool isCustomBatch() const;
    bool hasCustomAnimation() const;

    virtual bool hasEditObjectFunction() const = 0;
    virtual bool hasEditSpecialFunction() const = 0;

    virtual void customEditObject(GameObject*, cocos2d::CCArray*) const = 0;
    virtual void customEditSpecial(GameObject*, cocos2d::CCArray*) const = 0;

    virtual GameObject* create() const = 0;
#endif
};

template <class ObjectType>
struct CustomObjectConfig final : public CustomObjectConfigBase {
private:
#if __INTELLISENSE__ != 1 || CUSTOM_OBJECTS_EXPORTING
    std::function<void(ObjectType*)> setupCustomObjectFunction;
    std::function<void(ObjectType*)> resetCustomObjectFunction;
    std::function<void(ObjectType*, cocos2d::CCArray*)> editObjectFunction;
    std::function<void(ObjectType*, cocos2d::CCArray*)> editSpecialFunction;
    std::function<void(ObjectType*, GJBaseGameLayer*, PlayerObject*)> activateCustomObjectFunction;

    template <class ObjectType, class ObjectBase>
    friend class CustomObjectBase;

public:
    bool hasEditObjectFunction() const override { return (bool)editObjectFunction; }
    bool hasEditSpecialFunction() const override { return (bool)editSpecialFunction; }

    void customEditObject(GameObject* obj, cocos2d::CCArray* objs) const override { editObjectFunction(static_cast<ObjectType*>(obj), objs); }
    void customEditSpecial(GameObject* obj, cocos2d::CCArray* objs) const override { editSpecialFunction(static_cast<ObjectType*>(obj), objs); }

    CustomObjectConfig(CustomObjectsMod* mod, int id) : CustomObjectConfigBase(mod, id) {}
#endif
public:
    CustomObjectConfig<ObjectType>& setMainSprite(std::string frame, int x, int y, int w, int h) { mainSprite = CustomSpriteConfig(mod, this, frame, x, y, w, h); return *this; }
    CustomObjectConfig<ObjectType>& setDetailSprite(std::string frame, int x, int y, int w, int h) { detailSprite = CustomSpriteConfig(mod, this, frame, x, y, w, h); return *this; }
    CustomObjectConfig<ObjectType>& setGlowSprite(std::string frame, int x, int y, int w, int h) { glowSprite = CustomSpriteConfig(mod, this, frame, x, y, w, h); return *this; }

    inline CustomObjectConfig<ObjectType>& setMainSprite(std::string frame, int w, int h) { return setMainSprite(frame, 0, 0, w, h); }
    inline CustomObjectConfig<ObjectType>& setMainSprite(std::string frame, int size) { return setMainSprite(frame, 0, 0, size, size); }
    inline CustomObjectConfig<ObjectType>& setMainSprite(std::string frame) { return setMainSprite(frame, 0, 0, 0, 0); }
    inline CustomObjectConfig<ObjectType>& setDetailSprite(std::string frame, int w, int h) { return setDetailSprite(frame, 0, 0, w, h); }
    inline CustomObjectConfig<ObjectType>& setDetailSprite(std::string frame, int size) { return setDetailSprite(frame, 0, 0, size, size); }
    inline CustomObjectConfig<ObjectType>& setDetailSprite(std::string frame) { return setDetailSprite(frame, 0, 0, 0, 0); }
    inline CustomObjectConfig<ObjectType>& setGlowSprite(std::string frame, int w, int h) { return setGlowSprite(frame, 0, 0, w, h); }
    inline CustomObjectConfig<ObjectType>& setGlowSprite(std::string frame, int size) { return setGlowSprite(frame, 0, 0, size, size); }
    inline CustomObjectConfig<ObjectType>& setGlowSprite(std::string frame) { return setGlowSprite(frame, 0, 0, 0, 0); }

    CustomObjectConfig<ObjectType>& setBoxSize(int w, int h) { boxSize = cocos2d::CCSize(w, h); return *this; }
    CustomObjectConfig<ObjectType>& setBoxOffset(int x, int y) { boxOffset = cocos2d::CCPoint(x, y); return *this; }
    CustomObjectConfig<ObjectType>& setBoxRadius(int radius) { boxRadius = radius; return *this; }
    CustomObjectConfig<ObjectType>& setCreateOffset(int x, int y) { createOffset = cocos2d::CCPoint(x, y); return *this; }
    CustomObjectConfig<ObjectType>& setObjectType(GameObjectType type) { objectType = type; return *this; }
    CustomObjectConfig<ObjectType>& setBatchMode(int mode) { batchMode = mode; return *this; }
    CustomObjectConfig<ObjectType>& setDisableBatchRender() { disableBatch = true; return *this; }
    CustomObjectConfig<ObjectType>& setFramesCount(int frames) { framesCount = frames; return *this; }
    CustomObjectConfig<ObjectType>& setFrameTime(float time) { frameTime = time; return *this; }
    CustomObjectConfig<ObjectType>& setGlowColor(GLubyte r, GLubyte g, GLubyte b) { glowColor = {r,g,b}; return *this; }
    CustomObjectConfig<ObjectType>& setParticleColor(GLubyte r, GLubyte g, GLubyte b) { particleColor = {r,g,b}; return *this; }
    CustomObjectConfig<ObjectType>& setParticleOpacity(GLubyte opacity) { particleOpacity = opacity; return *this; }
    CustomObjectConfig<ObjectType>& setParticleBlending(bool blending) { particleBlending = blending; return *this; }
    CustomObjectConfig<ObjectType>& setEditorTabPriority(int priority) { editorPriority = priority; return *this; }

    CustomObjectConfig<ObjectType>& onEditObjectButton(std::function<void(ObjectType*, cocos2d::CCArray*)> callback) { editObjectFunction = callback; return *this; }
    CustomObjectConfig<ObjectType>& onEditSpecialButton(std::function<void(ObjectType*, cocos2d::CCArray*)> callback) { editSpecialFunction = callback; return *this; }
    CustomObjectConfig<ObjectType>& onSetupCustomObject(std::function<void(ObjectType*)> callback) { setupCustomObjectFunction = callback; return *this; }
    CustomObjectConfig<ObjectType>& onResetCustomObject(std::function<void(ObjectType*)> callback) { resetCustomObjectFunction = callback; return *this; }
    CustomObjectConfig<ObjectType>& onActivateCustomObject(std::function<void(ObjectType*, GJBaseGameLayer*, PlayerObject*)> callback) { activateCustomObjectFunction = callback; return *this; }

#if __INTELLISENSE__ != 1 || CUSTOM_OBJECTS_EXPORTING

    // Create new object using this config
    ObjectType* create() const override {
        ObjectType* obj = ObjectType::create(this);

        // Setup custom object values
        obj->m_parentMode = 10;
        obj->m_objectID = objectID;

        // Apply object config values
        if (boxSize != BOX_SIZE_DEFAULT) { obj->m_width = boxSize.width; obj->m_height = boxSize.height; }
        if (boxOffset != BOX_OFFSET_DEFAULT) obj->m_customBoxOffset = boxOffset;
        if (boxRadius != BOX_RADIUS_DEFAULT) obj->m_objectRadius = boxRadius;
        if (objectType != OBJECT_TYPE_DEFAULT) obj->m_objectType = objectType;
        if (batchMode != BATCH_MODE_DEFAULT) obj->m_parentMode = batchMode;
        if (disableBatch) { obj->m_parentMode = 4; obj->m_addToNodeContainer = true; }
        if (glowColor != GLOW_COLOR_DEFAULT) obj->setGlowColor(glowColor);

        return obj;
    } // create
#endif
};