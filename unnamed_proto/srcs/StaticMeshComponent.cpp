#include "StaticMeshComponent.hpp"

#include "hatcher/ComponentRegisterer.hpp"

using namespace hatcher;

namespace
{
ComponentTypeRegisterer<StaticMeshComponent, EComponentList::Rendering> registerer;
} // namespace
