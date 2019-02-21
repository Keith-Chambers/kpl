#include "eventsystem.h"

namespace kpl
{

    EventSystem * EventSystem::mInstance = nullptr;

    EventSystem::EventSystem()
    {
    }

    EventSystem::~EventSystem()
    {
    }

    EventSystem *EventSystem::getInstance()
    {
        if(!mInstance)
            return new EventSystem;
        return mInstance;
    }

    bool EventSystem::registerEvent(std::string pName)
    {
        if(mEventBindings[pName])
            return false;
            
        mEventBindings[pName] = new std::vector<EventNotifier*>;
        
        if(!mEventBindings[pName])
        {
            std::cout << "Failed to allocate vector for mEventBindings. Out of Memory?" << std::endl;
            exit(EXIT_FAILURE);
        }
        
        return true;
    }

    bool EventSystem::subscribeForEvent(std::string pName, EventNotifier * pEventNotifier)
    {
        if(!mEventBindings[pName])
            return false;
            
        mEventBindings[pName]->push_back(pEventNotifier);
        std::cout << "Added subscriber for " << pName << std::endl;
        
        return true;
    }

    bool EventSystem::triggerEvent(std::string pName)
    {
        if(!mEventBindings[pName])
        {
            std::cout << "No subscribers for event" << std::endl;
            return false;
        }
        
        std::cout << mEventBindings[pName]->size() << " subscriber(s) for event.." << std::endl;
            
        for(std::size_t i = 0; i < mEventBindings[pName]->size(); i++)
            mEventBindings[pName]->at(i)->notify();
            
        return true;
    }

}
