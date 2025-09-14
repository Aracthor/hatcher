#include "Blueprint.hpp"

#include "hatcher/ComponentRegisterer.hpp"
#include "hatcher/DataLoader.hpp"
#include "hatcher/DataSaver.hpp"

void Blueprint::Save(DataSaver& saver) const
{
    saver << position;
    saver << active;
    saver << possible;
}

void Blueprint::Load(DataLoader& loader)
{
    loader >> position;
    loader >> active;
    loader >> possible;
}

namespace
{
WorldComponentTypeRegisterer<Blueprint, EComponentList::Rendering> registerer;
} // namespace
