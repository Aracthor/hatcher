namespace hatcher
{
template <class Component>
ComponentReader<Component> ComponentAccessor::ReadComponents() const
{
    using RealComponentList = const IdentifiableComponentList<Component>*;

    const IComponentList* componentList = m_componentManager->GetComponentList<Component>();
    RealComponentList realComponentList = reinterpret_cast<RealComponentList>(componentList);
    return realComponentList->GetComponentList();
}

template <class Component>
ComponentWriter<Component> ComponentAccessor::WriteComponents()
{
    using RealComponentList = IdentifiableComponentList<Component>*;

    IComponentList* componentList = m_componentManager->GetComponentList<Component>();
    RealComponentList realComponentList = reinterpret_cast<RealComponentList>(componentList);
    return realComponentList->GetComponentList();
}

template <class Component>
const Component* ComponentAccessor::ReadWorldComponent() const
{
    return checked_cast<const Component*>(m_componentManager->GetWorldComponent<Component>());
}

template <class Component>
Component* ComponentAccessor::WriteWorldComponent()
{
    return checked_cast<Component*>(m_componentManager->GetWorldComponent<Component>());
}
} // namespace hatcher
