#pragma once

#include "CustomSpriteConfig.hpp"

class CUSTOM_OBJECTS_DLL CustomObjectConfigBase {
public:
    CustomSpriteConfig mainSprite;
    CustomSpriteConfig detailSprite;
    CustomSpriteConfig glowSprite;

#ifdef CUSTOM_OBJECTS_INTELLISENSE_DISABLED
protected:
    const CustomObjectsMod* mod;
    int objectID;

    cocos2d::CCSize boxSize;
    cocos2d::CCPoint boxOffset;
    int boxRadius;
    cocos2d::CCPoint objectOffset;
    GameObjectType objectType;
    int batchMode;
    bool disableBatch;
    int framesCount;
    float frameTime;
    cocos2d::ccColor3B glowColor;
    cocos2d::ccColor3B particleColor;
    GLubyte particleOpacity;
    bool particleBlending;
    int editorPriority;

    template <class, class>
    friend class CustomObjectBase;
    friend class CustomObjectsManager;
    friend class CustomEditorUI;

protected:
    CustomObjectConfigBase(const CustomObjectsMod*);

public:
    std::string getModID() const;
    std::string getModName() const;
    int getObjectID() const;

    std::string getMainSprite() const;
    std::string getDetailSprite() const;
    std::string getGlowSprite() const;

    bool hasMainSprite() const;
    bool hasDetailSprite() const;
    bool hasGlowSprite() const;

    bool isCustomBatch() const;
    bool hasCustomAnimation() const;

    virtual bool hasEditObjectFunction() const = 0;
    virtual bool hasEditSpecialFunction() const = 0;

    virtual void customEditObject(GameObject*, cocos2d::CCArray*) const = 0;
    virtual void customEditSpecial(GameObject*, cocos2d::CCArray*) const = 0;

    void applyConfigValues(GameObject*) const;
    virtual GameObject* create() const = 0;
#endif
};

/**
 * A class representing a custom object. This class can be configured to change properties of your custom object.
 * 
 * You should never try to create one of these directly, to obtain a CustomObjectConfig you must first register a custom object:
 * 
 * auto object = CustomObjectsAPI::registerCustomObject(...);
 * object.setObjectProperty(...);
 * 
 * Or you can configure your object inline:
 * 
 * CustomObjectsAPI::registerCustomObject(...).setObjectProperty1(...).setObjectProperty2(...);
 */
template <class ObjectType>
class CustomObjectConfig final : public CustomObjectConfigBase {
private:
#ifdef CUSTOM_OBJECTS_INTELLISENSE_DISABLED
    std::function<void(ObjectType*)> setupCustomObjectCallback;
    std::function<void(ObjectType*)> resetCustomObjectCallback;
    std::function<void(ObjectType*, cocos2d::CCArray*)> editObjectCallback;
    std::function<void(ObjectType*, cocos2d::CCArray*)> editSpecialCallback;
    std::function<void(ObjectType*, GJBaseGameLayer*, PlayerObject*)> activateCustomObjectCallback;

    CustomObjectConfig(const CustomObjectsMod* mod) : CustomObjectConfigBase(mod) {}

    template <class, class>
    friend class CustomObjectBase;
    friend class CustomObjectsManager;
    friend class CustomObjectsAPI;

public:
    bool hasEditObjectFunction() const override { return (bool)editObjectCallback; }
    bool hasEditSpecialFunction() const override { return (bool)editSpecialCallback; }

    void customEditObject(GameObject* obj, cocos2d::CCArray* objs) const override { editObjectCallback(static_cast<ObjectType*>(obj), objs); }
    void customEditSpecial(GameObject* obj, cocos2d::CCArray* objs) const override { editSpecialCallback(static_cast<ObjectType*>(obj), objs); }
#endif
public:

    /**
     * Sets the main sprite for the custom object.
     * The sprite is automatically registered with a custom size and custom offset.
     * 
     * @param frame The name of the sprite frame to use.
     * @param offsetX The custom horizontal offset of the sprite.
     * @param offsetY The custom vertical offset of the sprite.
     * @param width The custom width of the sprite.
     * @param height The custom height of the sprite.
     */
    CustomObjectConfig<ObjectType>& setMainSprite(
        std::string frame, int offsetX, int offsetY, int width, int height
    ) {
        mainSprite.set(frame, offsetX, offsetY, width, height); return *this;
    }

