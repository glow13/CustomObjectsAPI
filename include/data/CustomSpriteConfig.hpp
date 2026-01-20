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

using namespace geode::prelude;

class CustomObjectsMod;
class CustomObjectConfigBase;

struct CUSTOM_OBJECTS_DLL CustomSpriteConfig final {
#if __INTELLISENSE__ != 1 || CUSTOM_OBJECTS_EXPORTING
private:
    CustomObjectsMod* mod;
    CustomObjectConfigBase* object;

    std::string frameName;
    std::string sourceFrame;
    CCPoint offset;
    CCSize size;

    void generateFrame();

    friend class CustomObjectsManager;
    friend class CustomSpritesManager;

public:
    CustomSpriteConfig() : frameName(""), sourceFrame(""), offset(CCPointZero), size(CCSizeZero) {}
    CustomSpriteConfig(CustomObjectsMod* mod, CustomObjectConfigBase* object, std::string frame, int offX, int offY, int sizeW, int sizeH);
#endif
public:
    std::string getModID() const;
    std::string getFrameName() const;
    std::string getSourceFrame() const;
    CCPoint getOffset() const;
    CCSize getSize() const;

    bool isCustomSprite() const;
    bool isAnimationFrame() const;
    bool isModTrigger() const;
};