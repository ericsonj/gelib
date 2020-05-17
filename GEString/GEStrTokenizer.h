/*
 * GEStrTokenizer.h
 *
 *  Created on: Apr 28, 2020
 *      Author: Ericson Joseph
 */
/*<test>*/
#ifndef GELIB_GESTRING_GESTRTOKENIZER_H_
#define GELIB_GESTRING_GESTRTOKENIZER_H_

#include "GEString.h"

struct _GEStrTk {
    gechar*  index;
    GEString* token;
};

typedef struct _GEStrTk GEStrTk;

GEStrTk* ge_strtk_new(GRoot* root, GEString* gestr, const char* token, GErr *err);

void ge_strtk_free(GRoot *root, GEStrTk* tokenizer, GErr* err);

const GEStringRef ge_strtk_nextToken(GEString* gestr, GEStrTk* tokenizer, GErr* err);

#endif /* GELIB_GESTRING_GESTRTOKENIZER_H_ */
/*</test>*/
