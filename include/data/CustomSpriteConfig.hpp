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

class CustomObjectsMod;
class CustomObjectConfigBase;

struct CUSTOM_OBJECTS_DLL CustomSpriteConfig final {
#if __INTELLISENSE__ != 1 || CUSTOM_OBJECTS_EXPORTING
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

public:
    CustomSpriteConfig() : frameName(""), sourceFrame(""), offset({0, 0}), size({0, 0}) {}
    CustomSpriteConfig(CustomObjectsMod* mod, CustomObjectConfigBase* object, std::string frame, int offX, int offY, int sizeW, int sizeH);
#endif
public:
    std::string getModID() const;
    std::string getFrameName() const;
    std::string getSourceFrame() const;
    cocos2d::CCPoint getOffset() const;
    cocos2d::CCSize getSize() const;

    bool isCustomSprite() const;
    bool isAnimationFrame() const;
    bool isModTrigger() const;
};