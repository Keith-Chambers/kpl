#include "kpl/reflection/class.h"

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

        std::string Class::qualifiedName() const
        {
            return mNamespace + "::" + mName;
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

        kpl::reflection::MemberFunction * Class::getMethodPtr( const std::string& name )
        {
            for( kpl::reflection::MemberFunction& memFunc : mMethods )
                if( memFunc.funcData().name() == name )
                    return &memFunc;

            return nullptr;
        }

        const kpl::reflection::MemberFunction * Class::getMethodPtr( const std::string& name ) const
        {
            for( const kpl::reflection::MemberFunction& memFunc : mMethods )
                if( memFunc.funcData().name() == name )
                    return &memFunc;

            return nullptr;
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

        uint8_t Class::methodDecoration( const std::string& methodName ) const
        {
            for(std::size_t i = 0; i < mMethods.size(); i++)
                if( mMethods[i].funcData().name() == methodName )
                    return mMethodDecorations[i];

            std::cout << "Error: Tried to get method declaration for unknown method '" + methodName + "'" << std::endl;
            exit( EXIT_FAILURE );

            return NO_DECORATIONS;
        }

        uint8_t Class::variableDecoration( const std::string& variableName ) const
        {
            for(std::size_t i = 0; i < mVariables.size(); i++)
                if( mVariables[i].varData().name() == variableName )
                    return mVariableDecorations[i];

            std::cout << "Error: Tried to get method declaration for unknown method '" + variableName + "'" << std::endl;
            exit( EXIT_FAILURE );

            return NO_DECORATIONS;
        }

        bool Class::operator==(const Class& classData) const
        {
            return classData.name() == mName && classData.classNamespace() == classNamespace();
        }

        bool Class::operator!=(const Class& classData) const
        {
            return classData.name() != mName || classData.classNamespace() != classNamespace();
        }

        bool Class::operator<(const Class& classData) const
        {
            if(mName < classData.name())
                return true;

            if(mName > classData.name())
                return false;

            if(mNamespace < classData.classNamespace())
                return true;

            return false;
        }

        bool Class::operator>(const Class& classData) const
        {
            if(mName > classData.name())
                return true;

            if(mName < classData.name())
                return false;

            if(mNamespace > classData.classNamespace())
                return true;

            return false;
        }

        std::string Class::asHeaderFileGuard() const
        {
            std::string result = mName;
            std::for_each(result.begin(), result.end(), ::toupper);

            return "INC_" + result + "_H";
        }

        std::string Class::includeDirectiveCode( const std::string& pathPrefix ) const
        {
            std::string nameLower = mName;
            std::transform(nameLower.begin(), nameLower.end(), nameLower.begin(), ::tolower);

            return "#include <" + pathPrefix + asHeaderFileName() + ">";
        }

        std::string Class::singletonInstanciationCode() const
        {
            return mNamespace + "::" + mName + "::getInstance()";
        }

        std::string Class::singletonAssignmentCode(std::string instanceName) const
        {
            if(instanceName.empty())
                instanceName = asSingletonName();

            return mNamespace + "::" + mName + "* " + instanceName + " = " + mNamespace + "::" + mName + "::getInstance()";
        }

        std::string Class::objectInstanciationCode(const std::string& parameters) const
        {
            std::string result = mNamespace + "::" + mName + " " + asSingletonName();

            if(! parameters.empty() )
                result += "( " + parameters + ")";

            return result;
        }

        std::string Class::objectInstanciationCode(const std::string& objName, const std::string& parameters) const
        {
            std::string result = mNamespace + "::" + mName + " " + objName;

            if(! parameters.empty() )
                result += "( " + parameters + ")";

            return result;
        }

        std::string Class::asSourceFileName() const
        {
            std::string nameLower = mName;
            std::transform(nameLower.begin(), nameLower.end(), nameLower.begin(), ::tolower);

            return nameLower + ".cpp";
        }

        std::string Class::asHeaderFileName() const
        {
            std::string nameLower = mName;
            std::transform(nameLower.begin(), nameLower.end(), nameLower.begin(), ::tolower);

            return nameLower + ".h";
        }

        std::string Class::asSingletonName() const
        {
            std::string singletonName = mName;
            std::transform(mName.begin(), mName.begin() + 1, singletonName.begin(), ::tolower);

            return singletonName;
        }
    }
}
