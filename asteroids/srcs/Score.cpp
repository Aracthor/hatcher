#include "Score.hpp"

#include "hatcher/ComponentLoader.hpp"
#include "hatcher/ComponentRegisterer.hpp"
#include "hatcher/ComponentSaver.hpp"

using namespace hatcher;

void Score::Save(ComponentSaver& saver) const
{
    saver << points;
}

void Score::Load(ComponentLoader& loader)
{
    loader >> points;
}

namespace
{
WorldComponentTypeRegisterer<Score, EComponentList::Gameplay> registerer;
} // namespace
