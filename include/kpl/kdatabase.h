#ifndef KDATABASE_H
#define KDATABASE_H

#include <sqlite3.h>
#include <string>
#include <iostream>
#include <vector>
#include <utility>
#include <functional>

#include "kpl/kfile.h"

namespace kpl
{
    namespace sqlite
    {
        sqlite3 * openSqliteDatabase(std::string databasePath);
        bool createDatabaseTablesFromFile(sqlite3 * database, std::string filePath);
        bool insertInto(sqlite3 * database, const std::string& tableName, const std::vector<std::pair<std::string, std::string>>& parameters);
        bool select(sqlite3 * database, const std::string& tableName, const std::vector<std::string>& selectFields, int (*callback)(void*,int,char**,char**), void *callbackData);
    }
}

#endif // KDATABASE_H
