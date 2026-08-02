#pragma once

template <class BaseType>
class CustomObjectBase : public BaseType {
public:
    bool init(const class CustomObjectConfig*) { return true; }
};