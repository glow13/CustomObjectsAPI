#pragma once

#include "CustomObjectBase.hpp"

template <class ObjectType>
class $base(CustomRotateObject, EnhancedGameObject) {
protected:
    using CustomObjectBase = CustomObjectBase<ObjectType, EnhancedGameObject>::Base;
public:
    bool init(const CustomObjectConfig<ObjectType>* config) {
        if (!EnhancedGameObject::init(config->mainSprite)) return false;

        this->setDontDraw(true);
        this->addCustomChild(config->mainSprite, CCPoint(0, 0), 0);

        if (config->detailSprite) {
            auto detail = this->addCustomColorChild(config->detailSprite);
            this->addInternalCustomColorChild(config->detailSprite, CCPoint(0, 0), 0);
            detail->setDontDraw(true);
        } // if

        if (config->glowSprite && !this->m_editorEnabled && !this->m_hasNoGlow) {
            this->createGlow(config->glowSprite);
            auto glow = this->addInternalGlowChild(config->glowSprite, CCPoint(0, 0));
            this->m_glowSprite->setDontDraw(true);
        } // if

        this->m_objectType = GameObjectType::Decoration;
        this->m_objectRadius = this->m_width / 2;
        this->m_hasCustomRotation = true;
        this->m_rotationDelta = 360;
        this->m_rotationAngle = 360;
        this->m_glowColorIsLBG = true;

        return true;
    } // init
};

class $generic(CustomRotateObject);