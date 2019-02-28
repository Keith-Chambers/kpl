#include "kpl/class.h"

namespace kpl {
    namespace reflection {

        Class::Class( const std::string& name, const std::string& classNamespace )
            : mName { name }, mNamespace { classNamespace }
        {  }

        bool Class::addMethod( kpl::reflection::Function function, kpl::reflection::AccessModifier accessModifier, bool isStatic, bool isConst )
        {
            if( containsMethod( function.name() ) )
                return false;

            mMethods.emplace_back( *this, function, accessModifier, isStatic, isConst );
            mMethodDecorations.push_back( NO_DECORATIONS );

            assert( mMethods.size() == mMethodDecorations.size() && "Error: mMethods and mMethodDecorations are of different sizes" );

            return true;
        }

        bool Class::addMethod( kpl::reflection::Function function, uint8_t decorations, kpl::reflection::AccessModifier accessModifier, bool isStatic, bool isConst )
        {
            if( containsMethod( function.name() ) )
                return false;

            mMethods.emplace_back( *this, function, accessModifier, isStatic, isConst );
            mMethodDecorations.push_back(decorations);

            assert( mMethods.size() == mMethodDecorations.size() && "Error: mMethods and mMethodDecorations are of different sizes" );

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
            mVariableDecorations.push_back( NO_DECORATIONS );

            assert( mVariables.size() == mVariableDecorations.size() && "Error: mVariables and mVariableDecorations are of different sizes" );

            return true;
        }

        bool Class::addVariable( kpl::reflection::Variable variable, uint8_t decorations, kpl::reflection::AccessModifier accessModifier, bool isStatic )
        {
            if( containsVariable( variable.name() ))
                return false;

            mVariables.emplace_back( *this, variable, accessModifier, isStatic );
            mVariableDecorations.push_back( decorations );

            assert( mVariables.size() == mVariableDecorations.size() && "Error: mVariables and mVariableDecorations are of different sizes" );

            return true;
        }

        bool Class::containsVariable( const std::string& name ) const
        {
            for( const kpl::reflection::MemberVariable& memVar : mVariables )
                if( memVar.varData().name() == name )
                    return true;

            return false;
        }

        bool Class::containsVariable( const std::string& name, const uint8_t decorationMask ) const
        {
            for( std::size_t i = 0; i < mVariables.size(); i++ )
                if( mVariables[i].varData().name() == name )
                    return ( (mVariableDecorations[i] & decorationMask) == decorationMask );

            return false;
        }

        bool Class::containsMethod( const std::string& name ) const
        {
            for( const kpl::reflection::MemberFunction& memFunc : mMethods )
                if( memFunc.funcData().name() == name )
                    return true;

            return false;
        }

        bool Class::containsMethod( const std::string& name, const uint8_t decorationMask ) const
        {
            for( std::size_t i = 0; i < mMethods.size(); i++ )
                if( mMethods[i].funcData().name() == name )
                    return ( (mMethodDecorations[i] & decorationMask) == decorationMask );

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

        const std::vector<kpl::reflection::MemberFunction>& Class::getMethods() const
        {
            return mMethods;
        }

        std::vector<kpl::reflection::MemberFunction> Class::getMethods( const uint8_t decorationMask ) const
        {
            std::vector<kpl::reflection::MemberFunction> result;

            for(std::size_t i = 0; i < mMethods.size(); i++)
                if( (mMethodDecorations[i] & decorationMask) == decorationMask )
                    result.push_back( mMethods[i] );

            return result;
        }

        const std::vector<kpl::reflection::MemberVariable>& Class::getVariables() const
        {
            return mVariables;
        }

        std::vector<kpl::reflection::MemberVariable> Class::getVariables( const uint8_t decorationMask ) const
        {
            std::vector<kpl::reflection::MemberVariable> result;

            for(std::size_t i = 0; i < mVariables.size(); i++)
                if( (mVariableDecorations[i] & decorationMask) == decorationMask )
                    result.push_back( mVariables[i] );

            return result;
        }
    }
}
