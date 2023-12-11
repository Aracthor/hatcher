namespace hatcher
{

template <typename T>
void ComponentLoader::operator>>(T& value)
{
    static_assert(std::is_trivially_copyable_v<T>);
    LoadData(&value, sizeof(value));
}

template <typename T>
void ComponentLoader::operator>>(std::vector<T>& vector)
{
    int size;
    *this >> size;
    vector.resize(size);
    for (int i = 0; i < size; i++)
        *this >> vector[i];
}

template <class Key, class T, class Hash>
void ComponentLoader::operator>>(std::unordered_map<Key, T, Hash>& map)
{
    map = {};

    int size;
    *this >> size;
    for (int i = 0; i < size; i++)
    {
        Key key;
        T value;
        *this >> key;
        *this >> value;
        map.insert({key, value});
    }
}

} // namespace hatcher
