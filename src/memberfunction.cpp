#include "kpl/memberfunction.h"

#include <kpl/class.h>

namespace kpl {
    namespace reflection {

        MemberFunction::MemberFunction( const kpl::reflection::Class& classRef, const kpl::reflection::Function function,
                        kpl::reflection::AccessModifier accessModifier, bool isStatic, bool isConst )
            : mClass { classRef }, mFunction { function }, mAccessModifier { accessModifier }, mIsStatic { isStatic }, mIsConst { isConst }
        {
            assert( ! ( mIsConst && mIsStatic ) && "Method cannot be both const and static" );
        }

        std::string MemberFunction::asDeclarationCode(bool fullyQualify) const
        {
            static const std::string staticQualifier = ( mIsStatic ) ? "static" : "";
            static const std::string constQualifier = ( mIsConst ) ? "const" : "";
            const std::string classQualifier = (fullyQualify) ? mClass.classNamespace() + "::" : "";

            return fmt::format("{} {} {}{}({}) {};", staticQualifier, mFunction.returnValueString(), classQualifier, mFunction.name(), mFunction.parameterListString(), constQualifier);
        }

        std::string MemberFunction::asInvocationCode(const std::string instanceName, const std::string passedParameterString, bool isPointer) const
        {
            const std::string accessNotation = ( isPointer ) ? "->" : ".";

            return fmt::format("{}{}{}({})", instanceName, accessNotation, mFunction.name(), passedParameterString);
        }

        const kpl::reflection::Function& MemberFunction::funcData() const
        {
            return mFunction;
        }

        const kpl::reflection::Class& MemberFunction::classData() const
        {
            return mClass;
        }

        bool MemberFunction::isStatic() const
        {
            return mIsStatic;
        }

        bool MemberFunction::isConst() const
        {
            return mIsConst;
        }
    }
}
