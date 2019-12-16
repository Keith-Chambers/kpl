#include "kpl/reflection/logicaldatatype.h"

#include "kpl/reflection/variable.h"

namespace kpl {
    namespace reflection
    {

        LogicalDataType::LogicalDataType(std::string name, std::vector<Variable> memberList,
                                         const std::string aliasFor)
            : DataType(name, calculateSizeBits(memberList), aliasFor, false), mMemberVariables { memberList }
        {  }

        LogicalDataType::LogicalDataType(std::string name, std::vector<Variable> memberList)
            : DataType (name, calculateSizeBits(memberList), false), mMemberVariables { memberList }
        {  }

        LogicalDataType::LogicalDataType(std::string name, size_t szBits)
            : DataType (name, szBits, true), mMemberVariables {}
        {  }

        LogicalDataType::LogicalDataType(std::string name, size_t szBits, const std::string aliasFor)
            : DataType (name, szBits, aliasFor, true), mMemberVariables {}
        {  }

        LogicalDataType::LogicalDataType(const DataType & dataType)
            : DataType (dataType)
        {  }

        size_t LogicalDataType::calculateSizeBits(std::vector<Variable> memberList)
        {
            size_t result = 0;

            for(const Variable& var : memberList)
                result += var.dataTypePtr()->sizeBits();

            return result;
        }

        LogicalDataType LogicalDataType::makeAlias(std::string newName, const LogicalDataType& dataType)
        {
            if(dataType.isPrimative())
            {
                std::vector<Variable> vars;
                if(! dataType.getMemberList(vars) )
                {
                    std::cout << "Error: Failed to get member list of checked non-primative LogicalDataType" << std::endl;
                    // TODO: Many use an optional instead of crashing the program
                    exit(EXIT_FAILURE);
                }

                return LogicalDataType (newName, vars, dataType.name());
            }

            /* Primative */
            return LogicalDataType (newName, dataType.sizeBits(), dataType.name());
        }

        bool LogicalDataType::memberExists(std::string name) const
        {
            for(const Variable& var : mMemberVariables)
                if(name == var.name())
                    return true;

            return false;
        }

        bool LogicalDataType::getMemberList(std::vector<Variable>& memberListOutput) const
        {
            if(this->isPrimative())
                return false;

            for(const Variable& var : mMemberVariables)
                memberListOutput.push_back(var);

            return true;
        }
    }
}
