#pragma once
#include "CustomObjectBase.hpp"
#include "CustomObjectProperty.hpp"

struct CustomObjectInterface::Impl final {
    const CustomObjectConfig* m_config;
    std::unordered_map<uint16_t, std::unique_ptr<ObjectPropertyInterface>> m_customProperties;
    std::unordered_map<uint16_t, std::string> m_loadedProperties;
};