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

IEntityDescriptor* EntityDescriptorBuilder::CreateDescriptor()
{
    ComponentSaver header;
    ComponentSaver renderingHeader;
    header << m_componentCount;
    header.separator('\n');
    renderingHeader << m_renderingComponentCount;
    renderingHeader.separator('\n');
    return new EntityDescriptor(header.Result() + m_saver.Result(),
                                renderingHeader.Result() + m_renderingSaver.Result());
}
} // namespace hatcher
