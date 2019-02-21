#ifndef KDATABASE_H
#define KDATABASE_H

#include <sqlite3.h>
#include <string>
#include <iostream>

#include "kfile.h"

namespace kpl
{
    namespace sqlite
    {
        sqlite3 * openSqliteDatabase(std::string databasePath);
        bool createDatabaseTablesFromFile(sqlite3 * database, std::string filePath);
    }
}

#endif // KDATABASE_H
