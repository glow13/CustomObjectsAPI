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
    std::string detailFrame;
    std::string detailSourceFrame;
    std::function<GameObject*(CustomObjectConfig)> createFunction;

    CustomObjectConfig() : frame(""), sourceFrame(""), detailFrame(""), detailSourceFrame(""), id(0), spriteSize(SPRITE_SIZE_DEFAULT) {}
    CustomObjectConfig(std::string frame, std::string detail, std::string mod, int id, std::function<GameObject*(CustomObjectConfig)> create) : id(id), spriteSize(SPRITE_SIZE_DEFAULT), createFunction(create) {
        if (!frame.empty()) {
            auto frameName = frame.substr(frame.find("/") + 1);
            this->frame = fmt::format("custom-objects/{}/{}/{}/{}", mod, spriteSize.width, spriteSize.height, frameName);
            this->sourceFrame = frame;
        } // if

        if(!detail.empty()) {
            auto detailName = detail.substr(detail.find("/") + 1);
            this->detailFrame = fmt::format("custom-objects/{}/{}/{}/{}", mod, spriteSize.width, spriteSize.height, detailName);
            this->detailSourceFrame = detail;
        } // if
    } // CustomObjectConfig

    void regenerateFrames() {
        if (customRender) return;
        if (spriteSize == SPRITE_SIZE_DEFAULT) return;

        if (!frame.empty()) {
            auto frameName = sourceFrame.substr(sourceFrame.find("/") + 1);
            auto prefix = frame.substr(0, frame.find("/", frame.find("/") + 1));
            frame = fmt::format("{}/{}/{}/{}", prefix, spriteSize.width, spriteSize.height, frameName);
        } // if

        if(!detailFrame.empty()) {
            auto detailName = detailSourceFrame.substr(detailSourceFrame.find("/") + 1);
            auto prefix = detailFrame.substr(0, detailFrame.find("/", detailFrame.find("/") + 1));
            detailFrame = fmt::format("{}/{}/{}/{}", prefix, spriteSize.width, spriteSize.height, detailName);
        } // if
    } // regenerateFrames

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
    CustomObjectConfig& useCustomRender() { customRender = true; frame = sourceFrame; detailFrame = detailSourceFrame; return *this; }

    void applyBoxSize(GameObject* obj) { if (boxSize != BOX_SIZE_DEFAULT) { obj->m_width = boxSize.width; obj->m_height = boxSize.height; } }
    void applyBoxOffset(GameObject* obj) { if (boxOffset != BOX_OFFSET_DEFAULT) { obj->m_customBoxOffset = boxOffset; } }
    void applyObjectType(GameObject* obj) { if (objectType != OBJECT_TYPE_DEFAULT) obj->m_objectType = objectType; }
    void applyCustomRender(GameObject* obj) { if(customRender != CUSTOM_RENDER_DEFAULT) obj->m_parentMode = 4; }

    GameObject* create() { return createFunction(*this); }
};