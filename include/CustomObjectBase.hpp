#pragma once
#include "CustomObjectConfig.hpp"

class ICustomObjectBase {
    struct Impl;
    std::unique_ptr<Impl> m_impl;

    void addConfig(const CustomObjectConfig*);
    friend class CustomObjectConfig;
public:
    ICustomObjectBase();
    ~ICustomObjectBase();
protected:
    void setupCustomObject(GameObject*) const;
    void resetCustomObject(GameObject*) const;
    void activateCustomObject(GameObject*, GJBaseGameLayer*, PlayerObject*) const;
};

template <class BaseType>
requires std::derived_from<BaseType, GameObject>
class CustomObjectBase : ICustomObjectBase, public BaseType {
public:
    virtual bool init(const CustomObjectConfig&& config) {
        if (!BaseType::init(config.getMainSprite().c_str())) return false;

        // Add sprites to the custom object
        if (!config.hasMainSprite()) this->setDontDraw(true);
        if (config.hasDetailSprite()) this->addCustomColorChild(config.getDetailSprite());

        // Add glow to the custom object
        if (this->m_editorEnabled || this->m_hasNoGlow) return true;
        if (config.hasGlowSprite()) this->createGlow(config.getGlowSprite());

        return true;
    }

    virtual void setupCustomObject() { ICustomObjectBase::setupCustomObject(this); }
    virtual void resetCustomObject() { ICustomObjectBase::resetCustomObject(this); }
    virtual void activateCustomObject(GJBaseGameLayer* level, PlayerObject* player) { ICustomObjectBase::activateCustomObject(this, level, player); }
};