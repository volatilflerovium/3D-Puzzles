#ifndef ASSERTION_H
#define ASSERTION_H

#include <cassert>

#define assertm(exp, msg) assert(((void)msg, exp))

#endif