    /**
     * Sets the main sprite for the custom object.
     * The sprite is automatically registered with a custom size and NO custom offset.
     * 
     * @param frame The name of the sprite frame to use.
     * @param width The custom width of the sprite.
     * @param height The custom height of the sprite.
     */
    inline CustomObjectConfig<ObjectType>& setMainSprite(std::string frame, int width, int height) {
        return setMainSprite(frame, 0, 0, width, height);
    }

    /**
     * Sets the main sprite for the custom object.
     * The sprite is automatically registered with a custom size and NO custom offset.
     * 
     * @param frame The name of the sprite frame to use.
     * @param size The custom size of the sprite. This value is used for both the width and height.
     */
    inline CustomObjectConfig<ObjectType>& setMainSprite(std::string frame, int size) {
        return setMainSprite(frame, 0, 0, size, size);
    }

    /**
     * Sets the main sprite for the custom object.
     * The sprite is automatically registered with its actual size and NO custom offset.
     * 
     * @param frame The name of the sprite frame to use.
     */
    inline CustomObjectConfig<ObjectType>& setMainSprite(std::string frame) {
        return setMainSprite(frame, 0, 0, 0, 0);
    }

    /**
     * Sets the main sprite for the custom object.
     * You can use this to set the main sprite to an already registered sprite.
     * 
     * You can register a new sprite using CustomObjectsAPI::registerCustomSprite(...), or you can use one from a previously registered object.
     * 
     * @param sprite The CustomSpriteConfig to use as this object's main sprite.
     */
    CustomObjectConfig<ObjectType>& setMainSprite(const CustomSpriteConfig& sprite) {
        mainSprite = sprite; return *this;
    }

    /**
     * Sets the detail sprite for the custom object.
     * The sprite is automatically registered with a custom size and custom offset.
     * This sprite is added at the same position as the main sprite and uses the object's detail color by default.
     * 
     * @param frame The name of the sprite frame to use.
     * @param offsetX The custom horizontal offset of the sprite.
     * @param offsetY The custom vertical offset of the sprite.
     * @param width The custom width of the sprite.
     * @param height The custom height of the sprite.
     */
    CustomObjectConfig<ObjectType>& setDetailSprite(
        std::string frame, int offsetX, int offsetY, int width, int height
    ) {
        detailSprite.set(frame, offsetX, offsetY, width, height); return *this;
    }

    /**
     * Sets the detail sprite for the custom object.
     * The sprite is automatically registered with a custom size and NO custom offset.
     * This sprite is added at the same position as the main sprite and uses the object's detail color by default.
     * 
     * @param frame The name of the sprite frame to use.
     * @param width The custom width of the sprite.
     * @param height The custom height of the sprite.
     */
    inline CustomObjectConfig<ObjectType>& setDetailSprite(std::string frame, int width, int height) {
        return setDetailSprite(frame, 0, 0, width, height);
    }

    /**
     * Sets the detail sprite for the custom object.
     * The sprite is automatically registered with a custom size and NO custom offset.
     * This sprite is added at the same position as the main sprite and uses the object's detail color by default.
     * 
     * @param frame The name of the sprite frame to use.
     * @param size The custom size of the sprite. This value is used for both the width and height.
     */
    inline CustomObjectConfig<ObjectType>& setDetailSprite(std::string frame, int size) {
        return setDetailSprite(frame, 0, 0, size, size);
    }

    /**
     * Sets the detail sprite for the custom object.
     * The sprite is automatically registered with its actual size and NO custom offset.
     * This sprite is added at the same position as the main sprite and uses the object's detail color by default.
     * 
     * @param frame The name of the sprite frame to use.
     */
    inline CustomObjectConfig<ObjectType>& setDetailSprite(std::string frame) {
        return setDetailSprite(frame, 0, 0, 0, 0);
    }

    /**
     * Sets the detail sprite for the custom object.
     * You can use this to set the detail sprite to an already registered sprite.
     * 
     * You can register a new sprite using CustomObjectsAPI::registerCustomSprite(...), or you can use one from a previously registered object.
     * 
     * @param sprite The CustomSpriteConfig to use as this object's detail sprite.
     */
    CustomObjectConfig<ObjectType>& setDetailSprite(const CustomSpriteConfig& sprite) {
        detailSprite = sprite; return *this;
    }

