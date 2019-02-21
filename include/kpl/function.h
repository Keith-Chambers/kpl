#ifndef FUNCTION_H
#define FUNCTION_H

#include <vector>
#include <string>

#include "kpl/datatype.h"
#include "kpl/variable.h"

namespace kpl {
    namespace reflection {

        class Function
        {
        public:
            Function(   const std::string name,
                        const DataType returnValue,
                        std::vector<Variable>& parameters,
                        const std::string& nodeName,
                        const std::string& nodeNamespace = "");

            bool hasParameters() const;
            bool hasVoidReturnValue() const;
            std::string name() const;

        private:
            const std::string mName;
            const DataType mReturnValue;
            const std::vector<Variable> mParameters;

            const std::string& mNodeName;
            const std::string& mNodeNamespace;
        };
    }
}



#endif // FUNCTION_H
