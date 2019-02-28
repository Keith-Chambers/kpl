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
            //if(mReturnValue.name() == VOID_TYPE_NAME)
            if(mReturnValue.name() == "void")
                return true;

            return false;
        }

        bool Function::operator==(const Function& func) const
        {
            return mReturnValue.name() == func.returnValueString() && parameterListString() == func.parameterListString();
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
            result.erase(result.size() - 1);

            return result;
        }
    }
}
