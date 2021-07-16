#include "garbageCollector.h"


namespace bunny
{
    void GC::GCAdd(Object *_obj)
    {
        _obj->nextObject = head->nextObject; //Bisogna inserirlo dopo la head
        head->nextObject = _obj;
    }

    void GC::GCMark(Object *_obj)
    {
        if(_obj->isMarked) //Già markato
            return;

        _obj->isMarked = true;

        if(_obj->getType() == ARRAY_OBJ)
            for(auto p: static_cast<Array *>(_obj)->elements)
                GCMark(p); //Non sappiamo che tipo di oggetto, potrebbe essere anche un array quindi bisogna usare la funzione in maniera ricorsiva
    }

    void GC::GCMark(Environment *_env)
    {
        for(std::pair<std::string, Object *> p: _env->table) //Prendi la lista di variabili di uno scope
            GCMark(p.second);

        //Ci sono livelli (scope) superiori?
        if(_env->outside != nullptr)
            GCMark(_env->outside);

    }

    void GC::GCSweep()
    {
        Object *node = head;

        while(node->nextObject != nullptr)
        {
            if(node->nextObject->isMarked)
            {
                Object *tmp = node->nextObject;
                node->nextObject = tmp->nextObject;

                delete tmp;
            }
            else
            {
                node = node->nextObject;
            }

            node = head;

            while(node->nextObject != nullptr)
            {
                node->nextObject->isMarked = false;
                node = node->nextObject;
            }
        }
    }


}
