/*
 * estring.h
 *
 *  Created on: May 24, 2020
 *      Author: Ericson Joseph
 */

#ifndef ESTRING_ESTRING_H_
#define ESTRING_ESTRING_H_

#include "etypes.h"

typedef struct _EString EString;

struct _EString {
	echar *str;
	esize len;
	esize allocated_len;
};

EString* e_str_new(const echar *init);

EString* e_str_sized_new(esize dfl_size);



#endif /* ESTRING_ESTRING_H_ */
