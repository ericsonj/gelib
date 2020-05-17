/*
 * GEStrTokenizer.c
 *
 *  Created on: Apr 28, 2020
 *      Author: Ericson Joseph
 */
/*<test>*/

#include <string.h>

#include "GEStrTokenizer.h"
#include "GEString.h"


GEStrTk* ge_strtk_new(GRoot* root, GEString* gestr, const char* token, GErr *err) {

	if (gestr == NULL) {
		(*err) = gErr;
		return NULL;
	}

	if (gestr->len == 0) {
		(*err) = gErr;
		return NULL;
	}

	GEStrTk* resp = root->ge_malloc(sizeof(GEStrTk));
	resp->index = &(gestr->str[0]);
	resp->token = ge_string_new(root, token, err);
	if (resp == NULL) {
		(*err) = gMemErr;
	}

	(*err) = gOK;
	return resp;
}

void ge_strtk_free(GRoot *root, GEStrTk* tokenizer, GErr* err) {
	root->ge_free(tokenizer);
	(*err) = gOK;
}

const GEStringRef ge_strtk_nextToken(GEString* gestr, GEStrTk* tokenizer, GErr* err) {

	GEStringRef ref;
	ref.len = 0;
	ref.str = NULL;

	gechar* ptrStart = tokenizer->index;

    if (ptrStart < &(gestr->str[0])) {
    	(*err) = gIdxErr;
        return ref;
    }

    if ((ptrStart > &(gestr->str[gestr->len - 1]))) {
    	(*err) = gIdxErr;
    	return ref;
    }

	gechar *ptr = ptrStart;
	while ((ptr = strstr(ptrStart, tokenizer->token->str)) != NULL) {
		ref.str = ptrStart;
		ref.len = ((gechar*) ptr) - ptrStart;
		tokenizer->index = &(ref.str[ref.len + tokenizer->token->len]);

		/*skin start and end tokens in string*/
		if (ptr == ptrStart) {
			ptrStart = tokenizer->index;
			if ((ptrStart >= &(gestr->str[gestr->len - 1]))) {
				ref.str = NULL;
				ref.len = 0;
				(*err) = gOK;
				return ref;
			}
		} else {
			break;
		}

	}

	(*err) = gOK;
	return ref;

}
/*</test>*/
