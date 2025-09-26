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
        this->addCustomChild(config.frame.c_str(), CCPoint(0, 0), 0);

        if (!config.detailFrame.empty()) {
            auto detail = this->addCustomColorChild(config.detailFrame);
            this->addInternalCustomColorChild(config.detailFrame.c_str(), CCPoint(0, 0), 0);
            detail->setDontDraw(true);
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