#pragma once
#include <Geode/Geode.hpp>

#include "CustomSpriteConfig.hpp"

class CustomObjectsMod;

struct CUSTOM_OBJECTS_DLL CustomObjectConfigBase {
#ifdef CUSTOM_OBJECTS_INTELLISENSE_DISABLED
protected:
    CustomObjectsMod* mod;
    int objectID;

    CustomSpriteConfig mainSprite;
    CustomSpriteConfig detailSprite;
    CustomSpriteConfig glowSprite;

    cocos2d::CCSize boxSize;
    cocos2d::CCPoint boxOffset;
    int boxRadius;
    cocos2d::CCPoint objectOffset;
    GameObjectType objectType;
    int batchMode;
    bool disableBatch;
    int framesCount;
    float frameTime;
    cocos2d::ccColor3B glowColor;
    cocos2d::ccColor3B particleColor;
    GLubyte particleOpacity;
    bool particleBlending;
    int editorPriority;

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

    void applyConfigValues(GameObject*) const;
    virtual GameObject* create() const = 0;
#endif
};

template <class ObjectType>
struct CustomObjectConfig final : public CustomObjectConfigBase {
private:
#ifdef CUSTOM_OBJECTS_INTELLISENSE_DISABLED
    std::function<void(ObjectType*)> setupCustomObjectCallback;
    std::function<void(ObjectType*)> resetCustomObjectCallback;
    std::function<void(ObjectType*, cocos2d::CCArray*)> editObjectCallback;
    std::function<void(ObjectType*, cocos2d::CCArray*)> editSpecialCallback;
    std::function<void(ObjectType*, GJBaseGameLayer*, PlayerObject*)> activateCustomObjectCallback;

    template <class ObjectType, class ObjectBase>
    friend class CustomObjectBase;

public:
    bool hasEditObjectFunction() const override { return (bool)editObjectCallback; }
    bool hasEditSpecialFunction() const override { return (bool)editSpecialCallback; }

    void customEditObject(GameObject* obj, cocos2d::CCArray* objs) const override { editObjectCallback(static_cast<ObjectType*>(obj), objs); }
    void customEditSpecial(GameObject* obj, cocos2d::CCArray* objs) const override { editSpecialCallback(static_cast<ObjectType*>(obj), objs); }

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
    CustomObjectConfig<ObjectType>& setObjectOffset(int x, int y) { objectOffset = cocos2d::CCPoint(x, y); return *this; }
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

    CustomObjectConfig<ObjectType>& onEditObjectButton(std::function<void(ObjectType*, cocos2d::CCArray*)> callback) { editObjectCallback = callback; return *this; }
    CustomObjectConfig<ObjectType>& onEditSpecialButton(std::function<void(ObjectType*, cocos2d::CCArray*)> callback) { editSpecialCallback = callback; return *this; }
    CustomObjectConfig<ObjectType>& onSetupCustomObject(std::function<void(ObjectType*)> callback) { setupCustomObjectCallback = callback; return *this; }
    CustomObjectConfig<ObjectType>& onResetCustomObject(std::function<void(ObjectType*)> callback) { resetCustomObjectCallback = callback; return *this; }
    CustomObjectConfig<ObjectType>& onActivateCustomObject(std::function<void(ObjectType*, GJBaseGameLayer*, PlayerObject*)> callback) { activateCustomObjectCallback = callback; return *this; }

#ifdef CUSTOM_OBJECTS_INTELLISENSE_DISABLED

    // Create new object using this config
    ObjectType* create() const override {
        ObjectType* obj = ObjectType::create(this);
        applyConfigValues(static_cast<GameObject*>(obj));
        return obj;
    } // create
#endif
};