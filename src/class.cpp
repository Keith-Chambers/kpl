#include "kpl/class.h"

namespace kpl {
    namespace reflection {

        Class::Class(const std::string& name, const std::string& classNamespace)
            : mName { name }, mNamespace { classNamespace }
        {  }

        bool Class::addMethod(kpl::reflection::Function function, kpl::reflection::AccessModifier accessModifier, bool isStatic, bool isConst)
        {
            if( containsMethod( function.name() ) )
                return false;

            mMethods.emplace_back( *this, function, accessModifier, isStatic, isConst );

            return true;
        }



        const std::string& Class::classNamespace() const
        {
            return mNamespace;
        }

        const std::string& Class::name() const
        {
            return mName;
        }

        bool Class::addVariable( kpl::reflection::Variable variable, kpl::reflection::AccessModifier accessModifier, bool isStatic )
        {
            if( containsVariable( variable.name() ))
                return false;

            mVariables.emplace_back( *this, variable, accessModifier, isStatic );

            return true;
        }

        bool Class::containsVariable( const std::string& name ) const
        {
            for( const kpl::reflection::MemberVariable& memVar : mVariables )
                if( memVar.varData().name() == name )
                    return true;

            return false;
        }

        bool Class::containsMethod( const std::string& name ) const
        {
            for( const kpl::reflection::MemberFunction& memFunc : mMethods )
                if( memFunc.funcData().name() == name )
                    return true;

            return false;
        }

        std::optional<kpl::reflection::MemberFunction> Class::getMethod( const std::string& name ) const
        {
            for( const kpl::reflection::MemberFunction& memFunc : mMethods )
                if( memFunc.funcData().name() == name )
                    return std::optional<kpl::reflection::MemberFunction>( memFunc );

            return std::nullopt;
        }

        std::optional<kpl::reflection::MemberVariable> Class::getVariable( const std::string& name ) const
        {
            for( const kpl::reflection::MemberVariable& memVar : mVariables )
                if( memVar.varData().name() == name )
                    return std::optional<kpl::reflection::MemberVariable>(memVar);

            return std::nullopt;
        }

        const std::vector<kpl::reflection::MemberFunction>& Class::getMethods()
        {
            return mMethods;
        }

        const std::vector<kpl::reflection::MemberVariable>& Class::getVariables()
        {
            return mVariables;
        }

    }
}