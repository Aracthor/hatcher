#include "ComponentManager.hpp"

namespace hatcher
{
template <class Component>
std::optional<Component>& EntityEgg::GetComponent()
{
    return m_componentManager->WriteComponents<Component>()[m_temporaryEntityID];
}
} // namespace hatcher
