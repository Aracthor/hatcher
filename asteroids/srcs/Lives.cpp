#include "Lives.hpp"

#include "hatcher/ComponentRegisterer.hpp"
#include "hatcher/ISaveLoader.hpp"

using namespace hatcher;

void Lives::SaveLoad(ISaveLoader& saveLoader)
{
    saveLoader << remaining;
}

namespace
{
WorldComponentTypeRegisterer<Lives, EComponentList::Gameplay> registerer;
} // namespace
