#pragma once

#include <sstream>

#include "ISaveLoader.hpp"

namespace hatcher
{

class ComponentSaver final : public ISaveLoader
{
public:
    void operator<<(float& value) override;
    void operator<<(glm::vec3& value) override;

    std::string Result() const;

private:
    std::ostringstream m_stream;
};

} // namespace hatcher
