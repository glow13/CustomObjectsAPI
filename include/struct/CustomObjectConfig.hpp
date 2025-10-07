#pragma once
#include <Geode/Geode.hpp>

#include "CustomSpriteConfig.hpp"

using namespace geode::prelude;

// Config defaults
#define BOX_SIZE_DEFAULT CCSize(0, 0)
#define BOX_OFFSET_DEFAULT CCPoint(0, 0)
#define BOX_RADIUS_DEFAULT 0
#define CREATE_OFFSET_DEFAULT CCPoint(0, 0)
#define OBJECT_TYPE_DEFAULT (GameObjectType)(-1)
#define PARENT_MODE_DEFAULT 10
#define FRAMES_COUNT_DEFAULT 0
#define FRAME_TIME_DEFAULT 1

struct CustomObjectConfig {
public:
    int id;
    std::string mod;
    std::function<GameObject*(CustomObjectConfig)> createFunction;

    // Sprite frame variables
    CustomSpriteConfig mainSprite;
    CustomSpriteConfig detailSprite;
    CustomSpriteConfig glowSprite;

    // Config variables
    CCSize boxSize = BOX_SIZE_DEFAULT;
    CCPoint boxOffset = BOX_OFFSET_DEFAULT;
    int boxRadius = BOX_RADIUS_DEFAULT;
    CCPoint createOffset = CREATE_OFFSET_DEFAULT;
    GameObjectType objectType = OBJECT_TYPE_DEFAULT;
    int parentMode = PARENT_MODE_DEFAULT;
    int framesCount = FRAMES_COUNT_DEFAULT;
    float frameTime = FRAME_TIME_DEFAULT;

    CustomObjectConfig() {}
    CustomObjectConfig(std::string mod, int id, std::function<GameObject*(CustomObjectConfig)> create);

    CustomObjectConfig& setMainSprite(std::string frame, int w, int h);
    CustomObjectConfig& setDetailSprite(std::string frame, int w, int h);
    CustomObjectConfig& setGlowSprite(std::string frame, int w, int h);

    CustomObjectConfig& setBoxSize(int w, int h);
    CustomObjectConfig& setBoxOffset(int x, int y);
    CustomObjectConfig& setBoxRadius(int radius);
    CustomObjectConfig& setCreateOffset(int x, int y);
    CustomObjectConfig& setObjectType(GameObjectType type);
    CustomObjectConfig& setCustomRender(int parent = 4);
    CustomObjectConfig& setFramesCount(int frames);
    CustomObjectConfig& setFrameTime(float time);

    // Inline helper functions for sprite frames
    inline CustomObjectConfig& setMainSprite(std::string frame, int size) { return setMainSprite(frame, size, size); }
    inline CustomObjectConfig& setMainSprite(std::string frame) { return setMainSprite(frame, 0, 0); }
    inline CustomObjectConfig& setDetailSprite(std::string frame, int size) { return setDetailSprite(frame, size, size); }
    inline CustomObjectConfig& setDetailSprite(std::string frame) { return setDetailSprite(frame, 0, 0); }
    inline CustomObjectConfig& setGlowSprite(std::string frame, int size) { return setGlowSprite(frame, size, size); }
    inline CustomObjectConfig& setGlowSprite(std::string frame) { return setGlowSprite(frame, 0, 0); }

    inline bool isCustomRender() const { return parentMode != PARENT_MODE_DEFAULT; }
    inline bool hasCustomAnimation() const { return framesCount != FRAMES_COUNT_DEFAULT && mainSprite.isAnimationFrame(); }

    // Create an instance of the custom game object represented by this config struct
    GameObject* create();
};