#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

// Config defaults
#define BOX_SIZE_DEFAULT CCSize(0, 0)
#define BOX_OFFSET_DEFAULT CCPoint(0, 0)
#define BOX_RADIUS_DEFAULT 15
#define SPRITE_SIZE_DEFAULT CCSize(30, 30)
#define CREATE_OFFSET_DEFAULT CCPoint(0, 0)
#define OBJECT_TYPE_DEFAULT (GameObjectType)(-1)
#define PARENT_MODE_DEFAULT 10

struct CustomObjectConfig {
public:
    int id;
    std::string mod;
    std::function<GameObject*(CustomObjectConfig)> createFunction;

    // Sprite frame names
    std::string frame;
    std::string sourceFrame;
    std::string detailFrame;
    std::string detailSourceFrame;
    std::string glowFrame;
    std::string glowSourceFrame;

    // Config variables
    CCSize spriteSize = SPRITE_SIZE_DEFAULT;
    CCSize boxSize = BOX_SIZE_DEFAULT;
    CCPoint boxOffset = BOX_OFFSET_DEFAULT;
    int boxRadius = BOX_RADIUS_DEFAULT;
    CCPoint createOffset = CREATE_OFFSET_DEFAULT;
    GameObjectType objectType = OBJECT_TYPE_DEFAULT;
    int parentMode = PARENT_MODE_DEFAULT;

    // Set config options
    CustomObjectConfig& setSpriteSize(int w, int h) { spriteSize = CCSize(w, h); return *this; }
    CustomObjectConfig& setBoxSize(int w, int h) { boxSize = CCSize(w, h); return *this; }
    CustomObjectConfig& setBoxOffset(int x, int y) { boxOffset = CCPoint(x, y); return *this; }
    CustomObjectConfig& setBoxRadius(int radius) { boxRadius = radius; return *this; }
    CustomObjectConfig& setCreateOffset(int x, int y) { createOffset = CCPoint(x, y); return *this; }
    CustomObjectConfig& setObjectType(GameObjectType type) { objectType = type; return *this; }
    CustomObjectConfig& setCustomRender(int parent = 4) { frame = sourceFrame; detailFrame = detailSourceFrame; parentMode = parent; return *this; }

    // Config constructors
    CustomObjectConfig() : frame(""), sourceFrame(""), detailFrame(""), detailSourceFrame(""), id(0), spriteSize(SPRITE_SIZE_DEFAULT) {}
    CustomObjectConfig(std::string frame, std::string detail, std::string mod, int id, std::function<GameObject*(CustomObjectConfig)> create) : sourceFrame(frame), detailSourceFrame(detail), mod(mod), id(id), spriteSize(SPRITE_SIZE_DEFAULT), createFunction(create) {}

    // Create an instance of the custom game object represented by this config struct
    GameObject* create() {
        GameObject* obj = createFunction(*this);

        // Setup custom object values
        obj->m_parentMode = 10;
        obj->m_objectID = id;

        // Apply object config
        if (boxSize != BOX_SIZE_DEFAULT) { obj->m_width = boxSize.width; obj->m_height = boxSize.height; }
        if (boxOffset != BOX_OFFSET_DEFAULT) obj->m_customBoxOffset = boxOffset;
        if (boxRadius != BOX_RADIUS_DEFAULT) obj->m_objectRadius = boxRadius;
        if (createOffset != CREATE_OFFSET_DEFAULT) obj->m_unk464 = createOffset;
        if (objectType != OBJECT_TYPE_DEFAULT) obj->m_objectType = objectType;
        if (parentMode != PARENT_MODE_DEFAULT) obj->m_parentMode = parentMode;

        return obj;
    } // create

    // Create the custom object frame names based on the mod and sprite size
    void generateFrames() {
        if (parentMode != PARENT_MODE_DEFAULT) {
            frame = sourceFrame;
            detailFrame = detailSourceFrame;
            return;
        } // if

        if (!sourceFrame.empty()) {
            auto frameName = sourceFrame.substr(sourceFrame.find("/") + 1);
            frame = fmt::format("custom-objects/{}/{}/{}/{}", mod, spriteSize.width, spriteSize.height, frameName);
        } // if

        if(!detailSourceFrame.empty()) {
            auto detailName = detailSourceFrame.substr(detailSourceFrame.find("/") + 1);
            detailFrame = fmt::format("custom-objects/{}/{}/{}/{}", mod, spriteSize.width, spriteSize.height, detailName);
        } // if
    } // generateFrames
};