#ifndef BUNNY_TOKEN_H
#define BUNNY_TOKEN_H

#include <iostream>
#include <string>
#include <unordered_map>


typedef std::string tokenType; //per rendere il codice più facile da interpretare

namespace bunny
{


    class Token
    {
        public:

            Token() = default;

            Token(tokenType _type, std::string _literal): type(_type), literal(_literal){}
            Token(tokenType _type, char currentChar)    : type(_type), literal(std::string(1, currentChar)){}


            // es. TOKEN(INT, "322")
            tokenType type;
            std::string literal;

            void printToken();


    };

    /*
        Ho preferito gestire i token usando le std::string e non una ENUM.
        Questo perchè è più facile così la stampa e successiva gestione dei TOKEN.

        Sono CONST poichè non vogliamo che, in nessun modo, i valori vengano cambiati da qualche funzione
        essendo che darebbe poi problemi nella creazione dell'AST durante il parsing.
    */


    // Error TOKEN

    const tokenType ILLEGAL = "ILLEGAL"; // un token che non conosciamo
    const tokenType END     = "END";     // fine file


    // Variable TOKEN

    const tokenType INT        = "INT";
    const tokenType STRING     = "STRING";
    const tokenType IDENTIFIER = "IDENTIFIER";

    // Operations TOKEN

    const tokenType PLUS     = "+";
    const tokenType MINUS    = "-";
    const tokenType SLASH    = "/";
    const tokenType ASTERISK = "*";
    const tokenType PERCENT  = "%";
    const tokenType BANG     = "!";
    const tokenType ASSIGN   = "=";

    // Confront TOKEN

    const tokenType LT = "<";  //Lower Thna
    const tokenType GT = ">";  //Greater Than
    const tokenType LE = "<="; //Lower Equal
    const tokenType GE = ">="; //Greater Equal
    const tokenType EQ = "=="; //Equal
    const tokenType NE = "!="; //Not Equal

    //General TOKEN

    const tokenType SEMI  = ";";
    const tokenType COMMA = ",";

    //Keywords TOKEN [TO ADD FOR LOOP]

    const tokenType WHILE    = "WHILE";

    const tokenType IF       = "IF";
    const tokenType ELSE     = "ELSE";

    const tokenType TRUE     = "TRUE";
    const tokenType FALSE    = "FALSE";

    const tokenType RETURN   = "RETURN";

    const tokenType VAR      = "VAR";

    const tokenType FUNCTION = "FUNCTION";

    //General TOKEN

    const tokenType LPAREN   = "(";
    const tokenType RPAREN   = ")";

    const tokenType LBRACE   = "{";
    const tokenType RBRACE   = "}";

    const tokenType LBRACKET = "[";
    const tokenType RBRACKET = "]";

    const tokenType REF      = "&";


    extern std::unordered_map<std::string, tokenType> keywords;


    // Serve per verificare se un identificatore è una parola chiave oppure una variabile che viene dichiarata
    tokenType checkIdent(std::string ident);


}




#endif // BUNNY_TOKEN_H
