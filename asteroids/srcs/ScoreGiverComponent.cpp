#include "ScoreGiverComponent.hpp"

#include "hatcher/ComponentRegisterer.hpp"

using namespace hatcher;

namespace
{
ComponentTypeRegisterer<ScoreGiverComponent, EComponentList::Gameplay> registerer;
} // namespace
