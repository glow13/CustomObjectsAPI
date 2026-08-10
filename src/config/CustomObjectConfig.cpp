#include <Geode/Geode.hpp>
#include "../impl.hpp"

#include "../manager/CustomObjectsManager.hpp"

using namespace geode::prelude;

#define CONFIG_OPTION(name, type, variable, result, ...)                    \
    type CustomObjectConfig::get##name() const { return m_impl->variable; } \
    CustomObjectConfig&& CustomObjectConfig::set##name(__VA_ARGS__)         \
    { m_impl->variable = result; return (CustomObjectConfig&&)*this; }

constexpr CCSize BOX_SIZE_DEFAULT = CCSizeZero;
constexpr CCPoint BOX_OFFSET_DEFAULT = CCPointZero;
constexpr int BOX_RADIUS_DEFAULT = 0;
constexpr CCPoint OBJECT_OFFSET_DEFAULT = CCPointZero;
constexpr GameObjectType OBJECT_TYPE_DEFAULT = (GameObjectType)(-1);
constexpr int BATCH_MODE_DEFAULT = CUSTOM_PARENT_MODE;
constexpr bool DISABLE_BATCH_DEFAULT = false;
constexpr int FRAMES_COUNT_DEFAULT = 1;
constexpr float FRAME_TIME_DEFAULT = 1.0f;
constexpr ccColor3B GLOW_COLOR_DEFAULT = {255, 255, 255};
constexpr ccColor3B PARTICLE_COLOR_DEFAULT = {255, 255, 255};
constexpr GLubyte PARTICLE_OPACITY_DEFUALT = 255;
constexpr bool PARTICLE_BLENDING_DEFAULT = true;
constexpr int EDITOR_PRIORITY_DEFAULT = 0;

CustomObjectConfig::Impl::Impl(std::string_view id, int objectID, ObjectConstructor ctor) :
    m_id(id), m_mod(id.substr(0, id.find("/"))),
    m_objectID(objectID), m_ctor(std::move(ctor)),
    m_boxSize(BOX_SIZE_DEFAULT),
    m_boxOffset(BOX_OFFSET_DEFAULT),
    m_boxRadius(BOX_RADIUS_DEFAULT),
    m_objectOffset(OBJECT_OFFSET_DEFAULT),
    m_objectType(OBJECT_TYPE_DEFAULT),
    m_batchMode(BATCH_MODE_DEFAULT),
    m_disableBatch(DISABLE_BATCH_DEFAULT),
    m_framesCount(FRAMES_COUNT_DEFAULT),
    m_frameTime(FRAME_TIME_DEFAULT),
    m_glowColor(GLOW_COLOR_DEFAULT),
    m_particleColor(PARTICLE_COLOR_DEFAULT),
    m_particleOpacity(PARTICLE_OPACITY_DEFUALT),
    m_particleBlending(PARTICLE_BLENDING_DEFAULT),
    m_editorPriority(EDITOR_PRIORITY_DEFAULT) {}

CustomObjectConfig::~CustomObjectConfig() = default;
CustomObjectConfig::CustomObjectConfig(std::string_view id, int objectID, ObjectConstructor ctor) : m_impl(std::make_unique<Impl>(id, objectID, std::move(ctor))) {}

CONFIG_OPTION(BoxSize, CCSize, m_boxSize, CCSize(w, h), int w, int h);
CONFIG_OPTION(BoxOffset, CCPoint, m_boxOffset, CCPoint(x, y), int x, int y);
CONFIG_OPTION(BoxRadius, int, m_boxRadius, radius, int radius);
CONFIG_OPTION(ObjectOffset, CCPoint, m_objectOffset, CCPoint(x, y), int x, int y);
CONFIG_OPTION(ObjectType, GameObjectType, m_objectType, type, GameObjectType type);
CONFIG_OPTION(BatchMode, int, m_batchMode, mode, int mode);
CONFIG_OPTION(DisableBatchRender, bool, m_disableBatch, true);
CONFIG_OPTION(FramesCount, int, m_framesCount, frames, int frames);
CONFIG_OPTION(FrameTime, float, m_frameTime, time, float time);
CONFIG_OPTION(GlowColor, ccColor3B, m_glowColor, ccColor3B(r, g, b), GLubyte r, GLubyte g, GLubyte b);
CONFIG_OPTION(ParticleColor, ccColor3B, m_particleColor, ccColor3B(r, g, b), GLubyte r, GLubyte g, GLubyte b);
CONFIG_OPTION(ParticleOpacity, GLubyte, m_particleOpacity, opacity, GLubyte opacity);
CONFIG_OPTION(ParticleBlending, bool, m_particleBlending, blending, bool blending);
CONFIG_OPTION(EditorPriority, int, m_editorPriority, priority, int priority);

