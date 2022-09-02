#include <algorithm> // std::clamp

#include "Camera.hpp"

#include "hatcher/ComponentManager.hpp"
#include "hatcher/Graphics/IEventListener.hpp"
#include "hatcher/assert.hpp"

namespace
{

using namespace hatcher;

const int dummy = RegisterEventListener<CameraEventListener>("Camera");

} // namespace