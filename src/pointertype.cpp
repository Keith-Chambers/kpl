#include "kpl/pointertype.h"

namespace kpl {
    namespace reflection {

        uint8_t kpl::reflection::PointerType::POINTER_SZ_BITS = 64;

        PointerType::PointerType(const kpl::reflection::DataType& dataType)
            : kpl::reflection::DataType(dataType.name() + "*", POINTER_SZ_BITS, true, true, false), mDataType { dataType }
        {

        }

        DataType PointerType::getSubType() const
        {
            return mDataType;
        }
    }
}
