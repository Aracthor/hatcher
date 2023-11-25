#pragma once

#include <vector>

#include "ISaveLoader.hpp"

namespace hatcher
{

class ComponentLoader final : public ISaveLoader
{
public:
    ComponentLoader(const std::vector<ubyte>& data);

    void separator(char c) override;
    void operator<<(bool& value) override;
    void operator<<(char& value) override;
    void operator<<(ubyte& value) override;
    void operator<<(int& value) override;
    void operator<<(uint& value) override;
    void operator<<(float& value) override;

private:
    bool IsSaving() const override { return false; }

    const std::vector<ubyte> m_data;
    int m_currentIndex = 0;
};

} // namespace hatcher
