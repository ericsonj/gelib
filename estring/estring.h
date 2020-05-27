/*
 * estring.h
 *
 *  Created on: May 24, 2020
 *      Author: Ericson Joseph
 */

#ifndef ESTRING_ESTRING_H_
#define ESTRING_ESTRING_H_

#include "etypes.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _EString EString;

struct _EString {
    echar* str;
    esize  len;
    esize  allocated_len;
};

EString* e_str_new(const echar* init);

EString* e_str_sized_new(esize dfl_size);

echar* e_str_free(EString* string, eboolean free_segment);

eboolean g_string_equal(const EString* v,
                        const EString* v2);

euint g_string_hash(const EString* str);

EString* e_str_assign(EString*     string,
                      const echar* rval);

EString* e_str_append(EString*     string,
                      const echar* val);

EString* e_str_append_len(EString* string, const echar* val, essize len);

EString* e_str_append_c(EString* string, echar c);

EString* e_str_insert_len(EString* string, essize pos, const echar* val,
                          essize len);

EString* e_str_insert_c(EString* string,
                        essize   pos,
                        echar    c);

EString* e_str_truncate(EString* string, esize len);

void e_str_printf(EString*     string,
                  const echar* format,
                  ...);

void e_str_append_vprintf(EString*     string,
                          const echar* format,
                          va_list      args);

void e_str_append_printf(EString*     string,
                         const echar* format,
                         ...);

#endif /* ESTRING_ESTRING_H_ */
