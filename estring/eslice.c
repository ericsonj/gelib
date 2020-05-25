/*
 * eslice.c
 *
 *  Created on: May 24, 2020
 *      Author: Ericson Joseph
 */
#include "eslice.h"
#include "emem.h"

epointer e_slice_alloc(esize block_size) {
	return e_malloc(block_size);
}


void e_slice_free1(esize mem_size, epointer mem_block) {
	e_free(mem_block);
}


