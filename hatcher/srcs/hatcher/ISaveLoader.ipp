namespace hatcher
{

void ISaveLoader::operator<<(std::string& string)
{
    int size = string.size();
    *this << size;
    string.resize(size);
    separator(' ');
    separator('"');
    for (char& value : string)
    {
        *this << value;
    }
    separator('"');
}

template <typename T>
void ISaveLoader::operator<<(std::optional<T>& optional)
{
    bool hasValue = optional.has_value();
    *this << hasValue;
    separator(' ');
    if (hasValue)
    {
        T value = *optional;
        *this << value;
        optional = value;
    }
}

template <typename T>
void ISaveLoader::operator<<(std::vector<T>& vector)
{
    int size = vector.size();
    *this << size;
    vector.resize(size);
    separator(' ');
    separator('[');
    for (auto& value : vector)
    {
        *this << value;
        separator(',');
    }
    separator(']');
}

template <class Key, class T, class Hash>
void ISaveLoader::operator<<(std::unordered_map<Key, T, Hash>& map)
{
    if (!IsSaving())
        map = {};

    int size = map.size();
    *this << size;
    separator(' ');
    separator('[');
    for (int i = 0; i < size; i++)
    {
        if (IsSaving())
        {
            auto it = map.begin();
            std::advance(it, i);
            Key key = it->first;
            T value = it->second;
            *this << key;
            separator(';');
            *this << value;
            separator(',');
        }
        else
        {
            Key key;
            T value;
            *this << key;
            separator(';');
            *this << value;
            separator(',');
            map.insert({key, value});
        }
    }
    separator(']');
}

template <glm::length_t L, typename T>
void ISaveLoader::operator<<(glm::vec<L, T>& vector)
{
    for (int i = 0; i < L; i++)
    {
        if (i > 0)
            separator(' ');
        *this << vector[i];
    }
    separator('\n');
}

template <glm::length_t L, typename T>
void ISaveLoader::operator<<(Box<L, T>& box)
{
    *this << box.Min();
    separator(' ');
    *this << box.Max();
    separator('\n');
}

} // namespace hatcher
