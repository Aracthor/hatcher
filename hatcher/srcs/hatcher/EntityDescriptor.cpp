#include "EntityDescriptor.hpp"

namespace hatcher
{

namespace
{
class EntityDescriptor final : public IEntityDescriptor
{
public:
    EntityDescriptor(const std::vector<ubyte>& componentData, const std::vector<ubyte>& renderingComponentData)
        : m_componentData(componentData)
        , m_renderingComponentData(renderingComponentData)
    {
    }

    const std::vector<ubyte>& GetComponentData() const override { return m_componentData; }
    const std::vector<ubyte>& GetRenderingComponentData() const override { return m_renderingComponentData; }

private:
    const std::vector<ubyte> m_componentData;
    const std::vector<ubyte> m_renderingComponentData;
};

} // namespace

std::vector<ubyte> ComponentDescriptorList::Result() const
{
    ComponentSaver header;
    int componentCount = m_componentCount;
    header << componentCount;
    header.separator('\n');
    std::vector<ubyte> result = header.Result();
    result.insert(result.end(), m_saver.Result().begin(), m_saver.Result().end());
    return result;
}

void RegisterEntityDescriptor(EntityDescriptorID id, IEntityDescriptor* descriptor);

EntityDescriptorRegisterer::EntityDescriptorRegisterer(EntityDescriptorID id,
                                                       const ComponentDescriptorList& componentList,
                                                       const ComponentDescriptorList& renderingComponentList)
{
    RegisterEntityDescriptor(id, new EntityDescriptor(componentList.Result(), renderingComponentList.Result()));
}

} // namespace hatcher
