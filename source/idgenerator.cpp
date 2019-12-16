#include "kpl/idgenerator.h"

namespace kpl {

    IDGenerator::IDGenerator()
        : mNextID { 1 }
    {

    }

    uint16_t IDGenerator::getID(const std::string& key)
    {
        if( mMap.count(key) == 0 )
            mMap.insert( std::pair<std::string, uint16_t>(key, mNextID++) );

        return mMap[key];
    }
}

