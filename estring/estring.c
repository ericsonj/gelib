/*
 * estring.c
 *
 *  Created on: May 24, 2020
 *      Author: Ericson Joseph
 */

#include "etypes.h"
#include "eslice.h"
#include "estring.h"
#include "emem.h"
#include "emacros.h"

static void e_str_maybe_expand(EString *string, esize len) {
	if (string->len + len >= string->allocated_len) {
		string->allocated_len = (esize) (((string->len + len) / 32) + 1) * 32;
		string->str = e_realloc(string->str, string->len,
				string->allocated_len);
	}
}

EString*
e_str_sized_new(esize dfl_size) {
	EString *string = e_slice_new(EString);

	string->allocated_len = 0;
	string->len = 0;
	string->str = NULL;

	e_str_maybe_expand(string, EMAX(dfl_size, 2));
	string->str[0] = 0;

	return string;
}

echar*
e_str_free(EString *string, eboolean free_segment) {
	echar *segment;

//  g_return_val_if_fail (string != NULL, NULL);

	if (free_segment) {
		e_free(string->str);
		segment = NULL;
	} else
		segment = string->str;

	e_slice_free(EString, string);

	return segment;
}

