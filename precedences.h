#ifndef PREC_BUNNY_H
#define PREC_BUNNY_H

#include <unordered_map>

#include "lexer.h"
#include "ast.h"

namespace bunny
{

    enum precedenza
    {
        LOWEST,
        EQUALS,      // ==
        LESSGREATER, // < oppure >
        SUM,         // + o -
        PRODUCT,     // *
        PREFIX,      // -X oppure !X
        CALL,        // chiamata di funzione
        INDEX        // array[]
    };

    // serve per associare i vari token al loro livello di precedenza
    // da utilizzare successivamente nel parser
    extern std::unordered_map<tokenType, precedenza> precedences;

}

#endif // PREC_BUNNY_H
