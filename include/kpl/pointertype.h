#ifndef POINTERTYPE_H
#define POINTERTYPE_H

#include <string>

#include <kpl/datatype.h>

namespace kpl {
    namespace reflection {

        class PointerType : public kpl::reflection::DataType
        {
        public:
            PointerType(const kpl::reflection::DataType& dataType);
            DataType getSubType() const;

            static uint8_t POINTER_SZ_BITS;
        private:
            kpl::reflection::DataType mDataType;
        };
    }
}



#endif // POINTERTYPE_H
