#pragma once
#include "CustomObjectConfig.hpp"
#include "CustomObjectProperty.hpp"

class CUSTOM_OBJECTS_DLL CustomObjectInterface {
    struct Impl;
    std::unique_ptr<Impl> m_impl;
    friend class CustomObjectConfig;

    template <class BaseType, typename Enable>
    requires (std::derived_from<BaseType, GameObject> && !std::derived_from<BaseType, RegisteredObjectByClass>)
    friend class CustomObjectBase;

    void setupCustomObject(GameObject*) const;
    void resetCustomObject(GameObject*) const;
    void activateCustomObject(GameObject*, GJBaseGameLayer*, PlayerObject*) const;

    void bindObjectProperty(int, std::unique_ptr<ObjectPropertyInterface>&&);
    void setupObjectProperties(gd::vector<gd::string>&, gd::vector<void*>&);
    std::string getCustomSaveString() const;

    virtual GameObject* gameObject() = 0;
protected:
    virtual bool init() = 0;
    const CustomObjectConfig&& getConfig() const;
public:
    CustomObjectInterface();
    virtual ~CustomObjectInterface();
};

template <class BaseType, typename Enable = void>
requires (std::derived_from<BaseType, GameObject> && !std::derived_from<BaseType, RegisteredObjectByClass>)
class CustomObjectBase : public CustomObjectInterface, public BaseType {
public:
    bool init() override {
        const auto&& config = getConfig();
        if (!BaseType::init(config.getMainSprite().c_str())) return false;

        // Add detail sprite
        if (config.hasDetailSprite()) {
            this->addCustomColorChild(config.getDetailSprite());
        }

        // Add object glow
        if (!this->m_editorEnabled && !this->m_hasNoGlow && config.hasGlowSprite()) {
            this->createGlow(config.getGlowSprite());
        }

        return true;
    }

    virtual void setupCustomObject() {
        CustomObjectInterface::setupCustomObject(this);
    }

    virtual void resetCustomObject() {
        CustomObjectInterface::resetCustomObject(this);
    }

    virtual void activateCustomObject(GJBaseGameLayer* level, PlayerObject* player) {
        CustomObjectInterface::activateCustomObject(this, level, player);
    }

    template <typename T>
    void bindObjectProperty(int key, T& value, geode::Function<bool()> cond = nullptr) {
        CustomObjectInterface::bindObjectProperty(key, std::make_unique<ObjectProperty<T>>(value, std::move(cond)));
    }

    void customObjectSetup(gd::vector<gd::string>& propValues, gd::vector<void*>& propIsPresent) override final {
        BaseType::customObjectSetup(propValues, propIsPresent);
        CustomObjectInterface::setupObjectProperties(propValues, propIsPresent);
        setupCustomObject();
    }

    gd::string getSaveString(GJBaseGameLayer* p0) override final {
        std::string saveString = BaseType::getSaveString(p0);
        return saveString + CustomObjectInterface::getCustomSaveString();
    }

    void addMainSpriteToParent(bool p0) override {
        bool disableBlend = (this->m_parentMode == 4);
        this->m_colorZLayerRelated = this->m_colorZLayerRelated || disableBlend;

        BaseType::addMainSpriteToParent(p0);

        this->m_shouldBlendBase = this->m_shouldBlendBase && !disableBlend;
        this->m_shouldBlendDetail = this->m_shouldBlendDetail && !disableBlend;
    }

    void activateObject() override {
        this->m_unk3ee = false; // idek what this is lol
        if (!this->m_isActivated && !this->m_isDisabled) {
            this->m_isActivated = true;
            if (this->m_isInvisible) return;

            this->setVisible(true);
            this->addMainSpriteToParent(false);
            this->addColorSpriteToParent(false);

            if (this->m_glowSprite) {
                auto zLayer = (this->m_zLayer == ZLayer::Default) ? this->m_defaultZLayer : this->m_zLayer;
                auto mode = (this->m_parentMode == 4 || this->m_parentMode == CUSTOM_PARENT_MODE) ? this->m_parentMode : 5;
                auto parent = this->parentForZLayer((int)zLayer, true, mode);
                parent->addChild(this->m_glowSprite, -1000);
            }
        }
    }

    void setVisible(bool visible) override {
        BaseType::setVisible(visible);
        if (!visible || !this->m_particle) return;

        this->updateParticleColor(getConfig().getParticleColor());
        this->updateParticleOpacity(getConfig().getParticleOpacity());

        if (!getConfig().getParticleBlending()) {
            this->m_particle->setBlendFunc({GL_ONE, GL_ZERO});
        }
    }

    void firstSetup() override {
        BaseType::firstSetup();
        setupCustomObject();
    }

    void resetObject() override {
        BaseType::resetObject();
        resetCustomObject();
    }

    GameObject* gameObject() override final {
        return this;
    }
};

template <class BaseType>
requires (std::derived_from<BaseType, GameObject> && !std::derived_from<BaseType, RegisteredObjectByClass>)
class CustomObjectBase<BaseType, std::enable_if_t<std::is_base_of_v<CustomObjectInterface, BaseType>>> : public BaseType {};

#define $object(NAME, BASE) NAME : public CustomObjectBase<BASE>
#define $registerObject(NAME, BASE) NAME final : public CustomObjectBase<BASE>, RegisterCustomObject<NAME, #NAME>