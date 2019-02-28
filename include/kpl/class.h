#ifndef CLASS_H
#define CLASS_H

#include <vector>
#include <string>
#include <optional>

#include <kpl/function.h>
#include <kpl/variable.h>

#include <kpl/memberfunction.h>
#include <kpl/membervariable.h>
#include <kpl/accessmodifier.h>

namespace kpl {
    namespace reflection {

        class Class
        {
        public:

            Class(const std::string& name, const std::string& classNamespace);

            static constexpr uint8_t NO_DECORATIONS = 0b00000000;

            static const std::string& AccessModifierString(AccessModifier accessModifier);

            bool addMethod( kpl::reflection::Function function,
                            kpl::reflection::AccessModifier accessModifier = kpl::reflection::AccessModifier::PUBLIC,
                            bool isStatic = false,
                            bool isConst = false );

            bool addMethod( kpl::reflection::Function function,
                            uint8_t decorations,
                            kpl::reflection::AccessModifier accessModifier = kpl::reflection::AccessModifier::PUBLIC,
                            bool isStatic = false,
                            bool isConst = false );

            bool addVariable( kpl::reflection::Variable variable,
                              kpl::reflection::AccessModifier accessModifier = kpl::reflection::AccessModifier::PUBLIC,
                              bool isStatic = false );

            bool addVariable( kpl::reflection::Variable variable,
                              uint8_t decorations,
                              kpl::reflection::AccessModifier accessModifier = kpl::reflection::AccessModifier::PUBLIC,
                              bool isStatic = false );

            bool containsVariable( const std::string& name ) const;
            bool containsMethod( const std::string& name ) const;

            std::optional<kpl::reflection::MemberFunction> getMethod( const std::string& name ) const;
            std::optional<kpl::reflection::MemberVariable> getVariable( const std::string& name ) const;

            std::vector<kpl::reflection::MemberFunction> getMethods(uint8_t decorationMask);
            const std::vector<kpl::reflection::MemberFunction>& getMethods();

            std::vector<kpl::reflection::MemberVariable> getVariables(uint8_t decorationMask);
            const std::vector<kpl::reflection::MemberVariable>& getVariables();

            const std::string& classNamespace() const;
            const std::string& name() const;

        private:
            std::string mName;
            std::string mNamespace;

            std::vector<kpl::reflection::MemberFunction> mMethods;
            std::vector<uint8_t> mMethodDecorations;

            std::vector<kpl::reflection::MemberVariable> mVariables;
            std::vector<uint8_t> mVariableDecorations;
        };
    }
}



#endif // CLASS_H
