#include "ast.h"

namespace bunny
{

    //Program
    Program::~Program()
    {
        for (auto stm: statements)
            delete stm;
    }


    std::string Program::Type()
    {
        return "Program";
    }

    std::string Program::TokenLiteral()
    {
        if (statements.size() > 0)
            return statements[0]->TokenLiteral();

        return "";
    }

    std::string Program::String()
    {
        std::string result;

        for(auto stm: statements)
            result += stm->String() + "\n";

        return result;
    }

    // BlockStatement

    BlockStatement::~BlockStatement()
    {
        for(auto stm: statements)
            delete stm;

    }

    std::string BlockStatement::Type()
    {
        return "BlockStatement";
    }

    std::string BlockStatement::TokenLiteral()
    {
        return token.literal;
    }

    std::string BlockStatement::String()
    {
        std::string result = "{";

        for(auto stm: statements)
            result += stm->String() + " ";

        result += "}";
        return result;
    }


    // Identifier

    std::string Identifier::Type()
    {
        return "Identifier";
    }

    std::string Identifier::TokenLiteral()
    {
        return token.literal;
    }

    std::string Identifier::String()
    {
        return value;
    }

    // IntegerLiteral

    std::string IntegerLiteral::Type()
    {
        return "IntegerLiteral";
    }

    std::string IntegerLiteral::TokenLiteral()
    {
        return token.literal;
    }

    std::string IntegerLiteral::String()
    {
        return std::to_string(value);
    }

    //BooleanLiteral

    std::string BooleanLiteral::Type()
    {
        return "BooleanLiteral";
    }

    std::string BooleanLiteral::TokenLiteral()
    {
        return token.literal;
    }

    std::string BooleanLiteral::String()
    {
        return value == true ? "true" : "false";
    }

    //StringLiteral

    std::string StringLiteral::Type()
    {
        return "StringLiteral";
    }

    std::string StringLiteral::TokenLiteral()
    {
        return token.literal;
    }

    std::string StringLiteral::String()
    {
        return value;
    }

    //FunctionLiteral

    FunctionLiteral::~FunctionLiteral()
    {
        for (auto par: param)
            delete par;
    }

    std::string FunctionLiteral::Type()
    {
        return "FunctionLiteral";
    }

    std::string FunctionLiteral::TokenLiteral()
    {
        return token.literal;
    }

    std::string FunctionLiteral::String()
    {
        std::string result = "(";
        for(auto par: param)
            result += par->String() + ", ";

        result += ")";

        result += functionBody->String();

        return result;
    }

    //ArrayLiteral

    ArrayLiteral::~ArrayLiteral()
    {
        for (auto elem: elements)
            delete elem;
    }

    std::string ArrayLiteral::Type()
    {
        return "ArrayLiteral";
    }

    std::string ArrayLiteral::TokenLiteral()
    {
        return token.literal;
    }

    std::string ArrayLiteral::String()
    {
        std::string result = "[";
        for(auto elem: elements)
            result += elem->String() + ", ";

        result += "]";

        return result;
    }

    //CallExpression

    CallExpression::~CallExpression()
    {
        for(auto arg: arguments)
            delete arg;

        delete functionName;
    }

    std::string CallExpression::Type()
    {
        return "CallExpression";
    }

    std::string CallExpression::TokenLiteral()
    {
        return token.literal;
    }

    std::string CallExpression::String()
    {
        std::string result = "(";

        for(auto arg: arguments)
            result += arg->String() + ", ";

        result += ")";

        return result;
    }

    //IndexExpression

    std::string IndexExpression::Type()
    {
        return "IndexExpression";
    }

    std::string IndexExpression::TokenLiteral()
    {
        return token.literal;
    }

    std::string IndexExpression::String()
    {
        return "(" + arrayVar->String() + "[" + index->String() + "])";
    }

    //PrefixExpression

    std::string PrefixExpression::Type()
    {
        return "PrefixExpression";
    }

    std::string PrefixExpression::TokenLiteral()
    {
        return token.literal;
    }

    std::string PrefixExpression::String()
    {
        return "(" + operation + right->String() + ")";
    }

    //InfixExpression

    std::string InfixExpression::Type()
    {
        return "PrefixExpression";
    }

    std::string InfixExpression::TokenLiteral()
    {
        return token.literal;
    }

    std::string InfixExpression::String()
    {
        return "(" + left->String() + " " + operation + " " + right->String() + ")";
    }

    //IfExpression

    std::string IfExpression::Type()
    {
        return "IfExpression";
    }

    std::string IfExpression::TokenLiteral()
    {
        return token.literal;
    }

    std::string IfExpression::String()
    {
        std::string result = "if (" + condition->String() + ") {" + ifBlock->String() + "}";

        if(elseBlock != nullptr) // vuol dire che è stato trovato un else ed allocato
            result += "else {" + elseBlock->String() + " }";

        return result;
    }

    //WhileExpression

    std::string WhileExpression::Type()
    {
        return "WhileExpression";
    }

    std::string WhileExpression::TokenLiteral()
    {
        return token.literal;
    }

    std::string WhileExpression::String()
    {
        std::string result = "while (" + condition->String()  + ")"
                                 + "{" + whileBlock->String() + "}";

        return result;
    }

    //VarStatement

    VarStatement::~VarStatement()
    {
        delete value;
    }

    std::string VarStatement::Type()
    {
        return "VarStatement";
    }

    std::string VarStatement::TokenLiteral()
    {
        return token.literal;
    }

    std::string VarStatement::String()
    {
        std::string result = "var " + name.String() + " = " + value->String();

        return result;
    }

    //ReferenceStatement

    ReferenceStatement::~ReferenceStatement()
    {
        delete value; // passaggio di valore
    }

    std::string ReferenceStatement::Type()
    {
        return "ReferenceStatement";
    }

    std::string ReferenceStatement::TokenLiteral()
    {
        return token.literal;
    }

    std::string ReferenceStatement::String()
    {
        std::string result = this->TokenLiteral() + " " + name.String() + " = ";

        if (value != nullptr)
            result += value->String();

        return result;
    }

    //ReturnStatement

    ReturnStatement::~ReturnStatement()
    {
        delete returnValue;
    }

    std::string ReturnStatement::Type()
    {
        return "ReturnStatement";
    }

    std::string ReturnStatement::TokenLiteral()
    {
        return token.literal;
    }

    std::string ReturnStatement::String()
    {
        std::string result = this->TokenLiteral();

        if (returnValue != nullptr) //la funzione ritorna qualcosa?
            result += returnValue->String();

        result += ";";

        return result;
    }

    //ExpressionStatement

    ExpressionStatement::~ExpressionStatement()
    {
        delete expression;
    }

    std::string ExpressionStatement::Type()
    {
        return "ExpressionStatement";
    }

    std::string ExpressionStatement::TokenLiteral()
    {
        return token.literal;
    }

    std::string ExpressionStatement::String()
    {
        std::string result;

        if (expression != nullptr)
            result += expression->String();

        result += ";";

        return result;
    }


}
