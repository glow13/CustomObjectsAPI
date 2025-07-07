#include <Geode/Geode.hpp>

using namespace geode::prelude;

class CustomEffectGameObject : public EffectGameObject {
public:
    static CustomEffectGameObject* create(int id);
    bool customInit();

    void triggerObject(GJBaseGameLayer* p0, int p1, gd::vector<int> const* p2) override;
};