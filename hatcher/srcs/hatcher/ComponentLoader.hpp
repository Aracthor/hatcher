#pragma once

#include <sstream>
#include <string>

#include "ISaveLoader.hpp"

namespace hatcher
{

class ComponentLoader final : public ISaveLoader
{
public:
    ComponentLoader(const std::string& data);

    void operator<<(char value) override;
    void operator<<(bool& value) override;
    void operator<<(ubyte& value) override;
    void operator<<(int& value) override;
    void operator<<(uint& value) override;
    void operator<<(float& value) override;

private:
    bool IsSaving() const override { return false; }

    std::istringstream m_stream;
};

} // namespace hatcher
