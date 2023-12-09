#pragma once

#include <vector>

#include "ISaveLoader.hpp"

namespace hatcher
{

class ComponentLoader final : public ISaveLoader
{
public:
    ComponentLoader(const std::vector<ubyte>& data);

private:
    bool IsSaving() const override { return false; }

    void SaveLoadData(void* value, int size) override;

    const std::vector<ubyte> m_data;
    int m_currentIndex = 0;
};

} // namespace hatcher
