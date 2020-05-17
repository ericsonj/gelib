/*
 * gtypes.h
 *
 *  Created on: Mar 9, 2020
 *      Author: ericson
 */
/*<test>*/

#ifndef GESTRING_GETYPES_H_
#define GESTRING_GETYPES_H_

#include <stdint.h>
#include <stdbool.h>

#define GE_MIN(a, b) \
        ((a < b) ? a : b)

#define MALLOC_BLOCK(l) \
        (((((l) / 32) + 1) * 32))

typedef char gechar;
typedef int32_t gessize;
typedef uint32_t gesize;
typedef int32_t geint;
typedef bool gebool;

#endif /* GESTRING_GETYPES_H_ */

/*</test>*/
