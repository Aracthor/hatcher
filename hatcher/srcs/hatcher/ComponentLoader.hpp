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

    void operator<<(float& value) override;
    void operator<<(glm::vec3& value) override;

private:
    std::istringstream m_stream;
};

} // namespace hatcher
