#pragma once
#include <rect_structs.h>

enum Quality : int {
    LOW = 1,
    MEDIUM = 2,
    HIGH = 4
};

class CustomSpriteConfig final {
    struct Impl;
    std::unique_ptr<Impl> m_impl;
public:
    CustomSpriteConfig(class CustomObjectConfig*, std::string, int, int, int, int, bool);
    ~CustomSpriteConfig();

    std::string getModID() const;
    std::string getFrameName() const;
    std::string getSourceFrame() const;
    struct SheetInfo getSheetInfo(Quality) const;

    bool isCustomSprite() const;
    bool isAnimationFrame() const;
    bool isModTrigger() const;
};