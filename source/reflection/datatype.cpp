#include "kpl/reflection/datatype.h"

namespace kpl {
    namespace reflection
    {
        DataType::DataType(std::string name, size_t szBits, bool isPrimative, bool isPointer, bool isReference)
            : mName { name },
              mSzBits { szBits },
              mIsAlias { false },
              mIsPrimative { isPrimative },
              mAliasFor { std::nullopt },
              mIsPointer { isPointer },
              mIsReference { isReference }
        {  }

        DataType::DataType(std::string name, size_t szBits, const std::string aliasFor, bool isPrimative, bool isPointer, bool isReference )
            : mName { name },
              mSzBits { szBits },
              mIsAlias { false },
              mIsPrimative { isPrimative },
              mAliasFor { aliasFor },
              mIsPointer { isPointer },
              mIsReference { isReference }
        {  }

        bool DataType::isPrimative() const
        {
            return mIsPrimative;
        }

        DataType DataType::makeAlias(std::string newName, const DataType& parentType)
        {
            return DataType(newName,  parentType.sizeBits(), parentType.name(), parentType.isPrimative());
        }

        size_t DataType::sizeBits() const
        {
            return mSzBits;
        }

        std::string DataType::name() const
        {
            return mName;
        }

        bool DataType::isAlias() const
        {
            return mIsAlias;
        }

        std::string DataType::getUnAliasedTypeName() const
        {
            if(! mIsAlias )
                return std::string();

            return mAliasFor.value();
        }

        std::optional<DataType> DataType::constructDefaultSupportedType(std::string name)
        {
            for(const DataType& dt : kpl::reflection::DEFAULT_DATATYPES)
                if(dt.name() == name)
                    return std::make_optional<DataType>( dt );

            return std::nullopt;
        }

        DataType DataType::makeVoidType()
        {
            return DataType("void", 0);
        }

        DataType DataType::makeUint8Type()
        {
            return DataType("int8_t", 1 * BITS_PER_BYTE);
        }

        DataType DataType::makeUint16Type()
        {
            return DataType("uint16_t", 2 * BITS_PER_BYTE);
        }

        DataType DataType::makeCharType()
        {
            return DataType("char", 1 * BITS_PER_BYTE);
        }

        bool DataType::isDefaultSupported(std::string name)
        {
            for(const DataType& dt : kpl::reflection::DEFAULT_DATATYPES)
                if(dt.name() == name)
                    return true;

            return false;
        }
    }
}
