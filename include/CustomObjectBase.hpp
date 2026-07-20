#pragma once

template <class BaseType>
class CustomObjectBase : public BaseType {
public:
    static void onLoad() {}
};