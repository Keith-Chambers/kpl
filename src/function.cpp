#include "kpl/function.h"

namespace kpl {
    namespace reflection {

        Function::Function( const std::string name,
                            const DataType returnValue,
                            std::vector<Variable> parameters)
            : mName { name },
              mReturnValue { returnValue },
              mParameters { parameters }

        {  }

        bool Function::hasParameters() const
        {
            return ( ! mParameters.empty() ) ? true : false;
        }

        bool Function::hasVoidReturnValue() const
        {
            if(mReturnValue.name() == "void")
                return true;

            return false;
        }

        const kpl::reflection::DataType& Function::returnDataType() const
        {
            return mReturnValue;
        }

        bool Function::operator==(const Function& func) const
        {
            return mReturnValue.name() == func.returnValueString() && parameterListString() == func.parameterListString();
        }

        bool Function::operator!=(const Function& func) const
        {
            return mReturnValue.name() != func.returnValueString() || parameterListString() != func.parameterListString();
        }

        bool Function::operator<(const Function& func) const
        {
            if(mName < func.name())
                return true;

            if(mName > func.name())
                return false;

            if(parameterListString() < func.parameterListString())
                return true;

            return false;
        }

        bool Function::operator>(const Function& func) const
        {
            if(mName > func.name())
                return true;

            if(mName < func.name())
                return false;

            if(parameterListString() > func.parameterListString())
                return true;

            return false;
        }

        std::string Function::name() const
        {
            return mName;
        }


        std::string Function::returnValueString() const
        {
            return mReturnValue.name();
        }

        std::string Function::parameterListString() const
        {
            std::string result;

            for(const kpl::reflection::Variable& var : mParameters)
            {
                result.append(var.dataTypePtr()->name() + " ");
                result.append(var.name() + ",");
            }

            // Remove trailing comma
            if(mParameters.size() > 0)
                result.erase(result.size() - 1);

            return result;
        }
    }
}
