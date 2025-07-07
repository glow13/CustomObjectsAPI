#include <Geode/Geode.hpp>

using namespace geode::prelude;

class CustomGameObject : public GameObject {
public:
    static CustomGameObject* create(int id);
    bool customInit();
};