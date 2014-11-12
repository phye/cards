#ifndef _ICARDCOMP_H
#define _ICARDCOMP_H
#include <tractor/Card.h>

class ICardComp {
public:
    ICardComp() {}
    virtual ~ICardComp() = 0;

    bool operator() (const Card& lhs, const Card& rhs);
};

#endif
