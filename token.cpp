#include "token.h"


namespace bunny
{
    std::unordered_map<std::string, tokenType> keywords({
            {"var", VAR},
            {"func",FUNCTION},
            {"true",TRUE},
            {"false",FALSE},
            {"if", IF},
            {"else", ELSE},
            {"return", RETURN},
            {"while", WHILE}
    });

    tokenType checkIdent(std::string ident)
    {
        return (keywords.find(ident) == keywords.end() ? IDENTIFIER : keywords[ident]);
    }

    void Token::printToken()
    {
        std::cout << "Literal: " << this->literal << " Type: " << this->type << std::endl;
        return;
    }

}
