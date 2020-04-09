/*
 * GString.h
 *
 *  Created on: Mar 3, 2020
 *      Author: Ericson Joseph
 */
/*<test>*/

#ifndef GESTRING_GESTRING_H_
#define GESTRING_GESTRING_H_


#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>

#include "getypes.h"

typedef void* (*g_port_malloc)(size_t);
typedef void (*g_port_free)(void*);

struct _GRoot {
    g_port_malloc ge_malloc;
    g_port_free   ge_free;
};

typedef struct _GRoot GRoot;

enum _GErr {
    gOK = 0,
    gErr,
    gMemErr,
    gObjNull,
    gIdxErr,
	gLenErr
};

typedef enum _GErr GErr;

struct _GEString {
    gechar*   str;
    uint16_t len;
    gesize    allocared_len;
};

typedef struct _GEString GEString;

GEString* ge_string_new(GRoot* root, const gechar* str, GErr* err);

GEString* ge_string_sized_new(GRoot* root, gesize dfl_size, GErr* err);

GEString* ge_string_insert(GRoot* root, GEString* string, gesize idx, const gechar* val, GErr* err);

GEString* ge_string_append(GRoot* root, GEString* string, const gechar* val, GErr* err);

void ge_string_printf(GRoot* root, GEString* string, GErr* err, const gechar* format, ...);

void ge_string_append_printf(GRoot* root, GEString* string, GErr* err, const gechar* format, ...);

GEString* ge_string_truncate(GEString* string, gesize len);

GEString* ge_string_overwrite(GRoot*       root,
                            GEString*     string,
                            gesize        pos,
                            const gechar* val,
                            GErr*        err);

GEString* ge_string_overwrite_len(GRoot*       root,
                                GEString*     string,
                                gesize        pos,
                                const gechar* val,
                                gesize        len,
                                GErr*        err);

GEString* ge_string_erase(GRoot* root, GEString* string, gesize pos, gesize len, GErr* err);

GEString* ge_string_set_size(GRoot* root, GEString* string, gesize len, GErr* err);

void ge_string_free(GRoot* root, GEString* objstr, GErr* err);

#endif /* GESTRING_GESTRING_H_ */

/*</test>*/
