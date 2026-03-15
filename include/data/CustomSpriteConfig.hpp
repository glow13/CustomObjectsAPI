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
    CustomSpriteConfig& set(
        std::string frame, int offsetX, int offsetY, int width, int height
    );

    CustomSpriteConfig& set(std::string frame, int width, int height) {
        return set(frame, 0, 0, width, height);
    }

    CustomSpriteConfig& set(std::string frame, int size) {
        return set(frame, 0, 0, size, size);
    }

    CustomSpriteConfig& set(std::string frame) {
        return set(frame, 0, 0, 0, 0);
    }

    CustomSpriteConfig& setOffset(int x, int y) {
        offset = cocos2d::CCPoint(x, y); return *this;
    }

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