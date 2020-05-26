/*
 * estring.c
 *
 *  Created on: May 24, 2020
 *      Author: Ericson Joseph
 */

#include <string.h>
#include "etypes.h"
#include "eslice.h"
#include "emem.h"
#include "emacros.h"
#include "estring.h"

static void e_str_maybe_expand(EString *string, esize len) {
	if (string->len + len >= string->allocated_len) {
		string->allocated_len = (esize) (((string->len + len) / 32) + 1) * 32;
		string->str = e_realloc(string->str, string->len,
				string->allocated_len);
	}
}

EString*
e_str_new(const echar *init) {
	EString *string;

	if (init == NULL || *init == '\0')
		string = e_str_sized_new(2);
	else {
		eint len;

		len = strlen(init);
		string = e_str_sized_new(len + 2);

		e_str_append_len(string, init, len);
	}

	return string;
}

EString* e_str_sized_new(esize dfl_size) {
	EString *string = e_slice_new(EString);

	string->allocated_len = 0;
	string->len = 0;
	string->str = NULL;

	e_str_maybe_expand(string, EMAX(dfl_size, 2));
	string->str[0] = 0;

	return string;
}

echar* e_str_free(EString *string, eboolean free_segment) {
	echar *segment;

	e_return_val_if_fail(string != NULL, NULL);

	if (free_segment) {
		e_free(string->str);
		segment = NULL;
	} else
		segment = string->str;

	e_slice_free(EString, string);

	return segment;
}

EString*
e_str_append_len(EString *string, const echar *val, essize len) {
	return e_str_insert_len(string, -1, val, len);
}

EString*
e_str_insert_len(EString *string, essize pos, const echar *val, essize len) {
	esize len_unsigned, pos_unsigned;

	e_return_val_if_fail(string != NULL, NULL);
	e_return_val_if_fail(len == 0 || val != NULL, string);

	if (len == 0)
		return string;

	if (len < 0)
		len = strlen(val);
	len_unsigned = len;

	if (pos < 0)
		pos_unsigned = string->len;
	else {
		pos_unsigned = pos;
		e_return_val_if_fail(pos_unsigned <= string->len, string);
	}

	/* Check whether val represents a substring of string.
	 * This test probably violates chapter and verse of the C standards,
	 * since ">=" and "<=" are only valid when val really is a substring.
	 * In practice, it will work on modern archs.
	 */
	if (E_UNLIKELY(val >= string->str && val <= string->str + string->len)) {
		esize offset = val - string->str;
		esize precount = 0;

		e_str_maybe_expand(string, len_unsigned);
		val = string->str + offset;
		/* At this point, val is valid again.  */

		/* Open up space where we are going to insert.  */
		if (pos_unsigned < string->len)
			memmove(string->str + pos_unsigned + len_unsigned,
					string->str + pos_unsigned, string->len - pos_unsigned);

		/* Move the source part before the gap, if any.  */
		if (offset < pos_unsigned) {
			precount = EMIN(len_unsigned, pos_unsigned - offset);
			memcpy(string->str + pos_unsigned, val, precount);
		}

		/* Move the source part after the gap, if any.  */
		if (len_unsigned > precount)
			memcpy(string->str + pos_unsigned + precount,
					val + /* Already moved: */precount +
					/* Space opened up: */len_unsigned,
					len_unsigned - precount);
	} else {
		e_str_maybe_expand(string, len_unsigned);

		/* If we aren't appending at the end, move a hunk
		 * of the old string to the end, opening up space
		 */
		if (pos_unsigned < string->len)
			memmove(string->str + pos_unsigned + len_unsigned,
					string->str + pos_unsigned, string->len - pos_unsigned);

		/* insert the new string */
		if (len_unsigned == 1)
			string->str[pos_unsigned] = *val;
		else
			memcpy(string->str + pos_unsigned, val, len_unsigned);
	}

	string->len += len_unsigned;

	string->str[string->len] = 0;

	return string;
}

/**
 * g_string_truncate:
 * @string: a #GString
 * @len: the new size of @string
 *
 * Cuts off the end of the GString, leaving the first @len bytes.
 *
 * Returns: (transfer none): @string
 */
EString*
e_str_truncate(EString *string, esize len) {
	g_return_val_if_fail(string != NULL, NULL);

	string->len = MIN(len, string->len);
	string->str[string->len] = 0;
	return string;
}
