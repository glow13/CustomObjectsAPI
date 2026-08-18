#pragma once

struct ObjectPropertyInterface {
    virtual ~ObjectPropertyInterface() = default;

    virtual bool isValid() = 0;
    virtual std::string serialize() = 0;
    virtual void deserialize(const std::string&) = 0;
};

template <typename T>
requires(
    std::same_as<T, bool> || std::same_as<T, int> ||
    std::same_as<T, float> || std::same_as<T, std::string>
)
struct ObjectProperty : ObjectPropertyInterface {
    T* m_value = nullptr;
    geode::Function<bool()> m_cond = nullptr;
    ObjectProperty(T&, geode::Function<bool()>);

    bool isValid() override;
    std::string serialize() override;
    void deserialize(const std::string&) override;
};