std::string CustomObjectConfig::getID() const
    { return m_impl->m_id; }
std::string CustomObjectConfig::getModID() const
    { return m_impl->m_mod; }
int CustomObjectConfig::getObjectID() const
    { return m_impl->m_objectID; }

std::string CustomObjectConfig::getMainSprite() const
    { return hasMainSprite() ? m_impl->m_mainSprite->getFrameName() : ""; }
std::string CustomObjectConfig::getDetailSprite() const
    { return hasDetailSprite() ? m_impl->m_detailSprite->getFrameName() : ""; }
std::string CustomObjectConfig::getGlowSprite() const
    { return hasGlowSprite() ? m_impl->m_glowSprite->getFrameName() : ""; }

bool CustomObjectConfig::hasMainSprite() const
    { return m_impl->m_mainSprite != nullptr; }
bool CustomObjectConfig::hasDetailSprite() const
    { return m_impl->m_detailSprite != nullptr; }
bool CustomObjectConfig::hasGlowSprite() const
    { return m_impl->m_glowSprite != nullptr; }

bool CustomObjectConfig::isCustomBatch() const
    { return !m_impl->m_disableBatch && m_impl->m_batchMode == BATCH_MODE_DEFAULT; }
bool CustomObjectConfig::hasCustomAnimation() const
    { return m_impl->m_framesCount != FRAMES_COUNT_DEFAULT && m_impl->m_mainSprite->isAnimationFrame(); }

CustomObjectConfig&& CustomObjectConfig::setMainSprite(std::string frame, int x, int y, int w, int h)
    { m_impl->m_mainSprite = std::make_unique<CustomSpriteConfig>(this, frame, x, y, w, h, true); return (CustomObjectConfig&&)*this; }
CustomObjectConfig&& CustomObjectConfig::setMainSprite(std::string frame, int w, int h)
    { m_impl->m_mainSprite = std::make_unique<CustomSpriteConfig>(this, frame, 0, 0, w, h, true); return (CustomObjectConfig&&)*this; }
CustomObjectConfig&& CustomObjectConfig::setMainSprite(std::string frame, int s)
    { m_impl->m_mainSprite = std::make_unique<CustomSpriteConfig>(this, frame, 0, 0, s, s, true); return (CustomObjectConfig&&)*this; }
CustomObjectConfig&& CustomObjectConfig::setMainSprite(std::string frame, bool sheet)
    { m_impl->m_mainSprite = std::make_unique<CustomSpriteConfig>(this, frame, 0, 0, 0, 0, sheet); return (CustomObjectConfig&&)*this; }

CustomObjectConfig&& CustomObjectConfig::setDetailSprite(std::string frame, int x, int y, int w, int h)
    { m_impl->m_detailSprite = std::make_unique<CustomSpriteConfig>(this, frame, x, y, w, h, true); return (CustomObjectConfig&&)*this; }
CustomObjectConfig&& CustomObjectConfig::setDetailSprite(std::string frame, int w, int h)
    { m_impl->m_detailSprite = std::make_unique<CustomSpriteConfig>(this, frame, 0, 0, w, h, true); return (CustomObjectConfig&&)*this; }
CustomObjectConfig&& CustomObjectConfig::setDetailSprite(std::string frame, int s)
    { m_impl->m_detailSprite = std::make_unique<CustomSpriteConfig>(this, frame, 0, 0, s, s, true); return (CustomObjectConfig&&)*this; }
CustomObjectConfig&& CustomObjectConfig::setDetailSprite(std::string frame, bool sheet)
    { m_impl->m_detailSprite = std::make_unique<CustomSpriteConfig>(this, frame, 0, 0, 0, 0, sheet); return (CustomObjectConfig&&)*this; }

