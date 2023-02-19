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
