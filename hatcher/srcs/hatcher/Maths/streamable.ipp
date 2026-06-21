template <std::size_t L, typename T>
std::ostream& operator<<(std::ostream& stream, hatcher::Vect<L, T> vect)
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
