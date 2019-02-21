#include "variable.h"

namespace kpl {
    namespace reflection
    {

        Variable::Variable(std::string name, const DataType dataType, uint16_t min, uint16_t max)
            : mName { name }, mDataType { dataType }, mMin { min }, mMax { max }
        {  }

        Variable::Variable(std::string name, const LogicalDataType dataType, uint16_t min, uint16_t max)
            : mName { name }, mDataType { dataType }, mMin { min }, mMax { max }
        {  }

        void Variable::print(const std::string&& prefix) const
        {
            std::cout << prefix << "name: " << mName << '\n';
            std::cout << prefix << "type: " << typeName() << '\n';
            std::cout << prefix << "primative: " << mDataType.isPrimative() << '\n';
            std::cout << prefix << "cardinality min: " << mMin << '\n';
            std::cout << prefix << "cardinality max: " << mMax << '\n';
        }

        bool Variable::isArray() const
        {
            if(getCardinalityMin() == 1 && getCardinalityMax() == 1)
                return false;

            return true;
        }

        std::string Variable::name() const
        {
            return mName;
        }

        std::string Variable::typeName() const
        {
            return mDataType.name();
        }

        const DataType * Variable::dataTypePtr() const
        {
            return static_cast<const DataType*>(&mDataType);
        }

        uint16_t Variable::getCardinalityMax() const
        {
            return mMax;
        }

        uint16_t Variable::getCardinalityMin() const
        {
            return mMin;
        }
    }
}
