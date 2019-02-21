#ifndef VARIABLE_H
#define VARIABLE_H

#include "logicaldatatype.h"
#include "datatype.h"

/* Variable - Represents a named data type used in code
 * Supports cardinality so that arrays can be defined.
 */

namespace kpl {
    namespace reflection
    {

        enum Cardinality { INFINITE = UINT16_MAX };

        class Variable
        {
        public:
            Variable(std::string name, const DataType dataType, uint16_t min = 1, uint16_t max = 1);
            Variable(std::string name, const LogicalDataType dataType, uint16_t min = 1, uint16_t max = 1);

            void print(const std::string&& prefix = "") const;

            bool isArray() const;
            std::string name() const;
            std::string typeName() const;
            const DataType * dataTypePtr() const;

            uint16_t getCardinalityMax() const;
            uint16_t getCardinalityMin() const;

        private:
            const std::string mName;
            const LogicalDataType mDataType;
            uint16_t mMin;
            uint16_t mMax;
        };
    }
}

#endif // KVARIABLE_H
