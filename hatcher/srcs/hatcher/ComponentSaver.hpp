#pragma once

#include <sstream>

#include "ISaveLoader.hpp"

namespace hatcher
{

class ComponentSaver final : public ISaveLoader
{
public:
    void operator<<(char value) override;
    void operator<<(bool& value) override;
    void operator<<(int& value) override;
    void operator<<(uint& value) override;
    void operator<<(float& value) override;

    std::string Result() const;

private:
    std::ostringstream m_stream;
};

} // namespace hatcher
