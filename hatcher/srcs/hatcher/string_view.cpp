#include "string_view.hpp"

namespace hatcher
{

std::ostream& operator<<(std::ostream& stream, const string_view& stringView)
{
    stream.write(stringView.data, stringView.size);
    return stream;
}

} // namespace hatcher
