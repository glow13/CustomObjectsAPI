#pragma once
#include <Geode/Geode.hpp>

#include "CustomObjectUtils.hpp"

using namespace geode::prelude;

template <class ObjectType>
class CustomRotateObjectBase : public CustomObjectUtils<ObjectType, EnhancedGameObject> {
public:
    bool init(const CustomObjectConfig& config) {
       if (!this->commonSetup(config, false)) return false;

        this->setDontDraw(true);
        this->addCustomChild(config.mainSprite.frame, CCPoint(0, 0), 0);

        if (config.detailSprite) {
            auto detail = this->addCustomColorChild(config.detailSprite.frame);
            this->addInternalCustomColorChild(config.detailSprite.frame, CCPoint(0, 0), 0);
            detail->setDontDraw(true);
        } // if

        if (config.glowSprite && !this->m_editorEnabled && !this->m_hasNoGlow) {
            this->createGlow(config.glowSprite.frame);
            auto glow = this->addInternalGlowChild(config.glowSprite.frame, CCPoint(0, 0));
            this->m_glowSprite->setDontDraw(true);
        } // if

        this->m_objectType = GameObjectType::Decoration;
        this->m_objectRadius = this->m_width / 2;
        this->m_hasCustomRotation = true;
        this->m_rotationDelta = 360;
        this->m_rotationAngle = 360;
        this->m_customGlowColor = true; // glow color lbg

        return true;
    } // init
};

class CustomRotateObject : public CustomRotateObjectBase<CustomRotateObject> {};