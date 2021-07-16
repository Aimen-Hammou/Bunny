#ifndef BUNNY_ENV_H
#define BUNNY_ENV_H

#include <unordered_map>
#include <string>
#include "object.h"

namespace bunny
{
    //!NB: NELLA CLASSE ENVIRONMENT NON CI DEVE ESSERE IN NESSUN MODO LA "delete" DI UN OGGETTO!!!
    //!l'UNICA COSA "al massimo" E' QUELLA DI STACCARE IL PUNTATORE DALLA VARIABILE table

    class Environment
    {
        public:

            std::unordered_map<std::string, Object *> table;
            Environment *outside;

            Environment();
            ~Environment();

            Environment *newInnerEnvironment();

            Object *getObject(std::string _name);
            Object *setObject(std::string _name, Object *_value);
            Object *setReferenceObject(std::string name, Object *_value);


    };
}


#endif // BUNNY_ENV_H
