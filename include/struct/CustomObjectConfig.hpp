#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

// Config defaults
#define BOX_SIZE_DEFAULT CCSize(0, 0)
#define BOX_OFFSET_DEFAULT CCPoint(0, 0)
#define SPRITE_SIZE_DEFAULT CCSize(30, 30)
#define OBJECT_TYPE_DEFAULT (GameObjectType)(-1)
#define CUSTOM_RENDER_DEFAULT false

struct CustomObjectConfig {
    int id;
    std::string frame;
    std::string sourceFrame;
    std::function<GameObject*(CustomObjectConfig)> createFunction;

    GameObject* create() { return createFunction(*this); }

    CustomObjectConfig() : frame(""), sourceFrame(""), id(0), spriteSize(SPRITE_SIZE_DEFAULT) {}
    CustomObjectConfig(std::string frame, std::string mod, int id, CCSize size, std::function<GameObject*(CustomObjectConfig)> create) {
        auto name = frame.substr(frame.find("/") + 1);
        this->frame = fmt::format("custom-objects/{}/{}/{}/{}", mod, size.width, size.height, name);

        this->id = id;
        this->sourceFrame = frame;
        this->spriteSize = size;
        this->createFunction = create;
    } // CustomObjectConfig

    // Config variables
    CCSize boxSize = BOX_SIZE_DEFAULT;
    CCPoint boxOffset = BOX_OFFSET_DEFAULT;
    CCSize spriteSize = SPRITE_SIZE_DEFAULT;
    GameObjectType objectType = OBJECT_TYPE_DEFAULT;
    bool customRender = CUSTOM_RENDER_DEFAULT;

    CustomObjectConfig& setSize(int w, int h) { spriteSize = CCSize(w, h); return *this; }
    CustomObjectConfig& setBoxSize(int w, int h) { boxSize = CCSize(w, h); return *this; }
    CustomObjectConfig& setBoxOffset(int x, int y) { boxOffset = CCPoint(x, y); return *this; }
    CustomObjectConfig& setObjectType(GameObjectType type) { objectType = type; return *this; }
    CustomObjectConfig& setCustomRender(bool render) { customRender = render; return *this; }

    void applyBoxSize(GameObject* obj) { if (boxSize != BOX_SIZE_DEFAULT) { obj->m_width = boxSize.width; obj->m_height = boxSize.height; } }
    void applyBoxOffset(GameObject* obj) { if (boxOffset != BOX_OFFSET_DEFAULT) { obj->m_customBoxOffset = boxOffset; } }
    void applyObjectType(GameObject* obj) { if (objectType != OBJECT_TYPE_DEFAULT) obj->m_objectType = objectType; }
    void applyCustomRender(GameObject* obj) { if (customRender != CUSTOM_RENDER_DEFAULT) obj->m_parentMode = 4; frame = sourceFrame; }
};