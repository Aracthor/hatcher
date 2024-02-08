#include "Score.hpp"

#include "hatcher/ComponentRegisterer.hpp"
#include "hatcher/DataLoader.hpp"
#include "hatcher/DataSaver.hpp"

using namespace hatcher;

void Score::Save(DataSaver& saver) const
{
    saver << points;
}

void Score::Load(DataLoader& loader)
{
    loader >> points;
}

namespace
{
WorldComponentTypeRegisterer<Score, EComponentList::Gameplay> registerer;
} // namespace
