#include "ISaveLoader.hpp"

namespace hatcher
{

void ISaveLoader::operator<<(EntityDescriptorID& id)
{
    SaveLoadData(&id, sizeof(id));
}

void ISaveLoader::operator<<(RandomGenerator& randomGenerator)
{
    SaveLoadData(&randomGenerator, sizeof(randomGenerator));
}

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
