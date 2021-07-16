#include <iostream>

#include "evaluator.h"



namespace bunny
{

    /*
        Funzioni builtin
        Nota: Qua dovrebbe essere inserite le funzioni in seguito
        al porting di librerie esterne

        Il parsing avviene nella CALL
    */

    Object *print(std::vector<Object *>& objs)
    {
        for(auto obj: objs)
        {
            if(obj->getType() == ERROR_OBJ)
                return obj;

            std::cout << obj->getValue() << " ";
        }

        std::cout << std::endl;

        return __NULL__;
    }

    std::unordered_map<std::string, Builtin *> builtin
    (
     {
         {"print", new Builtin(*print)}
     }
    );

    bool isTrue(Object *condition)
    {
        if(condition == __TRUE__)
            return true;
        else if (condition == __FALSE__)
            return false;
        else if (condition == __NULL__)
            return false;
        else if (condition->getType() == INTEGER_OBJ &&
                 static_cast<Integer *>(condition)->value == 0)
            return false;
        else
            return true;
    }

    bool isError(Object *toCheck)
    {
        return (toCheck != nullptr) ? toCheck->getType() == ERROR_OBJ : false;
    }

    Object *Evaluator::evalStatements(std::vector<Statement *> _statements, Environment *_env)
    {
        Object *result;

        for(auto stmt: _statements)
        {
            result = Eval(stmt, _env);

            garbageCollector.GCMark(result);

            if(result->getType() == RETURN_OBJ)
            {
                garbageCollector.GCMark(static_cast<Return *>(result)->value);
            }

            gcCapacity += 1;

            if(gcCapacity == 100) // capacità massima del GC
            {
                garbageCollector.GCMark(_env);
                garbageCollector.GCSweep();

                gcCapacity = 0;
            }

            if(result->getType() == RETURN_OBJ || result->getType() == ERROR_OBJ)
                return result;
        }


        return result;
    }

    Object *Evaluator::evalBang(Object *_right)
    {
        return isTrue(_right) ? __FALSE__ : __TRUE__;
    }


    // Applicare anche per gli array?
    Object *Evaluator::evalMinusPrefix(Object *_right)
    {
        if(_right->getType() == INTEGER_OBJ)
        {
            Integer *tmp = new Integer(- static_cast<Integer *>(_right)->value);
            return tmp;
        }

        return new Error("ERROR: Unknown operation: -" + _right->getType());
    }

    Object *Evaluator::evalPrefixExpression(std::string _op ,Object *_right)
    {

        if(_op == "!") return evalBang(_right);
        else if (_op == "-") return evalMinusPrefix(_right);
        else return __NULL__;
    }

    Object *Evaluator::evalIntegerInfix(std::string _op, Object *_left, Object *_right)
    {

        int leftValue = static_cast<Integer *>(_left)->value;
        int rightValue = static_cast<Integer *>(_right)->value;

        Object *result;


        if(_op == "+") {

            result = new Integer(leftValue + rightValue);

        }else if(_op == "-"){

            result = new Integer(leftValue - rightValue);

        }else if(_op == "*"){

            result = new Integer(leftValue * rightValue);

        }else if(_op == "/"){
            if(rightValue != 0)
                result = new Integer(leftValue / rightValue);
            else
                result = new Error("ERROR: Cannot perform division because right value is 0");

        }else if(_op == "%"){

            if(rightValue != 0)
                result = new Integer(leftValue % rightValue);
            else
                result = new Error("ERROR: Cannot perform % because right value is 0");

        }else if(_op == "=="){

            return leftValue == rightValue ? __TRUE__ : __FALSE__;

        }else if(_op == "!="){

            return leftValue != rightValue ? __TRUE__ : __FALSE__;

        }else if(_op == "<"){

            return leftValue < rightValue ? __TRUE__ : __FALSE__;
        }else if(_op == ">"){

            return leftValue > rightValue ? __TRUE__ : __FALSE__;

        }else if(_op == "<="){

            return leftValue <= rightValue ? __TRUE__ : __FALSE__;

        }else if(_op == ">="){

            return leftValue >= rightValue ? __TRUE__ : __FALSE__;
        }else{

            result = new Error("Unkown operation: " + _left->getType() + " " + _op + " " + _right->getType());
        }

        garbageCollector.GCAdd(result);

        return result;
    }


    //Ho preferito dividere le funzioni per semplificare il codice
    Object *Evaluator::evalStringAddInfix(std::string _op, Object *_left, Object *_right)
    {

        std::string leftString = ((String *)_left)->getValue();
        std::string rightString = ((String *)_right)->getValue();
        Object *str;

        if(_op == "+")
            str = new String(leftString + rightString);
        else
            str = new Error("ERROR: Cannot perform " + _op + " between two strings");

        garbageCollector.GCMark(str);
        return str;
    }

