#include "EntityDescriptor.hpp"

namespace hatcher
{

namespace
{
class EntityDescriptor final : public IEntityDescriptor
{
public:
    EntityDescriptor(const std::string& componentData, const std::string& renderingComponentData)
        : m_componentData(componentData)
        , m_renderingComponentData(renderingComponentData)
    {
    }

    const std::string& GetComponentData() const override { return m_componentData; }
    const std::string& GetRenderingComponentData() const override { return m_renderingComponentData; }

private:
    const std::string m_componentData;
    const std::string m_renderingComponentData;
};

} // namespace

std::string ComponentDescriptorList::Result() const
{
    ComponentSaver header;
    int componentCount = m_componentCount;
    header << componentCount;
    header.separator('\n');
    return header.Result() + m_saver.Result();
}

unique_ptr<IEntityDescriptor> CreateEntityDescriptor(const ComponentDescriptorList& componentList,
                                                     const ComponentDescriptorList& renderingComponentList)
{
    return hatcher::make_unique<EntityDescriptor>(componentList.Result(), renderingComponentList.Result());
}
} // namespace hatcher
