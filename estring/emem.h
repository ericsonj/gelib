/*
 * emem.h
 *
 *  Created on: May 24, 2020
 *      Author: ericson
 */

#ifndef ESTRING_EMEM_H_
#define ESTRING_EMEM_H_

#include "etypes.h"

epointer e_realloc(epointer mem, esize bytes, esize new_bytes);

epointer e_memmove(epointer to, const epointer from, esize size);

epointer e_malloc(esize size);

void e_free(epointer ptr);

#endif /* ESTRING_EMEM_H_ */
