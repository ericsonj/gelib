/*
 * eslice.h
 *
 *  Created on: May 24, 2020
 *      Author: ericson
 */

#ifndef ESTRING_ESLICE_H_
#define ESTRING_ESLICE_H_

#include "eopt.h"
#include "etypes.h"

#define e_slice_free(type, mem)                                 \
do {                                                  \
  if (1) e_slice_free1 (sizeof (type), (mem));			\
  else   (void) ((type*) 0 == (mem)); 				\
} while(0)

#define  e_slice_new(type) ((type*)e_slice_alloc(sizeof(type)))

epointer e_slice_alloc(esize block_size);

void e_slice_free1(esize mem_size, epointer mem_block);

#endif /* ESTRING_ESLICE_H_ */
