#ifndef BUNNY_LEXER_H
#define BUNNY_LEXER_H

#include "token.h"


namespace bunny
{

    /*
        Lexer: serve a leggere i caratteri del codice sorgente ed interpretare il contenuto di esso
        convertendo il tutto in token [dove possibile].
    */

    class lexer
    {
        private:

            std::string input;
            int position;        // posizione corrente del currentChar
            int readingPosition; // dove ci troviamo nella lettura dell'input
            char currentChar;

        public:

            lexer() = default;
            lexer(std::string& _input);

            void nextChar();
            char peekNextChar();

            void skipWhitespace(); // salta spazi bianchi, \n e commenti

            std::string getIdentifier();
            std::string getNumber();
            std::string getString();

            Token getNextToken();

    };

}

#endif // BUNNY_LEXER_H
