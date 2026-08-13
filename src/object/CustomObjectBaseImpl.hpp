#pragma once
#include "CustomObjectBase.hpp"

struct CustomObjectInterface::Impl {
    const CustomObjectConfig* m_config;
};