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
                        std::vector<Variable>& parameters);

            bool hasParameters() const;
            bool hasVoidReturnValue() const;
            std::string name() const;
            std::string returnValueString() const;
            std::string parameterListString() const;

        private:
            const std::string mName;
            const DataType mReturnValue;
            const std::vector<Variable> mParameters;
        };
    }
}

#endif // FUNCTION_H
