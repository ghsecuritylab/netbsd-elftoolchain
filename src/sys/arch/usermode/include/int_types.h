/* int_types.h,v 1.10 2012/02/05 17:40:08 reinoud Exp */

/*
 * Automatically generated by genheaders.sh on Sun Feb  5 18:39:34 CET 2012
 * Do not modify directly!
 */
#ifndef _USERMODE_INT_TYPES_H
#define _USERMODE_INT_TYPES_H

#if defined(__i386__)
#include "../../i386/include/int_types.h"
#elif defined(__x86_64__)
#include "../../amd64/include/int_types.h"
#else
#error port me
#endif

#endif