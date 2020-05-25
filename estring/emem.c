/*
 * emem.c
 *
 *  Created on: May 24, 2020
 *      Author: Ericsn Joseph
 */

#include "eopt.h"
#include "etypes.h"
#include "eslice.h"
#include "emem.h"
#include "string.h"


epointer e_realloc(epointer mem, esize bytes, esize new_bytes) {
	epointer newmem;
	newmem =  e_slice_alloc(new_bytes);
	if (bytes > 0) {
		memcpy(newmem, mem, bytes);
	}
	e_slice_free1(bytes, mem);
	return newmem;
}

epointer e_malloc(esize size) {
	return ESTR_MALLOC(size);
}

void e_free(epointer ptr) {
	ESTR_FREE(ptr);
}


