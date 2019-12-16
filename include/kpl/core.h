#ifndef CORE_H
#define CORE_H

#include <stdint.h>

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;
using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

//#define KPL_USE_QT 1
//#define KPL_USE_STL 0
//#define KPL_USE_KPL 0

#if(KPL_USE_QT)

    #include <QString>
    #include <QDir>
    #include <QDirIterator>

#else
    #include <string>
    #include <filesystem>
    #include <vector>
#endif

namespace kpl
{
    #if(KPL_USE_QT)
        using String = QString;
        using Directory = QDir;

        template<typename T>
        using List = QList<T>;
    #else
        using String = std::string;
        using Directory = std::filesystem::directory_entry;

        template<typename T>
        using List = std::vector<T>;
    #endif
}

#endif // CORE_H
