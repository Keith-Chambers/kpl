#ifndef DATATYPE_H
#define DATATYPE_H

#include <string>
#include <optional>
#include <vector>

/* DataType - Represents a DataType that can be used in code, defined by it's name and size
 * Supports alias and non-primative types
 */

namespace kpl {
    namespace reflection
    {
        // extern const char * VOID_TYPE_NAME;

        class DataType
        {
        public:
            DataType(std::string name, size_t szBits, bool isPrimative = true, bool isPointer = false, bool isReference = false);
            DataType(std::string name, size_t szBits, const std::string aliasFor, bool isPrimative = true, bool isPointer = false, bool isReference = false);

            size_t sizeBits() const;
            std::string name() const;
            bool isAlias() const;
            bool isPrimative() const;
            std::string getUnAliasedTypeName() const;

            /* Static Methods */
            static DataType makeAlias(std::string newName, const DataType& parentType);
            static const std::vector<DataType>* generateDefaultTypes();
            static bool isDefaultSupported(std::string name);
            static size_t szBitsOfSupportedType(std::string name);

        private:
            const std::string mName;
            const std::size_t mSzBits;
            const bool mIsAlias;
            const bool mIsPrimative;
            std::optional<const std::string> mAliasFor;
            const bool mIsPointer;
            const bool mIsReference;


            /* NOTE: Needs to be synced with generateDefaultTypes() */
            static const uint8_t NUM_DEFAULT_SUPPORTED_TYPES;
            static std::vector<DataType> mDefaultSupportedTypes;
        };
    }
}

#endif // DATATYPE_H
