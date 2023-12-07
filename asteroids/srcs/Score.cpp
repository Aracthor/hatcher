#include "Score.hpp"

#include "hatcher/ComponentRegisterer.hpp"
#include "hatcher/ISaveLoader.hpp"

using namespace hatcher;

void Score::SaveLoad(ISaveLoader& saveLoader)
{
    saveLoader << points;
}

namespace
{
WorldComponentTypeRegisterer<Score, EComponentList::Gameplay> registerer;
} // namespace
