#ifndef BUNNY_EVAL_H
#define BUNNY_EVAL_H

#include <unordered_map>
#include <functional>
#include <cstdarg>
#include "object.h"
#include "ast.h"
#include "env.h"
#include "garbageCollector.h"

namespace bunny
{
    class Evaluator
    {
        public:

            Object *Eval(ASTNode *_node, Environment *_env);

        private:

            GC garbageCollector;
            int gcCapacity = 0;

            Object *evalStatements(std::vector<Statement *> _statements, Environment *_env);
            Object *evalBang(Object *_right);
            Object *evalMinusPrefix(Object *_right);
            Object *evalPrefixExpression(std::string _op ,Object *_right);
            Object *evalIntegerInfix(std::string _op, Object *_left, Object *_right);
            Object *evalStringAddInfix(std::string _op, Object *_left, Object *_right);
            Object *evalStringProductInfix(std::string _op, Object *_string, Integer *_int);
            Object *evalInfixExpression(std::string _op, Object *_left, Object *_right);
            Object *evalIndex(Object *_array, Object *_index);
            Object *evalCallIndex(Object *_fn, std::vector<Object *>& _args, Environment *_env);
            Object *evalArrayIndex(Array *_array, Integer *_index);
            Object *evalStringIndex(String *_string, Integer *_index);
            Object *evalIdentifier(std::string _name ,Environment *_env);
            Object *evalProgram(Program *_program ,Environment *_env);

            Environment *extendFunctionEnv(Function *_fn, std::vector<Object *>& _args, Environment *_env);


    };
}


#endif // BUNNY_EVAL_H
