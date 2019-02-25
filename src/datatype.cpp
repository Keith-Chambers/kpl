#include "kpl/datatype.h"

namespace kpl {
    namespace reflection
    {
        // const char * VOID_TYPE_NAME = "void";
        std::vector<DataType> DataType::mDefaultSupportedTypes;
        const uint8_t DataType::NUM_DEFAULT_SUPPORTED_TYPES = 9;

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

        const std::vector<DataType>* DataType::generateDefaultTypes()
        {
            constexpr uint8_t BITS_PER_BYTE = 8;

            if(! mDefaultSupportedTypes.empty())
                return &mDefaultSupportedTypes;

            mDefaultSupportedTypes.reserve(NUM_DEFAULT_SUPPORTED_TYPES);

            /* NOTE: Update NUM_DEFAULT_SUPPORTED_TYPES if you change below */
            // mDefaultSupportedTypes.push_back(DataType(kpl::reflection::VOID_TYPE_NAME, 0));
            mDefaultSupportedTypes.push_back(DataType("void", 0));
            mDefaultSupportedTypes.push_back(DataType("char", 1 * BITS_PER_BYTE));
            mDefaultSupportedTypes.push_back(DataType("bool", 1));
            mDefaultSupportedTypes.push_back(DataType("uint8_t", 1 * BITS_PER_BYTE));
            mDefaultSupportedTypes.push_back(DataType("int8_t", 1 * BITS_PER_BYTE));
            mDefaultSupportedTypes.push_back(DataType("uint16_t", 2 * BITS_PER_BYTE));
            mDefaultSupportedTypes.push_back(DataType("int16_t", 2 * BITS_PER_BYTE));
            mDefaultSupportedTypes.push_back(DataType("uint32_t", 4 * BITS_PER_BYTE));
            mDefaultSupportedTypes.push_back(DataType("int32_t", 4 * BITS_PER_BYTE));

            return &mDefaultSupportedTypes;
        }

        bool DataType::isDefaultSupported(std::string name)
        {
            const std::vector<DataType>* defaultTypes { generateDefaultTypes() };

            for(const DataType& dt : *defaultTypes)
                if(dt.name() == name)
                    return true;

            return false;
        }

        size_t DataType::szBitsOfSupportedType(std::string name)
        {
            const std::vector<DataType>* defaultTypes { generateDefaultTypes() };

            for(const DataType& dt : *defaultTypes)
                if(dt.name() == name)
                    return dt.sizeBits();

            return 0;
        }
    }
}
