#pragma once

#include "hatcher/IWorldComponent.hpp"

class GridDisplay final : public hatcher::IWorldComponent
{
public:
    bool Enabled() const { return m_enabled; }

    void SetEnabled(bool enabled) { m_enabled = enabled; }

private:
    bool m_enabled = false;
};
