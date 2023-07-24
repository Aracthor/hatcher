#pragma once

#include <sstream>

#include "ISaveLoader.hpp"

namespace hatcher
{

class ComponentSaver final : public ISaveLoader
{
public:
    ComponentSaver();

    void operator<<(char value) override;
    void operator<<(bool& value) override;
    void operator<<(ubyte& value) override;
    void operator<<(int& value) override;
    void operator<<(uint& value) override;
    void operator<<(float& value) override;

    std::string Result() const;

private:
    bool IsSaving() const override { return true; }

    std::ostringstream m_stream;
};

} // namespace hatcher
