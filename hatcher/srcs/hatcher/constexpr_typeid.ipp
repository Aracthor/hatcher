namespace hatcher
{

template <class t>
constexpr string_view constexpr_typeid()
{
    const char* prettyFunctionName = __PRETTY_FUNCTION__;
    int classNameStart = 2;
    while (prettyFunctionName[classNameStart - 2] != '=')
        classNameStart++;
    int classNameEnd = classNameStart + 1;
    while (prettyFunctionName[classNameEnd] != ']' && prettyFunctionName[classNameEnd] != ';')
        classNameEnd++;
    return string_view({&prettyFunctionName[classNameStart], classNameEnd - classNameStart});
}

} // namespace hatcher
