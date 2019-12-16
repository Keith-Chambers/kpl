#include "kpl/reflection/accessmodifier.h"

namespace kpl {
    namespace reflection {

        const std::string& AccessModifierString(AccessModifier accessModifier)
        {
            static const std::string publicString = "public";
            static const std::string privateString = "private";
            static const std::string protectedString = "protected";

            switch(accessModifier)
            {
            case AccessModifier::PUBLIC:
                return publicString;
            case AccessModifier::PRIVATE:
                return privateString;
            case AccessModifier::PROTECTED:
                return protectedString;
            }

            std::cout << "Err: Reached end of kpl::reflection::Class::AccessModifierString switch" << std::endl;
            exit(EXIT_FAILURE);
        }
    }
}


