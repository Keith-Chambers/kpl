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

            return fmt::format("{0} {1} {2}{3}::{4}({5}) {6};",
                                staticQualifier,                    /* 0 */
                                mFunction.returnValueString(),      /* 1 */
                                classQualifier,                     /* 2 */
                                mClass.name(),                      /* 3 */
                                mFunction.name(),                   /* 4 */
                                mFunction.parameterListString(),    /* 5 */
                                constQualifier );                   /* 6 */
        }

        std::string MemberFunction::asDefinitionCode(const std::string& body, bool fullyQualify) const
        {
            static const std::string constQualifier = ( mIsConst ) ? "const" : "";
            const std::string classQualifier = (fullyQualify) ? mClass.classNamespace() + "::" : "";

            return fmt::format("{0} {1}{2}::{3}({4}) {5} \n{{\n {6} \n}}",
                                mFunction.returnValueString(),      /* 0 */
                                classQualifier,                     /* 1 */
                                mClass.name(),                      /* 2 */
                                mFunction.name(),                   /* 3 */
                                mFunction.parameterListString(),    /* 4 */
                                constQualifier,                     /* 5 */
                                body );                             /* 6 */
        }

        std::string MemberFunction::asInvocationCode(const std::string instanceName, const std::string passedParameterString, bool isPointer) const
        {
            const std::string accessNotation = ( isPointer ) ? "->" : ".";

            return fmt::format("{0}{1}{2}({3})",
                                instanceName,               /* 0 */
                                accessNotation,             /* 1 */
                                mFunction.name(),           /* 2 */
                                passedParameterString );    /* 3 */
        }

        const kpl::reflection::DataType& MemberFunction::returnDataType() const
        {
            return mFunction.returnDataType();
        }

        std::string MemberFunction::qualifiedName() const
        {
            return mClass.classNamespace() + "::" + mClass.name() + "." + mFunction.name();
        }

        bool MemberFunction::operator==(const MemberFunction& memFunc) const
        {
            return ( memFunc.mClass == mClass && mFunction == memFunc.funcData() );
        }

        bool MemberFunction::operator!=(const MemberFunction& memFunc) const
        {
            return ( memFunc.mClass != mClass || mFunction != memFunc.funcData() );
        }

        bool MemberFunction::operator<(const MemberFunction& memFunc) const
        {
            if(mClass < memFunc.classData())
                return true;

            if(mClass > memFunc.classData())
                return false;

            if(mFunction < memFunc.funcData())
                return true;

            return false;
        }

        bool MemberFunction::operator>(const MemberFunction& memFunc) const
        {
            if(mClass > memFunc.classData())
                return true;

            if(mClass < memFunc.classData())
                return false;

            if(mFunction > memFunc.funcData())
                return true;

            return false;
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
