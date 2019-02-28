#include "kpl/membervariable.h"

#include <kpl/class.h>

namespace kpl {
    namespace reflection {

        MemberVariable::MemberVariable(const kpl::reflection::Class& classRef,
                                       const kpl::reflection::Variable& variable,
                                       kpl::reflection::AccessModifier accessModifier,
                                       bool isStatic)
            : mClass { classRef }, mVariable { variable }, mAccessModifier { accessModifier }, mIsStatic { isStatic }
        {
            assert( ! variable.name().empty() );
        }

        std::string MemberVariable::defaultInstanceName() const
        {
            std::string result = mVariable.name();

            result = Util::upperFirstChar(result);
            result.insert(result.begin(), 'm');

            return result;
        }

        std::string MemberVariable::asGetterMethodDeclaration(bool fullyQualify) const
        {
            const std::string qualifier = ( fullyQualify ) ? mClass.classNamespace() + "::" : "";

            return fmt::format("const {}& {}{}() const;", mVariable.typeName(), qualifier, mVariable.name());
        }

        std::string MemberVariable::asGetterMethodDefinition(std::string instanceName, bool fullyQualify) const
        {
            bool defaultNameUsed = false;

            if(instanceName.empty())
            {
                instanceName = defaultInstanceName();
                defaultNameUsed = true;
            }

            std::string getterMethodName = instanceName;

            // If using the mTypeName convention, remove the m and lowercase first char
            if(defaultNameUsed)
            {
                getterMethodName.erase(0, 1);
                getterMethodName = kpl::Util::lowerFirstChar(getterMethodName);
            }

            const std::string qualifier = ( fullyQualify ) ? mClass.classNamespace() + "::" : "";

            return fmt::format("{} {}{}(){ return {}; }", mVariable.typeName(), qualifier, getterMethodName, mVariable.name());
        }

        std::string MemberVariable::fullyQualifiedName() const
        {
            return mClass.classNamespace() + "::" + mVariable.name();
        }

        const kpl::reflection::Variable MemberVariable::varData() const
        {
            return mVariable;
        }

        const kpl::reflection::Class MemberVariable::classData() const
        {
            return mClass;
        }

        const kpl::reflection::AccessModifier& MemberVariable::accessModifier() const
        {
            return mAccessModifier;
        }

        const std::string& MemberVariable::accessModifierString() const
        {
            return kpl::reflection::AccessModifierString( mAccessModifier );
        }
    }
}
