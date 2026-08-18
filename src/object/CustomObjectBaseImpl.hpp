#pragma once
#include "CustomObjectBase.hpp"
#include "CustomObjectProperty.hpp"

struct CustomObjectInterface::Impl final {
    const CustomObjectConfig* m_config;
    std::map<uint16_t, std::unique_ptr<ObjectPropertyInterface>> m_properties;
    std::map<uint16_t, std::string> m_loadedProperties;
};