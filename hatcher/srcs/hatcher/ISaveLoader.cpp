#include "ISaveLoader.hpp"

namespace hatcher
{

void ISaveLoader::operator<<(std::string& string)
{
    int size = string.size();
    *this << size;
    string.resize(size);
    for (char& value : string)
    {
        *this << value;
    }
}

} // namespace hatcher
