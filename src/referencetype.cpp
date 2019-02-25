#include "kpl/referencetype.h"

namespace kpl {
    namespace reflection {

        ReferenceType::ReferenceType(kpl::reflection::DataType dataType)
            : kpl::reflection::DataType( dataType.name() + "&", dataType.sizeBits(), dataType.isPrimative(), false, true ), mDataType { dataType }
        {

        }

        DataType ReferenceType::getSubType() const
        {
            return mDataType;
        }
    }
}


