#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

enum Config : int {
    SPRITE_SIZE,
    BOX_SIZE,
    BOX_OFFSET,
    BOX_RADIUS,
    OBJECT_TYPE,
    CUSTOM_RENDER,
    PARENT_MODE
};

struct CustomObjectConfig {
    int id;
    std::string mod;
    std::string frame;
    std::string sourceFrame;
    std::string detailFrame;
    std::string detailSourceFrame;
    std::function<GameObject*(CustomObjectConfig)> createFunction;

    CustomObjectConfig() : frame(""), sourceFrame(""), detailFrame(""), detailSourceFrame(""), id(0), spriteSizeConfig(CCSize(30, 30)) {}
    CustomObjectConfig(std::string frame, std::string detail, std::string mod, int id, std::function<GameObject*(CustomObjectConfig)> create) : sourceFrame(frame), detailSourceFrame(detail), mod(mod), id(id), spriteSizeConfig(CCSize(30, 30)), createFunction(create) {}

    GameObject* create() { return createFunction(*this); }

    void generateFrames() {
        if (customRenderConfig) {
            frame = sourceFrame;
            detailFrame = detailSourceFrame;
            return;
        } // if

        if (!sourceFrame.empty()) {
            auto frameName = sourceFrame.substr(sourceFrame.find("/") + 1);
            frame = fmt::format("custom-objects/{}/{}/{}/{}", mod, spriteSizeConfig.width, spriteSizeConfig.height, frameName);
        } // if

        if(!detailSourceFrame.empty()) {
            auto detailName = detailSourceFrame.substr(detailSourceFrame.find("/") + 1);
            detailFrame = fmt::format("custom-objects/{}/{}/{}/{}", mod, spriteSizeConfig.width, spriteSizeConfig.height, detailName);
        } // if
    } // generateFrames

    // Config defaults
    #define BOX_SIZE_DEFAULT CCSize(0, 0)
    #define BOX_OFFSET_DEFAULT CCPoint(0, 0)
    #define BOX_RADIUS_DEFAULT 15
    #define SPRITE_SIZE_DEFAULT CCSize(30, 30)
    #define OBJECT_TYPE_DEFAULT (GameObjectType)(-1)
    #define CUSTOM_RENDER_DEFAULT false
    #define PARENT_MODE_DEFAULT 4

    // Config variables
    CCSize spriteSizeConfig = SPRITE_SIZE_DEFAULT;
    CCSize boxSizeConfig = BOX_SIZE_DEFAULT;
    CCPoint boxOffsetConfig = BOX_OFFSET_DEFAULT;
    int boxRadiusConfig = BOX_RADIUS_DEFAULT;
    GameObjectType objectTypeConfig = OBJECT_TYPE_DEFAULT;
    bool customRenderConfig = CUSTOM_RENDER_DEFAULT;
    int parentModeConfig = PARENT_MODE_DEFAULT;

    CustomObjectConfig& spriteSize(int w, int h) { spriteSizeConfig = CCSize(w, h); return *this; }
    CustomObjectConfig& boxSize(int w, int h) { boxSizeConfig = CCSize(w, h); return *this; }
    CustomObjectConfig& boxOffset(int x, int y) { boxOffsetConfig = CCPoint(x, y); return *this; }
    CustomObjectConfig& boxRadius(int radius) { boxRadiusConfig = radius; return *this; }
    CustomObjectConfig& objectType(GameObjectType type) { objectTypeConfig = type; return *this; }
    CustomObjectConfig& customRender(int parent = 4) { customRenderConfig = true; frame = sourceFrame; detailFrame = detailSourceFrame; parentModeConfig = parent; return *this; }

    bool applyConfigOption(GameObject* obj, Config option) {
        switch (option) {
            case Config::BOX_SIZE:
            if (boxSizeConfig != BOX_SIZE_DEFAULT) {
                obj->m_width = boxSizeConfig.width;
                obj->m_height = boxSizeConfig.height;
            } return true;

            case Config::BOX_OFFSET:
            if (boxOffsetConfig != BOX_OFFSET_DEFAULT) {
                obj->m_customBoxOffset = boxOffsetConfig;
            } return true;

            case Config::BOX_RADIUS:
            if (boxRadiusConfig != BOX_RADIUS_DEFAULT) {
                obj->m_objectRadius = boxRadiusConfig;
            } return true;

            case Config::OBJECT_TYPE:
            if (objectTypeConfig != OBJECT_TYPE_DEFAULT) {
                obj->m_objectType = objectTypeConfig;
            } return true;

            case Config::CUSTOM_RENDER:
            if(customRenderConfig != CUSTOM_RENDER_DEFAULT) {
                obj->m_parentMode = parentModeConfig;
            } return true;

            default: return false;
        } // switch
    } // applyConfigOption
};