#pragma once

#include "CustomObjectBase.hpp"
#include "CustomObjectConfig.hpp"
#include "config/CustomSpriteConfig.hpp"

struct CustomObjectConfig::Impl {
    Impl(std::string_view id, int objectID, ObjectConstructor ctor);

    std::string m_id;
    std::string m_mod;
    int m_objectID;

    ObjectConstructor m_ctor = nullptr;
    EditObjectCallback m_editObject = nullptr;
    EditObjectCallback m_editSpecial = nullptr;

    SetupObjectCallback m_setupCallback = nullptr;
    ResetObjectCallback m_resetCallback = nullptr;
    ActivateObjectCallback m_activateCallback = nullptr;

    std::unique_ptr<CustomSpriteConfig> m_mainSprite = nullptr;
    std::unique_ptr<CustomSpriteConfig> m_detailSprite = nullptr;
    std::unique_ptr<CustomSpriteConfig> m_glowSprite = nullptr;

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

struct SheetInfo final {
    const class CustomSpriteConfig* m_sprite;
    rectpack2D::rect_wh m_offset;
    rectpack2D::rect_wh m_size;
    rectpack2D::rect_xywh m_trim;
    rectpack2D::rect_xywhf m_rect;

    auto& get_rect() { return m_rect; }
    const auto& get_rect() const { return m_rect; }
};

struct CustomSpriteConfig::Impl {
    CustomObjectConfig* m_object;
    std::string m_frameName;
    std::string m_sourceFrame;
    rectpack2D::rect_wh m_offset;
    rectpack2D::rect_wh m_size;
    bool m_customSprite;
};

struct ICustomObjectBase::Impl {
    const CustomObjectConfig* m_config;
};