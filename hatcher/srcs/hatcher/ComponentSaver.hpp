#pragma once

#include "ISaveLoader.hpp"
#include "basic_types.hpp"

#include <vector>

namespace hatcher
{

class ComponentSaver final : public ISaveLoader
{
public:
    void separator(char value) override;
    void operator<<(bool& value) override;
    void operator<<(char& value) override;
    void operator<<(ubyte& value) override;
    void operator<<(int& value) override;
    void operator<<(uint& value) override;
    void operator<<(float& value) override;

    const std::vector<ubyte>& Result() const;

    template <class Component>
    void SaveComponent(Component& component);

private:
    bool IsSaving() const override { return true; }

    template <typename T>
    inline void AddSimpleData(T value);

    std::vector<ubyte> m_data;
};

} // namespace hatcher

#include "ComponentSaver.ipp"
