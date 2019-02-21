#include "kpl/function.h"

namespace kpl {
    namespace reflection {

        Function::Function( const std::string name,
                            const DataType returnValue,
                            std::vector<Variable>& parameters,
                            const std::string& nodeName,
                            const std::string& nodeNamespace)
            : mName { name },
              mReturnValue { returnValue },
              mParameters { parameters },
              mNodeName { nodeName },
              mNodeNamespace { nodeNamespace }

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

        std::string Function::name() const
        {
            return mName;
        }
    }
}
