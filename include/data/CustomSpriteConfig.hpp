#pragma once
#include <Geode/Geode.hpp>

#ifdef GEODE_IS_WINDOWS
    #ifdef CUSTOM_OBJECTS_EXPORTING
        #define CUSTOM_OBJECTS_DLL __declspec(dllexport)
    #else
        #define CUSTOM_OBJECTS_DLL __declspec(dllimport)
    #endif
#else
    #define CUSTOM_OBJECTS_DLL __attribute__((visibility("default")))
#endif
#if (__INTELLISENSE__ != 1 && !defined(__CLION_IDE__)) || CUSTOM_OBJECTS_EXPORTING
    #define CUSTOM_OBJECTS_INTELLISENSE_DISABLED
#endif

class CustomObjectsMod;
class CustomObjectConfigBase;

/**
 * A struct representing a custom sprite. This struct can be configured to change properties of your custom sprite.
 * 
 * You can configure this through an already registered CustomObjectConfig, or you can register one yourself:
 * 
 * auto sprite = CustomObjectsAPI::registerCustomSprite(...);
 * 
 * A registered CustomSpriteConfig is added to the custom object spritesheet.
 * Any sprite you want to use in a custom object must be added to the spritesheet so that batch rendering can work correctly.
 */
struct CUSTOM_OBJECTS_DLL CustomSpriteConfig final {
#ifdef CUSTOM_OBJECTS_INTELLISENSE_DISABLED
private:
    CustomObjectsMod* mod;
    CustomObjectConfigBase* object;

    std::string frameName;
    std::string sourceFrame;
    cocos2d::CCPoint offset;
    cocos2d::CCSize size;

    void generateFrame();

    friend class CustomObjectsManager;
    friend class CustomSpritesManager;
    friend struct CustomObjectsMod;
    friend struct CustomObjectConfigBase;

    CustomSpriteConfig() : frameName(""), sourceFrame(""), offset(cocos2d::CCPoint(0, 0)), size(cocos2d::CCSize(0, 0)) {}
    CustomSpriteConfig(CustomObjectsMod* mod, CustomObjectConfigBase* object, std::string frame, int offX, int offY, int sizeW, int sizeH);
public:
    CustomSpriteConfig& operator=(const CustomSpriteConfig&);
#endif
public:

    /**
     * Sets the frame name, custom offset, and custom size of the sprite.
     * 
     * @param frame The name of the sprite frame to use.
     * @param offsetX The custom horizontal offset of the sprite.
     * @param offsetY The custom vertical offset of the sprite.
     * @param width The custom width of the sprite.
     * @param height The custom height of the sprite.
     */
    CustomSpriteConfig& set(
        std::string frame, int offsetX, int offsetY, int width, int height
    );

    /**
     * Sets the frame name and custom size of the sprite.
     * The sprite uses NO custom offset.
     * 
     * @param frame The name of the sprite frame to use.
     * @param width The custom width of the sprite.
     * @param height The custom height of the sprite.
     */
    CustomSpriteConfig& set(std::string frame, int width, int height) {
        return set(frame, 0, 0, width, height);
    }

    /**
     * Sets the frame name and custom size of the sprite.
     * The sprite uses NO custom offset.
     * 
     * @param frame The name of the sprite frame to use.
     * @param size The custom size of the sprite. This value is used for both the width and height.
     */
    CustomSpriteConfig& set(std::string frame, int size) {
        return set(frame, 0, 0, size, size);
    }

    /**
     * Sets the frame name of the sprite.
     * The sprite uses its actual size and NO custom offset.
     * 
     * @param frame The name of the sprite frame to use.
     */
    CustomSpriteConfig& set(std::string frame) {
        return set(frame, 0, 0, 0, 0);
    }

    /**
     * Sets the custom offset of the sprite.
     * 
     * @param x The custom horizontal offset of the sprite.
     * @param y The custom vertical offset of the sprite.
     */
    CustomSpriteConfig& setOffset(int x, int y) {
        offset = cocos2d::CCPoint(x, y); return *this;
    }

    /**
     * Sets the custom size of the sprite.
     * 
     * @param width The custom width of the sprite.
     * @param height The custom height of the sprite.
     */
    CustomSpriteConfig& setSize(int width, int height) {
        size = cocos2d::CCSize(width, height); return *this;
    }

    std::string getModID() const;
    std::string getFrameName() const;
    std::string getSourceFrame() const;
    cocos2d::CCPoint getOffset() const;
    cocos2d::CCSize getSize() const;

    bool isCustomSprite() const;
    bool isAnimationFrame() const;
    bool isModTrigger() const;
};