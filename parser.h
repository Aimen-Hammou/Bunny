#ifndef PARSER_BUNNY_H
#define PARSER_BUNNY_H

#include <vector>
#include <string>
#include <unordered_map>

#include "lexer.h"
#include "ast.h"
#include "precedences.h"

namespace bunny
{
    class Parser
    {
        public:

            Parser(lexer &_lex);

            std::vector<std::string> getErrors() {return this->errors; };

            Program *parseProgram();

        private:

            //Functions

            typedef Expression* (Parser::*PrefixParse)();
            typedef Expression* (Parser::*InfixParse)(Expression *);


            void nextToken();
            bool expectedPeek(const tokenType &_type);
            precedenza currentPrec();
            precedenza peekPrec();

            //Statements
            Statement           *parseStatement();
            VarStatement        *parseVarStatement();
            ReferenceStatement  *parseRefStatement();
            ReturnStatement     *parseReturnStatement();
            ExpressionStatement *parseExpressionStatement();
            BlockStatement      *parseBlockStatement();


            //Expression
            Expression *parseExpression(precedenza _prec);
            Expression *parseIdentifier();
            Expression *parseIntegerLiteral();
            Expression *parseStringLiteral();
            Expression *parsePrefixExpression();
            Expression *parseBoolean();
            std::vector<Identifier*> parseFunctionArguments();
            Expression *parseFunctionLiteral();
            Expression *parseArrayLiteral();
            Expression *parseGroupedExpressions();
            Expression *parseIfExpression();
            Expression *parseWhileExpression();

            std::vector<Expression*> parseExpressionList();
            Expression *parseCallExpression(Expression *func);
            Expression *parseIndexExpression(Expression *arr);
            Expression *parseInfixExpression(Expression *left);

            void missingPrefixFunctionError(tokenType type);

            void peekError(const tokenType &_type);


            //Variables

            lexer lex;

            Token currentToken;
            Token peekedToken;

            //We associate the different tokenTypes their type of parsing
            std::unordered_map<tokenType, PrefixParse> prefixParseFuncs;
            std::unordered_map<tokenType, InfixParse> InfixParseFuncs;

            std::vector<std::string> errors;


    };
}

#endif // PARSER_BUNNY_H
