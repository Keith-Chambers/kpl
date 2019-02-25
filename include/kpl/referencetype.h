#ifndef REFERENCETYPE_H
#define REFERENCETYPE_H

#include <kpl/datatype.h>

namespace kpl {
    namespace reflection {

        class ReferenceType : public kpl::reflection::DataType
        {
        public:
            ReferenceType(kpl::reflection::DataType dataType);
            DataType getSubType() const;

        private:
            kpl::reflection::DataType mDataType;
        };
    }
}



#endif // REFERENCETYPE_H