    /**
     * Sets the glow sprite for the custom object.
     * The sprite is automatically registered with a custom size and custom offset.
     * This sprite is not visible in the editor and is rendered with blending by default. 
     * 
     * @param frame The name of the sprite frame to use.
     * @param offsetX The custom horizontal offset of the sprite.
     * @param offsetY The custom vertical offset of the sprite.
     * @param width The custom width of the sprite.
     * @param height The custom height of the sprite.
     */
    CustomObjectConfig<ObjectType>& setGlowSprite(
        std::string frame, int offsetX, int offsetY, int width, int height
    ) {
        glowSprite.set(frame, offsetX, offsetY, width, height); return *this;
    }

    /**
     * Sets the glow sprite for the custom object.
     * The sprite is automatically registered with a custom size and NO custom offset.
     * This sprite is not visible in the editor and is rendered with blending by default.
     * 
     * @param frame The name of the sprite frame to use.
     * @param width The custom width of the sprite.
     * @param height The custom height of the sprite.
     */
    inline CustomObjectConfig<ObjectType>& setGlowSprite(std::string frame, int width, int height) {
        return setGlowSprite(frame, 0, 0, width, height);
    }

    /**
     * Sets the glow sprite for the custom object.
     * The sprite is automatically registered with a custom size and NO custom offset.
     * This sprite is not visible in the editor and is rendered with blending by default.
     * 
     * @param frame The name of the sprite frame to use.
     * @param size The custom size of the sprite. This value is used for both the width and height.
     */
    inline CustomObjectConfig<ObjectType>& setGlowSprite(std::string frame, int size) {
        return setGlowSprite(frame, 0, 0, size, size);
    }

    /**
     * Sets the glow sprite for the custom object.
     * The sprite is automatically registered with its actual size and NO custom offset.
     * This sprite is not visible in the editor and is rendered with blending by default.
     * 
     * @param frame The name of the sprite frame to use.
     */
    inline CustomObjectConfig<ObjectType>& setGlowSprite(std::string frame) {
        return setGlowSprite(frame, 0, 0, 0, 0);
    }

    /**
     * Sets the glow sprite for the custom object.
     * You can use this to set the glow sprite to an already registered sprite.
     * 
     * You can register a new sprite using CustomObjectsAPI::registerCustomSprite(...), or you can use one from a previously registered object.
     * 
     * @param sprite The CustomSpriteConfig to use as this object's glow sprite.
     */
    CustomObjectConfig<ObjectType>& setGlowSprite(const CustomSpriteConfig& sprite) {
        glowSprite = sprite; return *this;
    }

    /**
     * Sets the size of the object's collision hitbox.
     * The hitbox is set to the size of the main sprite by default.
     * 
     * @param w The width of the hitbox.
     * @param h The height of the hitbox.
     */
    CustomObjectConfig<ObjectType>& setBoxSize(int w, int h) {
        boxSize = cocos2d::CCSize(w, h); return *this;
    }

    /**
     * Sets the position offset of the object's collision hitbox.
     * The hitbox position is set to the center of the main sprite by default.
     * 
     * @param x The horizontal offset of the hitbox.
     * @param y The vertical offset of the hitbox.
     */
    CustomObjectConfig<ObjectType>& setBoxOffset(int x, int y) {
        boxOffset = cocos2d::CCPoint(x, y); return *this;
    }

    /**
     * Sets the length of the radius of the object's collision hitbox.
     * This only has an effect if the object has a circular hitbox, for example the CustomRotateObject class.
     * 
     * @param radius The length of the radius of the collision hitbox.
     */
    CustomObjectConfig<ObjectType>& setBoxRadius(int radius) {
        boxRadius = radius; return *this;
    }

    /**
     * Sets the offset when creating or transforming the custom object.
     * This controls the object's center point when placing it in the editor, and changes its pivot point when applying transformations with the edit tab.
     * For an example look at how a bounce pad is placed at the bottom of its grid tile instead of the center.
     * 
     * @param x The horizontal offset of the object.
     * @param y The vertical offset of the object.
     */
    CustomObjectConfig<ObjectType>& setObjectOffset(int x, int y) {
        objectOffset = cocos2d::CCPoint(x, y); return *this;
    }

    /**
     * Sets the GameObjectType type of the custom object.
     * This is primarily used to control how the player interacts with the object's hitbox, for example solid or hazard.
     * 
     * @param type The GameObjectType type of the object.
     */
    CustomObjectConfig<ObjectType>& setObjectType(GameObjectType type) {
        objectType = type; return *this;
    }

