//Készítette:    Gregorics Tibor
//Dátum:         2017.08.31.
//Feladat:       Programozási tételek ősosztálya

#pragma once

#include <cstddef>
#include "enumerator.hpp"

//Programozási tételek őssztály-sablonja
//Műveletek: run() - amely a programozási tétel algoritmusának végrehajtását végzi,
//           amelynek rugalmasságát a számos felülírandó/ható metódus biztosítja, valamint az
//           addEnumerator() - amely felsorolót csatol a programozási tétel algoritmusához
//Sablon paraméterek: Item - a programozási tételben felsorolt elemek típusa
template <typename Item >
class Procedure
{
    protected:
        Enumerator<Item> *_enor;

        Procedure():_enor(NULL){}
        virtual void init() = 0;
        virtual void body(const Item& current) = 0;
        virtual void first() {_enor->first();}
        virtual bool whileCond(const Item& current) const { return true; }
        virtual bool loopCond() const
                   { return !_enor->end() && whileCond(_enor->current()) ; }
    public:
        enum Exceptions { MissingEnumerator};
        virtual void run() final;
        void addEnumerator(Enumerator<Item>* en) { _enor = en;}
        virtual ~Procedure(){}
};

// Programozási tételek algoritmus váza
// virtuális metódusok: init(), body(), first(), loopCond(), whileCond(),
template <class Item>
void Procedure<Item>::run()
{
    if(_enor==NULL) throw MissingEnumerator;
    init();
    for(first(); loopCond(); _enor->next()){
        body(_enor->current());
    }
}
