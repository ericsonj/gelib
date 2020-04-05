/*
 * GString.h
 *
 *  Created on: Mar 3, 2020
 *      Author: Ericson Joseph
 */
/*<test>*/

#ifndef GSTRING_GSTRING_H_
#    define GSTRING_GSTRING_H_

#    include "gtypes.h"
#    include <stdarg.h>
#    include <stdint.h>
#    include <stdlib.h>

typedef void* (*g_port_malloc)(gsize);
typedef void (*g_port_free)(void*);

struct _GRoot {
    g_port_malloc g_malloc;
    g_port_free   g_free;
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

struct _GString {
    gchar*   str;
    uint16_t len;
    gsize    allocared_len;
};

typedef struct _GString GString;

GString* g_string_new(GRoot* root, const gchar* str, GErr* err);

GString* g_string_sized_new(GRoot* root, gsize dfl_size, GErr* err);

GString* g_string_insert(GRoot* root, GString* string, gsize idx, const gchar* val, GErr* err);

GString* g_string_append(GRoot* root, GString* string, const gchar* val, GErr* err);

void g_string_printf(GRoot* root, GString* string, GErr* err, const gchar* format, ...);

void g_string_append_printf(GRoot* root, GString* string, GErr* err, const gchar* format, ...);

GString* g_string_truncate(GString* string, gsize len);

GString* g_string_overwrite(GRoot*       root,
                            GString*     string,
                            gsize        pos,
                            const gchar* val,
                            GErr*        err);

GString* g_string_overwrite_len(GRoot*       root,
                                GString*     string,
                                gsize        pos,
                                const gchar* val,
                                gsize        len,
                                GErr*        err);

GString* g_string_erase(GRoot* root, GString* string, gsize pos, gsize len, GErr* err);

GString* g_string_set_size(GRoot* root, GString* string, gsize len, GErr* err);

void g_string_free(GRoot* root, GString* objstr, GErr* err);

#endif /* GSTRING_GSTRING_H_ */

/*</test>*/
