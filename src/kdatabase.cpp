#include "kdatabase.h"

namespace kpl {
    namespace sqlite {

        sqlite3 * openSqliteDatabase(std::string databasePath)
        {
            std::cout << "Attempting to create db at " << databasePath << std::endl;

            sqlite3 * result;
            if( sqlite3_open(databasePath.c_str(), &result) == 0 )
                return result;

            std::cout << "Failed to open " << databasePath << std::endl;

            return nullptr;
        }

        // TODO: Doesn't work with multiple CREATE TABLE statements
        bool createDatabaseTablesFromFile(sqlite3 * database, std::string filePath)
        {
            std::string createSqlString = kpl::loadTextFile(filePath);
            sqlite3_stmt * statement;
            const char * unused = nullptr;

            int rc = sqlite3_prepare_v2(database,
                                        createSqlString.c_str(),
                                        static_cast<int>(createSqlString.size()) + 1,
                                        &statement,
                                        &unused);
            if(rc != SQLITE_OK)
            {
                std::cout << "Failed to prepare DB command" << std::endl;
                return false;
            }

            rc = sqlite3_step(statement);

            if(rc != SQLITE_DONE)
            {
                std::cout << "Failed to execute create DB command" << std::endl;
                return false;
            }

            sqlite3_finalize(statement);

            return true;
        }
    }
}

