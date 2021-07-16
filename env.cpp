#include "env.h"


namespace bunny
{
    Environment::Environment() : outside(nullptr)
    {
        return;
    }

    Environment::~Environment()
    {
        table.clear();
    }

    Environment *Environment::newInnerEnvironment()
    {
        Environment *innerEnv = new Environment();
        innerEnv->outside = this; // Il livello superiore all'innerEnv
        return innerEnv;
    }

    Object *Environment::getObject(std::string _name)
    {
        if(table.find(_name) == table.end())
        {
            if(outside != nullptr) //proviamo a salire di livello per cercare l'oggetto
            {
                Object *obj = outside->getObject(_name);
                return obj;
            }

            //Non c'è un livello superiore quindi l'oggetto non esiste
            return new Error("ERROR: identifier " + _name + " not found");
        }

        // Trovato nel livello corrente

        return table[_name];
    }

    //!TODO: Permettere l'assegnazione di variabili di livelli superiori
    Object *Environment::setObject(std::string _name, Object *_value)
    {

        if(_value->getType() == RETURN_OBJ) // var x = add(1, 2)
            table[_name] = ( (Return*) _value)->value;
        else
            table[_name]  = _value; //var x = 3;

        return __NULL__;
    }

    //!TODO: Permettere l'assegnazione di variabili di livelli superiori
    Object *Environment::setReferenceObject(std::string _name, Object *_value)
    {
        if(table.find(_name) == table.end())
            return new Error("ERROR: identifier " + _name + " not found in his scope");

        Object *oldObj = getObject(_name);

        //!TODO: PERMETTERE LA CONVERSIONE AUTOMATICA TRA TIPI DIVERSI(?)
        if(oldObj->getType() != _value->getType())
            return new Error("ERROR: Can't convert from different types yet!");


        //!Grazie a Bjarne per non supportare un virtual copy costructor
        //!TODO: trovare un modo migliore di fare questo

        if(_value->getType() == INTEGER_OBJ)
            *static_cast<Integer *>(oldObj) = Integer(static_cast<Integer *>(_value)->value);

        else if(_value->getType() == STRING_OBJ)
            *static_cast<String *>(oldObj) = String(static_cast<String *>(_value)->value);

        else if(_value->getType() == RETURN_OBJ)
            *static_cast<Return *>(oldObj) = setReferenceObject(_name, static_cast<Return *>(_value)->value);

        else if(_value->getType() == FUNCTION_OBJ)
            *static_cast<Function *>(oldObj) = Function(static_cast<Function *>(_value)->params,
                                                        static_cast<Function *>(_value)->block);

        else if(_value->getType() == ARRAY_OBJ)
            *static_cast<Array *>(oldObj) = Array(static_cast<Array *>(_value)->elements);

        else
            setObject(_name, _value);

        return __NULL__;
    }
}
