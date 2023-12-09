#include "MeshComponent.hpp"

#include "hatcher/ComponentRegisterer.hpp"

using namespace hatcher;

namespace
{
ComponentTypeRegisterer<MeshComponent, EComponentList::Rendering> registerer;
} // namespace
