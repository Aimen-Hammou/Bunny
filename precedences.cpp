#include "precedences.h"

namespace bunny
{
    std::unordered_map<tokenType, precedenza> precedences
    (
     {
         {EQ, EQUALS},
         {NE, EQUALS},

         {LT, LESSGREATER},
         {GT, LESSGREATER},
         {LE, LESSGREATER},
         {GE, LESSGREATER},

         {PLUS, SUM},
         {MINUS,SUM},

         {ASTERISK, PRODUCT},
         {SLASH   , PRODUCT},
         {PERCENT, PRODUCT},

         {LPAREN,  CALL},
         {LBRACKET,INDEX},
     }
    );
}
