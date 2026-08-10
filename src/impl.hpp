#pragma once
#include <rect_structs.h>

#include "config/CustomSpriteConfig.hpp"
#include "CustomObjectConfig.hpp"
#include "CustomObjectBase.hpp"

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

struct ICustomObjectBase::Impl {
    const CustomObjectConfig* m_config;
};