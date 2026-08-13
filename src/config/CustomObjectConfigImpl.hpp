#pragma once
#include "CustomObjectConfig.hpp"

class CustomSpriteConfig;

struct CustomObjectConfig::Impl {
    Impl(std::string_view id, int objectID, ObjectConstructor ctor);

    std::string m_id;
    std::string m_mod;
    int m_objectID;

    ObjectConstructor m_ctor = nullptr;
    EditObjectCallback m_editObject = nullptr;
    EditObjectCallback m_editSpecial = nullptr;

    SetupObjectCallback m_setupObject = nullptr;
    ResetObjectCallback m_resetObject = nullptr;
    ActivateObjectCallback m_activateObject = nullptr;

    std::unique_ptr<CustomSpriteConfig> m_mainSprite;
    std::unique_ptr<CustomSpriteConfig> m_detailSprite;
    std::unique_ptr<CustomSpriteConfig> m_glowSprite;

    cocos2d::CCSize m_boxSize;
    cocos2d::CCPoint m_boxOffset;
    int m_boxRadius;
    cocos2d::CCPoint m_objectOffset;
    GameObjectType m_objectType;
    int m_batchMode;
    bool m_disableBatch;
    int m_framesCount;
    float m_frameTime;
    cocos2d::ccColor3B m_glowColor;
    cocos2d::ccColor3B m_particleColor;
    GLubyte m_particleOpacity;
    bool m_particleBlending;
    int m_editorPriority;
};