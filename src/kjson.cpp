#include "kjson.h"

namespace kpl
{
    namespace rapidjson {

        bool requireMember(::rapidjson::Value& obj, std::string memberName)
        {
            if(! obj.HasMember(memberName.c_str()) )
            {
                std::cout << "Error: " << memberName << " is a required attribute in current object" << std::endl;
                return false;
            }

            return true;
        }

        bool parseDocumentFromFile(std::string filePath, ::rapidjson::Document& document)
        {
            if(! kpl::fileExists(filePath) )
            {
                return false;
            } else {
                std::cout << "File exists" << std::endl;
            }


            /* Load a file */
            std::string buffer = kpl::loadTextFile(filePath);

            if(document.Parse(buffer.c_str()).HasParseError())
            {
                std::cout << "Failed to parse " << filePath << std::endl;
                return false;
            }

            return true;
        }

        bool checkForMemberList(::rapidjson::Document& document, const char ** memberList, size_t listLength)
        {
            for(size_t i = 0; i < listLength; i++)
            {
                if(! document.HasMember(*(memberList + i)) )
                {
                    std::cout << "Parse Err: Attribute '" << *(memberList + i) << "' is required." << std::endl;
                    return false;
                }
            }

            return true;
        }

        bool parseStringArray(::rapidjson::Value & root, std::vector<std::string>& outputArray)
        {
            if(! root.IsArray())
                return false;

            for(::rapidjson::SizeType i = 0; i < root.Size(); i++)
                outputArray.push_back( root[i].GetString() );

            return true;
        }




    }
}

