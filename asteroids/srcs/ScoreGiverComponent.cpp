#include "ScoreGiverComponent.hpp"

#include "hatcher/ComponentRegisterer.hpp"
#include "hatcher/ISaveLoader.hpp"

using namespace hatcher;

void operator<<(ISaveLoader& saveLoader, ScoreGiverComponent& component)
{
    saveLoader << component.points;
}

namespace
{
ComponentTypeRegisterer<ScoreGiverComponent, EComponentList::Gameplay> registerer;
} // namespace
