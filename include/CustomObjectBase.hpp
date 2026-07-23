#pragma once

template <class BaseType>
class CustomObjectBase : public BaseType {
public:
    static void onLoad() {}
    bool init(const class CustomObjectConfig*) { return true; }
};