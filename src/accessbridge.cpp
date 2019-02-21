#include "accessbridge.h"

namespace kpl
{

    AccessBridge * AccessBridge::mInstance = nullptr;

    AccessBridge::AccessBridge()
        : mDataSourceMappings()
    {
        
    }

    AccessBridge::~AccessBridge()
    {
    }

    AccessBridge *AccessBridge::getInstance()
    {
        if(!mInstance)
            mInstance = new AccessBridge; 
            
        return mInstance;
    }

    bool AccessBridge::registerDataSource(std::string pName, DataSource * pDataSource)
    {
        if(mDataSourceMappings[pName])
            return false;
            
        mDataSourceMappings[pName] = pDataSource;
        
        return true;
    }

    void *AccessBridge::fetchFromDataSource(std::string pName)
    {
        if(!mDataSourceMappings[pName])
            return nullptr;
        return mDataSourceMappings[pName]->get();
    }
}
