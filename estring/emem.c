/*
 * emem.c
 *
 *  Created on: May 24, 2020
 *      Author: Ericsn Joseph
 */
#include <stdlib.h>
#include <strings.h>
#include "emem.h"
#include "eopt.h"
#include "eslice.h"
#include "etypes.h"
#include "string.h"


epointer e_realloc(epointer mem, esize bytes, esize new_bytes) {
    epointer newmem;
    newmem = e_slice_alloc(new_bytes);
    if (bytes > 0) {
        memcpy(newmem, mem, bytes);
    }
    e_slice_free1(bytes, mem);
    return newmem;
}


epointer e_memmove(epointer to, const epointer from, esize size) {
    echar* prgcBuffer = NULL;
    echar* pcSource   = (echar*)from;
    echar* pcDstn     = (echar*)to;
    prgcBuffer = (echar*)e_malloc(size);
    if (prgcBuffer == NULL) {
        return NULL;
    } else {
        memcpy(prgcBuffer, pcSource, size);
        memcpy(pcDstn, prgcBuffer, size);
        e_free(prgcBuffer);
    }
    return from;
}


epointer e_malloc(esize size) {
    return ESTR_MALLOC(size);
}


void e_free(epointer ptr) {
    ESTR_FREE(ptr);
}
