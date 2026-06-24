template <std::size_t L, typename T>
std::ostream& operator<<(std::ostream& stream, hatcher::Vec<L, T> vect)
{
    stream << "(";
    for (std::size_t i = 0; i < L; i++)
    {
        if (i > 0)
            stream << ",";
        stream << vect[i];
    }
    stream << ")";
    return stream;
}

template <std::size_t L, typename T>
std::ostream& operator<<(std::ostream& stream, const hatcher::Mat<L, T>& mat)
{
    stream << "[" << std::endl;
    for (std::size_t i = 0; i < L; i++)
        stream << mat[i] << std::endl;
    stream << "]";
    return stream;
}