    /**
     * Sets the batch rendering mode used for the custom object.
     * This controls which CCSpriteBatchNode the object gets added to in the GJBaseGameLayer, and therefore which spritesheet the sprite uses to draw.
     * 
     * It's hard to explain if you don't understand already, but essentially you have to match the sprites you're using for your custom object to the correct spritesheet that the sprite belongs to.
     * You should never have to change this if you register all of your sprites to the custom objects spritesheet, which is the default behavior.
     * 
     * Some notable batch modes are mode 10 which is the custom objects spritesheet, this is the default value for all newly created objects.
     * Another important batch mode is mode 4, which doesn't use a spritesheed and just adds the object to a CCNodeContainer.
     * This is the mode used for objects that have sprites from multiple spritesheets, such as trigger objects so that they can correctly render their text labels.
     * 
     * Using this is HIGHLY DISCOURAGED unless absolutely required and you know what you're doing!
     * 
     * @param mode The batch rendering mode of the object.
     */
    CustomObjectConfig<ObjectType>& setBatchMode(int mode) {
        batchMode = mode; return *this;
    }

    /**
     * Disables batch rendering for the custom object. This is mostly equivalent to setting the batch mode to mode 4.
     * This is required when your object uses multiple sprites from different spritesheets, or when it uses text because the text sprites are not part of any spritesheet.
     * 
     * Using this is HIGHLY DISCOURAGED unless absolutely required and you know what you're doing, as batch rendering is important to maintain good performance and to reduce draw calls.
     */
    CustomObjectConfig<ObjectType>& setDisableBatchRender() {
        disableBatch = true; return *this;
    }

    /**
     * Sets the number of animation frames in the object's animation.
     * This only has an effect if the object has a custom animation, see the CustomAnimatedObject class.
     * 
     * @param frames The number of frames in the object's animation.
     */
    CustomObjectConfig<ObjectType>& setFramesCount(int frames) {
        framesCount = frames; return *this;
    }

    /**
     * Sets the duration of each frame in the object's animation.
     * This only has an effect if the object has a custom animation, see the CustomAnimatedObject class.
     * 
     * @param time The duration of each from of the animation, in seconds.
     */
    CustomObjectConfig<ObjectType>& setFrameTime(float time) {
        frameTime = time; return *this;
    }

    /**
     * Sets the color of the glow sprite of this object in RGB format.
     * This only has an effect if the object has a glow sprite.
     * 
     * @param r yeah its the red value
     * @param g this ones the green value
     * @param b youll never guess what this one is
     */
    CustomObjectConfig<ObjectType>& setGlowColor(GLubyte r, GLubyte g, GLubyte b) {
        glowColor = {r, g, b}; return *this;
    }

    /**
     * Sets the color of the particles of this object in RGB format.
     * This only has an effect if the object has a particle effect, currently it's only used in the CustomPadObject, CustomRingObject, and CustomPortalObject classes.
     * 
     * @param r yeah its the red value
     * @param g this ones the green value
     * @param b youll never guess what this one is
     */
    CustomObjectConfig<ObjectType>& setParticleColor(GLubyte r, GLubyte g, GLubyte b) {
        particleColor = {r, g, b}; return *this;
    }

    /**
     * Sets the opacity of the object's particle effect.
     * This only has an effect if the object has a particle effect, currently it's only used in the CustomPadObject, CustomRingObject, and CustomPortalObject classes.
     * 
     * @param opacity The alpha value of the particles.
     */
    CustomObjectConfig<ObjectType>& setParticleOpacity(GLubyte opacity) {
        particleOpacity = opacity; return *this;
    }

    /**
     * Toggles the blending effect on the object's particle effect.
     * This only has an effect if the object has a particle effect, currently it's only used in the CustomPadObject, CustomRingObject, and CustomPortalObject classes.
     * 
     * @param blending True if the particles should use blending, false otherwise
     */
    CustomObjectConfig<ObjectType>& setParticleBlending(bool blending) {
        particleBlending = blending; return *this;
    }

    /**
     * Sets the priority for the object order in the custom objects editor build tab.
     * This controls the order that the objects for your mod appear in the build tab. They are sorted by priority first, then by the object ID.
     * 
     * By default each object has a priority of 0, and therefore appear in the same order that you register them.
     * Objects with a lower priority are placed first, in ascending order.
     * 
     * @param priority The priority for the editor tab ordering.
     */
    CustomObjectConfig<ObjectType>& setEditorTabPriority(int priority) {
        editorPriority = priority; return *this;
    }

