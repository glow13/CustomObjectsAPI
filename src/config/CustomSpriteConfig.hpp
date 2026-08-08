#pragma once
#include <rect_structs.h>

enum Quality : int {
    LOW = 1,
    MEDIUM = 2,
    HIGH = 4
};

class CustomSpriteConfig final {
    struct SheetInfo final {
    private:
        rectpack2D::rect_wh m_offset;
        rectpack2D::rect_wh m_size;
        rectpack2D::rect_xywh m_trim;
        rectpack2D::rect_xywhf m_rect;
    public:
        auto& get_rect();
        const auto& get_rect() const;
        SheetInfo(int, int, int, int);
        friend CustomSpriteConfig;
    };

    struct Impl;
    std::unique_ptr<Impl> m_impl;
public:
    CustomSpriteConfig(class CustomObjectConfig*, std::string, int, int, int, int, bool);
    ~CustomSpriteConfig();

    std::string getModID() const;
    std::string getFrameName() const;
    std::string getSourceFrame() const;

    bool isCustomSprite() const;
    bool isAnimationFrame() const;
    bool isModTrigger() const;
};