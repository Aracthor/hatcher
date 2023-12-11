#include "Lives.hpp"

#include "hatcher/ComponentLoader.hpp"
#include "hatcher/ComponentRegisterer.hpp"
#include "hatcher/ComponentSaver.hpp"

using namespace hatcher;

void Lives::Save(ComponentSaver& saver) const
{
    saver << remaining;
}

void Lives::Load(ComponentLoader& loader)
{
    loader >> remaining;
}

namespace
{
WorldComponentTypeRegisterer<Lives, EComponentList::Gameplay> registerer;
} // namespace