    /**
     * Sets a callback for when try to edit this object using the EDIT OBJECT button.
     * This is usually used to open a UI for your object, but you can technically do whatever you want.
     * 
     * The callback provides a pointer to your custom object class and a pointer to a CCArray.
     * The object pointer is nullptr when you are editing multiple objects, and the CCArray pointer is nullptr when you are editing a single object.
     * 
     * @param callback The callback.
     */
    CustomObjectConfig<ObjectType>& onEditObjectButton(
        std::function<void(ObjectType*, cocos2d::CCArray*)> callback
    ) {
        editObjectCallback = callback; return *this;
    }

    /**
     * Sets a callback for when try to edit this object using the EDIT SPECIAL button.
     * This is usually used to open a UI for your object, but you can technically do whatever you want.
     * 
     * The callback provides a pointer to your custom object class and a pointer to a CCArray.
     * The object pointer is nullptr when you are editing multiple objects, and the CCArray pointer is nullptr when you are editing a single object.
     * 
     * @param callback The callback.
     */
    CustomObjectConfig<ObjectType>& onEditSpecialButton(
        std::function<void(ObjectType*, cocos2d::CCArray*)> callback
    ) {
        editSpecialCallback = callback; return *this;
    }

    /**
     * Sets a callback for when your custom object is initially created.
     * This is usually used to run any setup processes you need to initialize your custom object, such as initializing variables or adding any extra sprites.
     * It can be thought of as just an init() function.
     * 
     * This callback is run immediately whenever you place a new object in the editor.
     * It's also run for each of the custom objects in your level when loading into the level or into the level editor.
     * 
     * The callback provides a pointer to the custom object that is being created.
     * 
     * @param callback The callback.
     */
    CustomObjectConfig<ObjectType>& onSetupCustomObject(
        std::function<void(ObjectType*)> callback
    ) {
        setupCustomObjectCallback = callback; return *this;
    }

    /**
     * Sets a callback for resetting the custom object between attempts.
     * Because objects are not recreated each attempt, any variables or sprites that change will persist across attempts.
     * This callback can be used to reset your object back to its default state if you don't want this behavior.
     * 
     * The callback provides a pointer to the custom object that is being reset.
     * 
     * @param callback The callback.
     */
    CustomObjectConfig<ObjectType>& onResetCustomObject(
        std::function<void(ObjectType*)> callback
    ) {
        resetCustomObjectCallback = callback; return *this;
    }

    /**
     * Sets a callback for when the player activates the custom object.
     * 
     * This callback provides a pointer to the custom object being activated, a pointer to the current GJBaseGameLayer*, and a pointer to the PlayerObject* that activated the custom object.
     * 
     * THIS CALLBACK DOES NOTHING BY DEFAULT, AND ITS BEHAVIOR IS UNIQUE FOR EACH OBJECT CLASS!!!
     * 
     * Because some objects aren't able to be interacted with by the player, this callback is used by different classes in different ways.
     * 
     * Currently this callback is called by:
     * 
     * - A CustomPadObject when the player touches the object.
     * 
     * - A CustomRingObject when the player clicks while touching the object.
     * 
     * - A CustomPortalObject when the player enters the portal.
     * 
     * - A CustomCollectibleObject when the player touches and collects the object.
     * 
     * - A CustomTriggerObject when the trigger is either spawned or touch triggered, or just triggered normally.
     * 
     * If you want to add support for this callback in your own object class, you can just call the function CustomObjectBase::activateCustomObject wherever you want:
     * 
     * this->activateCustomObject(GJBaseGameLayer*, PlayerObject*);
     * 
     * @param callback The callback.
     */
    CustomObjectConfig<ObjectType>& onActivateCustomObject(
        std::function<void(ObjectType*, GJBaseGameLayer*, PlayerObject*)> callback
    ) {
        activateCustomObjectCallback = callback; return *this;
    }

#ifdef CUSTOM_OBJECTS_INTELLISENSE_DISABLED

    /**
     * Create a new custom object using this config.
     * You should have no reason to do this on your own, this is only called when placing the object in a level.
     * 
     * @return A pointer to a new custom object of type ObjectType.
     */
    ObjectType* create() const override {
        ObjectType* obj = ObjectType::create(this);
        applyConfigValues(static_cast<GameObject*>(obj));
        return obj;
    }

#endif
};