#ifndef BUNNY_OBJECT_H
#define BUNNY_OBJECT_H

#include <vector>
#include <string>
#include "ast.h"

namespace bunny
{
    typedef std::string objectType;

    const objectType INTEGER_OBJ  = "INTEGER";
    const objectType BOOLEAN_OBJ  = "BOOLEAN";
    const objectType STRING_OBJ   = "STRING";
    const objectType NULL_OBJ     = "NULL";
    const objectType RETURN_OBJ   = "RETURN";
    const objectType ERROR_OBJ    = "ERROR";
    const objectType FUNCTION_OBJ = "FUNCTION";
    const objectType ARRAY_OBJ    = "ARRAY";
    const objectType BUILTIN_OBJ  = "BUILTIN";

    class Object //Interfaccia
    {
        public:
        //Variabili

        // Ci servono queste due variabili per il Garbage Collector
        // Il quale usa l'algoritmo elementare di Mark And Sweep
        // Consiste nella creazione di object come linked list e mark gli oggetti non più utili
        // Per poi svolgere lo sweeping
        Object *nextObject;
        bool    isMarked;

        //Funzioni

        Object() : nextObject(nullptr), isMarked(0){ };
        virtual ~Object() { }; // NB: ovviamente non avviene qua la distruzione di nextObject sennò la linked List non si formerebbe

        virtual objectType getType()  = 0;
        virtual objectType getValue() = 0;
    };


    // Nota: Il costruttore di object viene richiamato all'interno di ogni classe perchè ci serve per settare
    // I valori alle variabili del GC, piuttosto che farlo a mano ogni volta.
    class Integer : public Object
    {
        public:
        int value;

        Integer(int _value): Object(), value(_value) { }

        objectType getType()  {return INTEGER_OBJ; }
        objectType getValue() {return std::to_string(value);}
    };

    class Boolean : public Object
    {
        public:

        bool value;

        Boolean(bool _value): Object(), value(_value) { };

        objectType getType()  {return BOOLEAN_OBJ; }
        objectType getValue() {return (value ? "true" : "false"); }
    };

    class String: public Object
    {
        public:

        std::string value;

        String(std::string _value): Object() , value(_value) { };

        objectType getType()  {return STRING_OBJ; }
        objectType getValue() {return value; }

    };

    class Null: public Object
    {
        public:

        objectType getType()  {return NULL_OBJ; }
        objectType getValue() {return "NULL"; }
    };

    class Return: public Object
    {
        public:

        Object *value;

        Return(Object *_value): Object() , value(_value) { };

        objectType getType()  {return RETURN_OBJ; }
        objectType getValue() {return value->getValue(); }
    };

    class Error: public Object
    {
        public:

        std::string value;

        Error(std::string _value): Object() , value(_value) { };

        objectType getType()  { return ERROR_OBJ; }
        objectType getValue() {return "error: " + value; }

    };

    class Function: public Object
    {
        public:

        std::vector<Identifier *> params;
        BlockStatement *block;

        Function(std::vector<Identifier *> &_params, BlockStatement *_block): Object(), params(_params), block(_block) { };
        ~Function() //Una volta che la funzione ci serve possiamo liberare la memoria
        {
            for(auto p: params)
                delete p;

            delete block;
        }

        objectType getType()  {return FUNCTION_OBJ; }

        objectType getValue()
        {
            std::string str = "func (";

            for(auto p: params)
                str += p->String() + ", ";

            str += ")" + block->String();

            return str;
        }

    };

    class Array: public Object
    {
        public:

        std::vector<Object *> elements;

        Array(std::vector<Object *>  &_elements): Object(), elements(_elements) { };
        ~Array() //Una volta che la funzione non ci serve possiamo liberare la memoria
        {
            for(auto elem: elements)
                delete elem;

        }

        objectType getType()  {return ARRAY_OBJ; }

        objectType getValue()
        {
            std::string str = "[";

            for(auto elem: elements)
                str += elem->getValue() + ", ";

            str += "]";

            return str;
        }

    };


    class Builtin : public Object
    {
        public:

        Object* (*fn)(std::vector<Object *> &);

        Builtin(Object* (*func)(std::vector<Object *>&)): Object(), fn(func) { }

        objectType getType() {return BUILTIN_OBJ; }
        objectType getValue(){return "builtin function"; }
    };

    extern Null    *__NULL__;
    extern Boolean *__TRUE__;
    extern Boolean *__FALSE__;
};

#endif // BUNNY_OBJECT_H