    Object *Evaluator::evalStringProductInfix(std::string _op, Object *_string, Integer *_int)
    {
        std::string strValue = _string->getValue();
        int intValue = std::stoi(_int->getValue());

        if (_op == "*")
        {
            std::string tmp = "";

            while(intValue--)
                tmp += strValue;

            return new String(tmp);
        }

        return new Error("Cannot perform " + _op + " between string and int");
    }

    Object *Evaluator::evalInfixExpression(std::string _op, Object *_left, Object *_right)
    {

        std::string leftType = _left->getType();
        std::string rightType = _right->getType();

        if(leftType == STRING_OBJ && rightType == STRING_OBJ)
            return evalStringAddInfix(_op, _left, _right);

        else if (leftType == STRING_OBJ  && rightType == INTEGER_OBJ)
            return evalStringProductInfix(_op, (String *)_left, static_cast<Integer *>(_right));

        else if (leftType == INTEGER_OBJ  && rightType == STRING_OBJ)
            return evalStringProductInfix(_op, (String *)_right, static_cast<Integer *>(_left));

        else if (leftType == INTEGER_OBJ && rightType == INTEGER_OBJ)
            return evalIntegerInfix(_op, _left, _right);

        else if (_left == __NULL__ && _right == __NULL__)
            return __NULL__;

        else if (_op == "==")
            return _left == _right ? __TRUE__ : __FALSE__;

        else if (_op == "!=")
            return _left != _right? __TRUE__ : __FALSE__;

        else
            return new Error("ERROR: Cannot perform " + leftType + " " + _op + " " + rightType);
    }

    Environment *Evaluator::extendFunctionEnv(Function *_fn, std::vector<Object *>& _args, Environment *_env)
    {
        Environment *innerEnv = _env->newInnerEnvironment();

        for(int i = 0; i < _args.size(); i++)
            innerEnv->setObject(_fn->params[i]->value, _args[i]);

        return innerEnv;
    }


    Object *Evaluator::evalCallIndex(Object *_fn, std::vector<Object *>& _args, Environment *_env)
    {
        std::string funcType = _fn->getType();

        if(funcType != FUNCTION_OBJ && funcType != BUILTIN_OBJ)
            return new Error("not a function: " + _fn->getType());

        if(funcType == BUILTIN_OBJ)
            return static_cast<Builtin *>(_fn)->fn(_args);

        if(static_cast<Function *>(_fn)->params.size() != _args.size())
            return new Error("argument length(" + std::to_string(_args.size()) + ") not equal to parameter length (" + std::to_string(static_cast<Function *>(_fn)->params.size()) + ")");

        Environment *inner = extendFunctionEnv(static_cast<Function *>(_fn), _args, _env);

        Object *evaluated = Eval(static_cast<Function *>(_fn)->block, inner);

        delete inner;

        if(evaluated->getType() == RETURN_OBJ)
            return static_cast<Return *>(evaluated)->value;


        return evaluated;
    }

    Object *Evaluator::evalArrayIndex(Array *_array, Integer *_index)
    {
        int index = _index->value;

        auto arr = _array->elements;

        try
        {
            return arr[index];
        }catch(const std::out_of_range e){
            return new Error("ERROR: Array Index: " + _index->getValue() + " out of range");
        }
    }

    Object *Evaluator::evalStringIndex(String *_string, Integer *_index)
    {
        int index = _index->value;

        try
        {
            return new String(std::string(1, _string->value[index]));
        }catch(const std::out_of_range e){
            return new Error("ERROR: String Index: " + _index->getValue() + " out of range");
        }
    }

    Object *Evaluator::evalIndex(Object *_array, Object *_index)
    {
        std::string arrayType = _array->getType();
        std::string indexType = _index->getType();

        if(arrayType == ARRAY_OBJ && indexType == INTEGER_OBJ)
            return evalArrayIndex((Array *)_array, (Integer *)_index);
        else if (arrayType == STRING_OBJ && indexType == INTEGER_OBJ)
            return evalStringIndex((String *)_array, (Integer *)_index);
        else
            return new Error("Index operator not supported in " + arrayType);
    }


    Object *Evaluator::evalIdentifier(std::string _name ,Environment *_env)
    {
        Object *identifier = _env->getObject(_name);

        if (identifier->getType() == ERROR_OBJ &&
            builtin.find(_name) != builtin.end())
                return builtin[_name];

        return identifier;
    }


    Object *Evaluator::evalProgram(Program *_program ,Environment *_env)
    {
        Object *stmt = evalStatements(_program->statements, _env);

        if (stmt->getType() == RETURN_OBJ)
            return static_cast<Return *>(stmt)->value;

        return stmt;
    }

