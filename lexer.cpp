#include <ctype.h>
#include "lexer.h"


namespace bunny
{
    lexer::lexer(std::string& _input)
    {
        input = _input;
        readingPosition = 0;
        position = 0;

        nextChar();
    }


    void lexer::nextChar()
    {
        currentChar = (readingPosition >= input.size() ? 0 : input[readingPosition]);

        position = readingPosition;
        readingPosition++;
    }


    char lexer::peekNextChar()
    {
        return (readingPosition >= input.size() ? 0 : input[readingPosition]);
    }


    //[TODO] IMPLEMENTA CHE IGNORI ANCHE I COMMENTI
    void lexer::skipWhitespace()
    {
        while(currentChar == ' ' || currentChar == '\n'|| currentChar == '\t'|| currentChar == '\r')

            this->nextChar();

        //qua implementi il codice dei commenti (pro-tip: usare funz ricorsivo può aiutare
    }

    // conviene di più substr() al posto di concatenare?
    std::string lexer::getIdentifier()
    {
        int start = position;

        while(isalpha(currentChar) || currentChar == '_')
            nextChar();

        return input.substr(start, position - start);

    }


    std::string lexer::getNumber()
    {
        int start = position;

        while(isdigit(currentChar))
            nextChar();

        return input.substr(start, position - start);
    }

    std::string lexer::getString()
    {
        int start = position + 1;

        do {
            nextChar();
        }while (currentChar != '"' && currentChar != 0);

        return input.substr(start, position - start);
    }


    Token lexer::getNextToken()
    {
        Token tok;

        skipWhitespace();

        switch(currentChar)
        {

        case '+':
            tok = Token(PLUS, "+");
            nextChar();
            break;

        case '-':
            tok = Token(MINUS, "-");
            nextChar();
            break;

        case '/':
            tok = Token(SLASH, "/");
            nextChar();
            break;

        // DA AGGIUNGERE TOKEN ** come elevata (vedi Python)
        case '*':
            tok = Token(ASTERISK, "*");
            nextChar();
            break;

        case '%':
            tok = Token(PERCENT, "%");
            nextChar();
            break;

        case '!':
            if(this->peekNextChar() == '=')
            {
                tok = Token(NE, "!=");
                nextChar();
            }
            else
            {
                tok = Token(BANG, "!");
            }

            nextChar();

            break;

        case '=':
            if(this->peekNextChar() == '=')
            {
                tok = Token(EQ, "==");
                nextChar();
            }
            else tok = Token(ASSIGN, "=");

            nextChar();

            break;

        // AGGIUNGERE << BIT OPERATION
        case '<':
            if(this->peekNextChar() == '=')
            {
                tok = Token(LE, "<=");
                nextChar();
            }
            else
                tok = Token(LT, "<");

            nextChar();
            break;

        case '>':
            if(this->peekNextChar() == '=')
            {
                tok = Token(GE, ">=");
                nextChar();
            }
            else tok = Token(GT, ">");

            nextChar();

            break;

        case ';':
            tok = Token(SEMI, ";");
            nextChar();
            break;

        case ',':
            tok = Token(COMMA,",");
            nextChar();
            break;

        case '(':
            tok = Token(LPAREN, "(");
            nextChar();
            break;

        case ')':
            tok = Token(RPAREN, ")");
            nextChar();
            break;

        case '{':
            tok = Token(LBRACE, "{");
            nextChar();
            break;

        case '}':
            tok = Token(RBRACE, "}");
            nextChar();
            break;

        case '[':
            tok = Token(LBRACKET, "[");
            nextChar();
            break;

        case ']':
            tok = Token(RBRACKET, "]");
            nextChar();
            break;

        case '&':
            tok = Token(REF, "&");
            nextChar();
            break;

        case '"':
            tok = Token(STRING, getString());
            nextChar();
            break;
        case 0:
            tok = Token(END, "");
            nextChar();
            break;

        default:
            if(isalpha(currentChar) || currentChar == '_')
            {
                tok.literal = getIdentifier();
                tok.type    = checkIdent(tok.literal);
            }
            else if(isdigit(currentChar))
            {
                tok = Token(INT, getNumber());
            }
            else
            {
                tok = Token(ILLEGAL, currentChar);
                nextChar();
            }
        }


        return tok;

    }


}


