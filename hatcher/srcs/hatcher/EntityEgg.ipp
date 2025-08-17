#include "ComponentAccessor.hpp"

namespace hatcher
{
template <class Component>
std::optional<Component>& EntityEgg::GetComponent()
{
    return m_componentAccessor->WriteComponents<Component>()[m_temporaryEntityID];
}

template <class Component>
std::optional<Component>& EntityEgg::GetRenderingComponent()
{
    return m_renderingComponentAccessor->WriteComponents<Component>()[m_temporaryEntityID];
}
} // namespace hatcher
