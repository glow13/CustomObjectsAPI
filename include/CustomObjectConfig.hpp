#pragma once

using geode::geode_internal::StringConcatModIDSlash;

constexpr int BASE_OBJECT_ID = 100000;
constexpr int CUSTOM_PARENT_MODE = 10;

class CustomObjectConfig final {
public:
    using SetupObjectCallback = geode::Function<void(GameObject*)>;
    using ResetObjectCallback = geode::Function<void(GameObject*)>;
    using ActivateObjectCallback = geode::Function<void(GameObject*, GJBaseGameLayer*, PlayerObject*)>;

    using ObjectConstructor = geode::Function<class CustomObjectInterface*(void)>;
    using EditObjectCallback = geode::Function<void(GameObject*, cocos2d::CCArray*)>;

    CustomObjectConfig(std::string_view, int, ObjectConstructor);
    ~CustomObjectConfig();

    #define CONFIG_OPTION(name, type, ...) type get##name() const; CustomObjectConfig&& set##name(__VA_ARGS__);
    CONFIG_OPTION(BoxSize, cocos2d::CCSize, int w, int h);
    CONFIG_OPTION(BoxOffset, cocos2d::CCPoint, int x, int y);
    CONFIG_OPTION(BoxRadius, int, int radius);
    CONFIG_OPTION(ObjectOffset, cocos2d::CCPoint, int x, int y);
    CONFIG_OPTION(ObjectType, GameObjectType, GameObjectType type);
    CONFIG_OPTION(BatchMode, int, int mode);
    CONFIG_OPTION(DisableBatchRender, bool);
    CONFIG_OPTION(FramesCount, int, int frames);
    CONFIG_OPTION(FrameTime, float, float time);
    CONFIG_OPTION(GlowColor, cocos2d::ccColor3B, GLubyte r, GLubyte g, GLubyte b);
    CONFIG_OPTION(ParticleColor, cocos2d::ccColor3B, GLubyte r, GLubyte g, GLubyte b);
    CONFIG_OPTION(ParticleOpacity, GLubyte, GLubyte opacity);
    CONFIG_OPTION(ParticleBlending, bool, bool blending);
    CONFIG_OPTION(EditorPriority, int, int priority);
    #undef CONFIG_OPTION

    std::string getID() const;
    std::string getModID() const;
    int getObjectID() const;

    std::string getMainSprite() const;
    std::string getDetailSprite() const;
    std::string getGlowSprite() const;

    bool hasMainSprite() const;
    bool hasDetailSprite() const;
    bool hasGlowSprite() const;

    bool isCustomBatch() const;
    bool hasCustomAnimation() const;

    CustomObjectConfig&& setMainSprite(std::string frame, int offsetX, int offsetY, int width, int height);
    CustomObjectConfig&& setMainSprite(std::string frame, int width, int height);
    CustomObjectConfig&& setMainSprite(std::string frame, int size);
    CustomObjectConfig&& setMainSprite(std::string frame, bool sheet = true);

    CustomObjectConfig&& setDetailSprite(std::string frame, int offsetX, int offsetY, int width, int height);
    CustomObjectConfig&& setDetailSprite(std::string frame, int width, int height);
    CustomObjectConfig&& setDetailSprite(std::string frame, int size);
    CustomObjectConfig&& setDetailSprite(std::string frame, bool sheet = true);

    CustomObjectConfig&& setGlowSprite(std::string frame, int offsetX, int offsetY, int width, int height);
    CustomObjectConfig&& setGlowSprite(std::string frame, int width, int height);
    CustomObjectConfig&& setGlowSprite(std::string frame, int size);
    CustomObjectConfig&& setGlowSprite(std::string frame, bool sheet = true);

    CustomObjectConfig&& onSetupCustomObject(SetupObjectCallback);
    CustomObjectConfig&& onResetCustomObject(ResetObjectCallback);
    CustomObjectConfig&& onActivateCustomObject(ActivateObjectCallback);

    CustomObjectConfig&& onEditObject(EditObjectCallback);
    CustomObjectConfig&& onEditSpecial(EditObjectCallback);

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;

    GameObject* createCustomObject() const;
    static CustomObjectConfig* registerConfig(std::string_view, ObjectConstructor);

    template <class, StringConcatModIDSlash> friend class RegisterCustomObject;
    friend class CustomObjectsManager, class CustomObjectInterface;
};

// Dummy class for determining how an object was registered
class RegisteredObjectByClass {};

template <class ObjectType, StringConcatModIDSlash StringID>
class RegisterCustomObject : public RegisteredObjectByClass {
    static inline struct {
        CustomObjectConfig* config = CustomObjectConfig::registerConfig(StringID.buffer, [](){ return new ObjectType(); });
        bool initialized = [](){ ObjectType::onRegisterConfig(std::move(*registration.config)); return true; }();
    } registration;
    static inline auto registrationRef = &registration;

    static void onRegisterConfig(CustomObjectConfig&&) {}
    static void onEditObject(ObjectType*, cocos2d::CCArray*) {}
    static void onEditSpecial(ObjectType*, cocos2d::CCArray*) {}
protected:
    using ObjectConstructor = CustomObjectConfig::ObjectConstructor;
    using EditObjectCallback = CustomObjectConfig::EditObjectCallback;

    static const CustomObjectConfig* getStaticConfig() { return registration.config; }
    static bool isInitialized() { return registration.initialized; }
};