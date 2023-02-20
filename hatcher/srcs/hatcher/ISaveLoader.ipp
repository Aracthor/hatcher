namespace hatcher
{

template <typename T>
void ISaveLoader::operator<<(std::optional<T>& optional)
{
    bool hasValue = optional.has_value();
    *this << hasValue;
    *this << ' ';
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
    *this << ' ';
    *this << '[';
    for (auto& value : vector)
    {
        *this << value;
        *this << ',';
    }
    *this << ']';
}

template <class Key, class T, class Hash>
void ISaveLoader::operator<<(std::unordered_map<Key, T, Hash>& map)
{
    if (!IsSaving())
        map = {};

    int size = map.size();
    *this << size;
    *this << ' ';
    *this << '[';
    for (int i = 0; i < size; i++)
    {
        if (IsSaving())
        {
            auto it = map.begin();
            std::advance(it, i);
            Key key = it->first;
            T value = it->second;
            *this << key;
            *this << ';';
            *this << value;
            *this << ',';
        }
        else
        {
            Key key;
            T value;
            *this << key;
            *this << ';';
            *this << value;
            *this << ',';
            map.insert({key, value});
        }
    }
    *this << ']';
}

template <glm::length_t L, typename T>
void ISaveLoader::operator<<(glm::vec<L, T>& vector)
{
    for (int i = 0; i < L; i++)
    {
        if (i > 0)
            *this << ' ';
        *this << vector[i];
    }
    *this << '\n';
}

template <glm::length_t L, typename T>
void ISaveLoader::operator<<(Box<L, T>& box)
{
    *this << box.Min();
    *this << ' ';
    *this << box.Max();
    *this << '\n';
}

} // namespace hatcher
