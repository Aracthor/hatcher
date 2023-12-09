#pragma once

#include "ISaveLoader.hpp"
#include "basic_types.hpp"

#include <vector>

namespace hatcher
{

class ComponentSaver final : public ISaveLoader
{
public:
    const std::vector<ubyte>& Result() const;

    template <class Component>
    void SaveComponent(Component& component);

private:
    bool IsSaving() const override { return true; }

    void SaveLoadData(void* value, int size) override;

    std::vector<ubyte> m_data;
};

} // namespace hatcher

#include "ComponentSaver.ipp"
