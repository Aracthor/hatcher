#include "Lives.hpp"

#include "hatcher/ComponentRegisterer.hpp"
#include "hatcher/DataLoader.hpp"
#include "hatcher/DataSaver.hpp"

using namespace hatcher;

void Lives::Save(DataSaver& saver) const
{
    saver << remaining;
    saver << cyclesToWait;
}

void Lives::Load(DataLoader& loader)
{
    loader >> remaining;
    loader >> cyclesToWait;
}

namespace
{
WorldComponentTypeRegisterer<Lives, EComponentList::Gameplay> registerer;
} // namespace
