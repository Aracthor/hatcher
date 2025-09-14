#include "Blueprint.hpp"

#include "hatcher/ComponentRegisterer.hpp"
#include "hatcher/DataLoader.hpp"
#include "hatcher/DataSaver.hpp"

void Blueprint::Save(DataSaver& saver) const
{
    saver << position;
    saver << active;
}

void Blueprint::Load(DataLoader& loader)
{
    loader >> position;
    loader >> active;
}

namespace
{
WorldComponentTypeRegisterer<Blueprint, EComponentList::Rendering> registerer;
} // namespace
