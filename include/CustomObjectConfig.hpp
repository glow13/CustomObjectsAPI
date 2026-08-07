#pragma once

#define CONFIG_OPTION(name, type, ...) type get##name() const; CustomObjectConfig&& set##name(__VA_ARGS__);

using geode::geode_internal::StringConcatModIDSlash;

class CustomObjectConfig final {
public:
    using ObjectConstructor = GameObject*(*)(const CustomObjectConfig*);
    using EditObjectCallback = void(*)(GameObject*, cocos2d::CCArray*);

    CustomObjectConfig(std::string_view, int, ObjectConstructor);
    ~CustomObjectConfig();

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

    std::string getID() const;
    std::string getModID() const;
    int getObjectID() const;

    std::string getMainSprite() const;
    std::string getDetailSprite() const;
    std::string getGlowSprite() const;

    bool hasMainSprite() const;
    bool hasDetailSprite() const;
    bool hasGlowSprite() const;

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

    CustomObjectConfig&& onEditObjectButton(EditObjectCallback);
    CustomObjectConfig&& onEditSpecialButton(EditObjectCallback);

    bool hasEditObjectCallback() const;
    bool hasEditSpecialCallback() const;

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;

    GameObject* createCustomObject() const;
    static CustomObjectConfig* registerConfig(std::string_view, ObjectConstructor);

    void customEditObject(GameObject*, cocos2d::CCArray*) const;
    void customEditSpecial(GameObject*, cocos2d::CCArray*) const;

    template <class, StringConcatModIDSlash> friend class ConfigGameObject;
    friend class CustomObjectsManager;
};

template <class ObjectType, StringConcatModIDSlash StringID>
class ConfigGameObject {
    static inline struct {
        CustomObjectConfig* config = CustomObjectConfig::registerConfig(StringID.buffer, (ObjectConstructor)ObjectType::createWithConfig);
        bool initialized = [](){ ObjectType::onRegisterConfig((CustomObjectConfig&&)*registration.config); return true; }();
    } registration;
    static inline auto registrationRef = &registration;
    static void onRegisterConfig(CustomObjectConfig&&) {}
protected:
    using ObjectConstructor = CustomObjectConfig::ObjectConstructor;
    using EditObjectCallback = CustomObjectConfig::EditObjectCallback;

    static const CustomObjectConfig* getConfig() { return registration.config; }
    static bool isInitialized() { return registration.initialized; }
public:
    static ObjectType* createWithConfig(const CustomObjectConfig* config) {
        auto obj = new ObjectType();
        if (obj->ObjectType::init(config)) {
            obj->autorelease();
            return obj;
        }
        delete obj;
        return nullptr;
    }
};

#undef CONFIG_OPTION