CustomObjectConfig&& CustomObjectConfig::setGlowSprite(std::string frame, int x, int y, int w, int h)
    { m_impl->m_glowSprite = std::make_unique<CustomSpriteConfig>(this, frame, x, y, w, h, true); return (CustomObjectConfig&&)*this; }
CustomObjectConfig&& CustomObjectConfig::setGlowSprite(std::string frame, int w, int h)
    { m_impl->m_glowSprite = std::make_unique<CustomSpriteConfig>(this, frame, 0, 0, w, h, true); return (CustomObjectConfig&&)*this; }
CustomObjectConfig&& CustomObjectConfig::setGlowSprite(std::string frame, int s)
    { m_impl->m_glowSprite = std::make_unique<CustomSpriteConfig>(this, frame, 0, 0, s, s, true); return (CustomObjectConfig&&)*this; }
CustomObjectConfig&& CustomObjectConfig::setGlowSprite(std::string frame, bool sheet)
    { m_impl->m_glowSprite = std::make_unique<CustomSpriteConfig>(this, frame, 0, 0, 0, 0, sheet); return (CustomObjectConfig&&)*this; }

CustomObjectConfig&& CustomObjectConfig::onEditObject(EditObjectCallback callback)
    { m_impl->m_editObject = std::move(callback); return (CustomObjectConfig&&)*this; }
CustomObjectConfig&& CustomObjectConfig::onEditSpecial(EditObjectCallback callback)
    { m_impl->m_editSpecial = std::move(callback); return (CustomObjectConfig&&)*this; }

bool CustomObjectConfig::hasEditObjectCallback() const
    { return m_impl->m_editObject != nullptr; }
bool CustomObjectConfig::hasEditSpecialCallback() const
    { return m_impl->m_editSpecial != nullptr; }

void CustomObjectConfig::setupCustomObject(GameObject* obj) const
    { if (m_impl->m_setupCallback) m_impl->m_setupCallback(obj); }
void CustomObjectConfig::resetCustomObject(GameObject* obj) const
    { if (m_impl->m_resetCallback) m_impl->m_resetCallback(obj); }
void CustomObjectConfig::activateCustomObject(GameObject* obj, GJBaseGameLayer* level, PlayerObject* player) const
    { if (m_impl->m_activateCallback) m_impl->m_activateCallback(obj, level, player); }

void CustomObjectConfig::customEditObject(GameObject* obj, cocos2d::CCArray* objs) const
    { if (m_impl->m_editObject != nullptr) m_impl->m_editObject(obj, objs); }
void CustomObjectConfig::customEditSpecial(GameObject* obj, cocos2d::CCArray* objs) const
    { if (m_impl->m_editSpecial != nullptr) m_impl->m_editSpecial(obj, objs); }

GameObject* CustomObjectConfig::createCustomObject() const {
    if (!m_impl->m_ctor) return nullptr;

    GameObject* obj = m_impl->m_ctor(this);
    obj->m_objectID = m_impl->m_objectID;
    obj->m_parentMode = BATCH_MODE_DEFAULT;

    if (m_impl->m_boxSize != BOX_SIZE_DEFAULT) { obj->m_width = m_impl->m_boxSize.width; obj->m_height = m_impl->m_boxSize.height; }
    if (m_impl->m_boxOffset != BOX_OFFSET_DEFAULT) obj->m_customBoxOffset = m_impl->m_boxOffset;
    if (m_impl->m_boxRadius != BOX_RADIUS_DEFAULT) obj->m_objectRadius = m_impl->m_boxRadius;
    if (m_impl->m_objectType != OBJECT_TYPE_DEFAULT) obj->m_objectType = m_impl->m_objectType;
    if (m_impl->m_batchMode != BATCH_MODE_DEFAULT) obj->m_parentMode = m_impl->m_batchMode;
    if (m_impl->m_disableBatch) { obj->m_parentMode = 4; obj->m_addToNodeContainer = true; }
    if (m_impl->m_glowColor != GLOW_COLOR_DEFAULT) obj->setGlowColor(m_impl->m_glowColor);

    return obj;
}

CustomObjectConfig* CustomObjectConfig::registerConfig(std::string_view stringID, ObjectConstructor ctor) {
    return CustomObjectsManager::get()->registerObjectConfig(stringID, std::move(ctor));
}