    Object *Evaluator::Eval(ASTNode *_node, Environment *_env)
    {
        std::string type = _node->Type();

        //std::cout << "Type of node: " + type << std::endl;
        if(type == "Program"){
            return evalProgram(static_cast<Program *>(_node),
                                                     _env);
        }else if(type == "IntegerLiteral"){
            Integer *i = new Integer(static_cast<IntegerLiteral *>(_node)->value);

            garbageCollector.GCAdd(i);

            return i;

        }else if (type == "BooleanLiteral"){
            return new Boolean(static_cast<BooleanLiteral *>(_node)->value ? __TRUE__ : __FALSE__);

        }else if (type == "StringLiteral"){

            String *str = new String(static_cast<StringLiteral *>(_node)->value);

            garbageCollector.GCAdd(str);

            return str;

        }else if (type == "Identifier"){

            return evalIdentifier(static_cast<Identifier *>(_node)->value, _env);

        }else if (type == "CallExpression"){

            Object *fn = Eval(static_cast<CallExpression *>(_node)->functionName, _env);

            if (isError(fn))
                return fn;

            std::vector<Object *> args;

            for (auto *arg: static_cast<CallExpression *>(_node)->arguments)
            {
                Object *argument = Eval(arg, _env);

                if (isError(argument))
                    return argument;

                args.push_back(argument);
            }

            return evalCallIndex(fn, args, _env);

        }else if (type == "FunctionLiteral"){

            Function *fn = new Function(static_cast<FunctionLiteral *>(_node)->param,
                                        static_cast<FunctionLiteral *>(_node)->functionBody);

            garbageCollector.GCAdd(fn);

            return fn;

        }else if (type == "IndexExpression"){

            Object *arr = Eval(static_cast<IndexExpression *>(_node)->arrayVar, _env);

            if(isError(arr))
                return arr;

            _env->setObject(std::to_string((intptr_t)arr), arr);

            Object *index = Eval(static_cast<IndexExpression *>(_node)->index, _env);

            _env->table.erase(std::to_string((intptr_t)arr));

            if(isError(index))
                return index;


            return evalIndex(arr, index);

        }else if (type == "ArrayLiteral"){

            std::vector<Object *> elems;

            for (auto * element: static_cast<ArrayLiteral *>(_node)->elements)
            {
                Object *elem = Eval(element, _env);

                if (isError(elem))
                    return elem;

                elems.push_back(elem);
            }

            return new Array(elems);

        //!ATTENZIONE AL PREFIX EXPRESSION
        }else if (type == "PrefixExpression"){


            Object *right = Eval(((PrefixExpression *)_node)->right, _env);

            if(isError(right))
                return right;

            return evalPrefixExpression(static_cast<PrefixExpression *>(_node)->operation, right);

        }else if (type == "InfixExpression"){

            std::cout << ((InfixExpression *)_node)->String(); // missing == , perchè?
            Object *left = Eval(static_cast<InfixExpression *>(_node)->left,_env);

            if (isError(left))
                return left;

            //Salvataggio di dati temporanei

            _env->setObject(std::to_string((intptr_t)left),left);

            Object *right = Eval(static_cast<InfixExpression *>(_node)->right,_env);
            _env->table.erase(std::to_string((intptr_t)left));

            if (isError(right))
                return right;


            return evalInfixExpression(((InfixExpression*)_node)->operation, left, right);

        }else if (type == "IfExpression"){


            Object *condition = Eval(((IfExpression *)_node)->condition, _env);



            if (isError(condition))
                return condition;

            if (isTrue(condition)){
                return Eval(static_cast<IfExpression *>(_node)->ifBlock, _env);}
            else if (static_cast<IfExpression *>(_node)->elseBlock != nullptr)
                return Eval(static_cast<IfExpression *>(_node)->elseBlock, _env);
            else
                return __NULL__;

        }else if (type == "WhileExpression"){

            while(true)
            {

                Object *condition = Eval(static_cast<WhileExpression *>(_node)->condition, _env);


                if (isError(condition))
                    return condition;

                if (!isTrue(condition))
                    return __NULL__;

                Object *result = Eval(((WhileExpression *)_node)->whileBlock, _env);



                if (result->getType() == ERROR_OBJ || result->getType() == RETURN_OBJ)
                    return result;
            }

        }else if (type == "ExpressionStatement"){

            return Eval(((ExpressionStatement *)_node)->expression, _env);

        }else if (type == "BlockStatement"){

            return evalStatements(((BlockStatement *)_node)->statements, _env);

        }else if (type == "ReturnStatement"){

            Object *val = Eval(((ReturnStatement *)_node)->returnValue, _env);
            if (isError(val))
                return val;

            return new Return(val);

        }else if (type == "VarStatement"){

            Object *val = Eval(((VarStatement *)_node)->value, _env);

            if (isError(val))
                return val;

            return _env->setObject(((VarStatement *)_node)->name.value, val);

        }else if (type == "ReferenceStatement") {

            Object* val = Eval(((ReferenceStatement*)_node)->value, _env);

            if(isError(val))
                return val;

            return _env->setReferenceObject(((VarStatement *)_node)->name.value, val);

        } else {

            return __NULL__;

        }

    }
}
