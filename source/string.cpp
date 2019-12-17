#include <kpl/string.h>

namespace kpl
{

QString viewToString(kpl::StringView string)
{
    kpl::Char tmp = *string.end();
    kpl::Char* end = const_cast<kpl::Char*>(string.end());

    *end = '\0';

    QString result(string.begin());
    result.append(tmp);

    *end = tmp;

    return result;
}

kpl::String trimEnd(const kpl::String& string, kpl::Char remove_char)
{
    kpl::String result = string;

    kpl::String::const_reverse_iterator sentinal = string.rbegin();
    u32 truncate = 0;

    for(kpl::String::const_reverse_iterator reverse_itr = result.rend(); reverse_itr != sentinal; reverse_itr++ )
    {
        if(*reverse_itr == remove_char) {
            break;
        }

        truncate++;
    }

    result.truncate(truncate);

    return result;
}

kpl::String& trimEndMut(kpl::String& string, kpl::Char remove_char)
{
    kpl::String::reverse_iterator sentinal = string.rbegin();
    u32 truncate = 0;

    for(kpl::String::reverse_iterator reverse_itr = string.rend(); reverse_itr != sentinal; reverse_itr++ )
    {
        if(*reverse_itr == remove_char) {
            break;
        }

        truncate++;
    }

    string.truncate(truncate);

    return string;
}

kpl::StringView stringToView(const kpl::String& string)
{
    u32 size = string.size();
    return kpl::StringView { string.data(), size };
}

kpl::StringView forwardUntilView(kpl::StringView string, const kpl::Char sentinal)
{
    return kpl::head(string, kpl::countElementsFromBegin(string, sentinal));
}

kpl::String forwardUntil(kpl::StringView string, const kpl::Char sentinal)
{
    return viewToString(kpl::head(string, kpl::countElementsFromBegin(string, sentinal)));
}

kpl::StringView backwardUntilView(kpl::StringView string, const kpl::Char sentinal)
{
    return kpl::tail(string, kpl::countElementsFromEnd(string, sentinal));
}

kpl::String backwardUntil(kpl::StringView string, const kpl::Char sentinal)
{
    return viewToString(kpl::tail(string, kpl::countElementsFromEnd(string, sentinal)));
}

kpl::String forwardUntilLast(const kpl::String& string, const kpl::Char find)
{
    u32 truncate_from_end = kpl::countElementsFromEnd(stringToView(string), find);
    return string.right(string.size() - truncate_from_end);
}

kpl::String backwardUntilFirst(const kpl::String& string, const kpl::Char find)
{
    u32 truncate_from_begin = kpl::countElementsFromBegin(stringToView(string), find);
    return string.left(string.size() - truncate_from_begin);
}
}
