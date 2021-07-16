#ifndef BUNNY_AST_H
#define BUNNY_AST_H

#include <vector>
#include "token.h"


namespace bunny
{

    // Interfaces

    class ASTNode
    {
        public:
            virtual std::string Type()         = 0;
            virtual std::string TokenLiteral() = 0;
            virtual std::string String()       = 0;
    };

    // Si possono vedere gli statement come righe di codice
    // con operazioni all'interno

    class Statement: public ASTNode
    {
        public:

            virtual void        statementNode()     = 0;

            virtual std::string Type()         = 0;
            virtual std::string TokenLiteral() = 0;
            virtual std::string String()       = 0;
    };

    // Expression intesa come espressione di valore matematico
    // quindi la computazione di un risultato

    class Expression: public ASTNode
    {
        public:
            virtual std::string Type()         = 0;
            virtual std::string TokenLiteral() = 0;
            virtual std::string String()       = 0;
    };


    // End Interfaces


    class Program: public ASTNode
    {
        public:

            //Variables
            std::vector<Statement*> statements;

            // Functions
            std::string Type();
            std::string TokenLiteral();
            std::string String();

            ~Program();
    };


    // Per BlockStatement si intende una sequenza di Statements
    // dentro delle parentesi grafe
    class BlockStatement: public Statement
    {
        public:

            std::vector<Statement*> statements;
            Token token; // --> "{"

            void statementNode() {};

            std::string Type();
            std::string TokenLiteral();
            std::string String();

            ~BlockStatement();

    };

    class Identifier: public Expression
    {
        public:

            //Variables
            std::string value;
            Token token;

            //Functions
            std::string Type();
            std::string TokenLiteral();
            std::string String();


    };

    class IntegerLiteral: public Expression
    {
        public:

            //Variables
            int value;
            Token token;

            //Functions
            std::string Type();
            std::string TokenLiteral();
            std::string String();


    };

    class BooleanLiteral: public Expression
    {
        public:

            //Variables
            bool value;
            Token token;

            //Functions
            std::string Type();
            std::string TokenLiteral();
            std::string String();


    };

    class StringLiteral: public Expression
    {
        public:

            //Variables
            std::string value;
            Token token;

            //Functions
            std::string Type();
            std::string TokenLiteral();
            std::string String();


    };

    class FunctionLiteral: public Expression
    {
        public:

            //Variables
            Token token;
            std::vector<Identifier*> param;
            BlockStatement* functionBody;

            //Functions
            std::string Type();
            std::string TokenLiteral();
            std::string String();


            ~FunctionLiteral();

    };

    class ArrayLiteral: public Expression
    {
        public:

            //Variables
            Token token;
            std::vector<Expression*> elements;

            //Functions
            std::string Type();
            std::string TokenLiteral();
            std::string String();


            ~ArrayLiteral();

    };

    class CallExpression: public Expression
    {
        public:

            //Variables
            Token token;
            Expression *functionName;
            std::vector<Expression*> arguments;

            //Functions
            std::string Type();
            std::string TokenLiteral();
            std::string String();


            ~CallExpression();
    };

    class IndexExpression: public Expression
    {
        public:

            //Variables
            Token token;
            Expression *arrayVar;
            Expression *index;

            //Functions
            std::string Type();
            std::string TokenLiteral();
            std::string String();

    };


    //Vedi RPL parsing (es. + 1 2 )
    class PrefixExpression: public Expression
    {
        public:

            //Variables
            Token token;
            std::string operation;
            Expression* right;

            //Functions
            std::string Type();
            std::string TokenLiteral();
            std::string String();

    };

    // es. 1 + 2
    class InfixExpression: public Expression
    {
        public:

            //Variables
            Token token;
            Expression* left;
            std::string operation;
            Expression* right;

            //Functions
            std::string Type();
            std::string TokenLiteral();
            std::string String();

    };

    // if ( (condition) ) { (blockStatement) } else {  (blockStatement) }
    class IfExpression: public Expression
    {
        public:

            //Variables
            Token token; // if
            Expression *condition;
            BlockStatement *ifBlock;
            BlockStatement *elseBlock;

            //Functions
            std::string Type();
            std::string TokenLiteral();
            std::string String();

    };

    // while ( (condition) ) { (blockStatement) }
    class WhileExpression: public Expression
    {
        public:

            //Variables
            Token token; // while
            Expression *condition;
            BlockStatement *whileBlock;

            //Functions
            std::string Type();
            std::string TokenLiteral();
            std::string String();

    };


    // es. var x = 5;
    class VarStatement: public Statement
    {
        public:

            //Variables
            Token token; // var
            Identifier  name;
            Expression *value;

            //Functions

            void statementNode() {} ;
            std::string Type();
            std::string TokenLiteral();
            std::string String();


            ~VarStatement();
    };

    // &x = 5;
    class ReferenceStatement: public Statement
    {
        public:

            //Variables
            Token token; // &
            Identifier  name;
            Expression *value;

            //Functions

            void statementNode() {} ;
            std::string Type();
            std::string TokenLiteral();
            std::string String();


            ~ReferenceStatement();
    };

    class ReturnStatement: public Statement
    {
        public:

            //Variables
            Token token; // return
            Expression *returnValue;

            //Functions

            void statementNode() {} ;
            std::string Type();
            std::string TokenLiteral();
            std::string String();


            ~ReturnStatement();
    };


    // es. x = (5 + ...) (quindi assegnazione di valore e non dichiarazione)
    class ExpressionStatement: public Statement
    {
        public:

            //Variables
            Token token; // il primo token della espressione
            Expression *expression;

            //Functions

            void statementNode() {} ;
            std::string Type();
            std::string TokenLiteral();
            std::string String();


            ~ExpressionStatement();
    };


}

#endif // BUNNY_AST_H
