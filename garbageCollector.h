#ifndef BUNNY_GC_H
#define BUNNY_GC_H

#include "env.h"

namespace bunny
{
    class GC
    {
        public:
            Object *head; //Il primo valore della linked list

            GC(): head(new String("head")) { };
            ~GC() { delete head; }; //Quando il programma termina si va a distruggere l'instanza quindi è sicuro eliminare la linked list

            void GCAdd(Object *_obj);

            void GCMark(Object *_obj);
            void GCMark(Environment *_env);

            void GCSweep();
    };
}

#endif // BUNNY_GC_H
