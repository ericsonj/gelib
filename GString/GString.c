/*
 * GString.c
 *
 *  Created on: Mar 3, 2020
 *      Author: Ericon Joseph
 */

/*<test>*/

#include "GString.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

static void g_string_append_vprintf(GRoot* root, GString* string, GErr* err, const gchar* format, va_list cpy, va_list args);

GString* g_string_new(GRoot* root, const gchar* init, GErr* err) {

    GString* gstr = (GString*)root->g_malloc((gsize)sizeof(GString));
    if (gstr == NULL) {
        (*err) = gMemErr;
        return NULL;
    }
    gstr->len           = strlen(init);
    gstr->allocared_len = MALLOC_BLOCK(gstr->len);
    gstr->str           = (gchar*)root->g_malloc(gstr->allocared_len);
    memset(gstr->str, 0, gstr->allocared_len);
    if (gstr->str == NULL) {
        root->g_free(gstr);
        (*err) = gMemErr;
        return NULL;
    }

    memcpy(gstr->str, init, gstr->len);
    (*err) = gOK;
    return gstr;
}

GString* g_string_sized_new(GRoot* root, gsize dfl_size, GErr* err) {

    GString* newStr = (GString*)root->g_malloc((gsize)sizeof(GString));

    if (newStr == NULL) {
        (*err) = gMemErr;
        return NULL;
    }

    newStr->len           = 0;
    newStr->allocared_len = MALLOC_BLOCK(dfl_size);
    newStr->str           = (gchar*)root->g_malloc(newStr->allocared_len);

    memset(newStr->str, 0, newStr->allocared_len);

    return newStr;
}

GString* g_string_insert(GRoot* root, GString* string, gsize idx, const gchar* val, GErr* err) {

    if (idx > string->len || idx < 0) {
        (*err) = gIdxErr;
        return NULL;
    }

    GString* res      = NULL;
    uint16_t valLen   = strlen(val);
    uint16_t totalLen = string->len + valLen;

    if (totalLen > string->allocared_len) {

        /* g_string_sized_new*/
        gchar* newStr = root->g_malloc(MALLOC_BLOCK(totalLen));
        if (newStr == gOK) {
            (*err) = gMemErr;
            return NULL;
        }

        /*insert into string*/
        uint16_t index = idx;
        strncpy(newStr, string->str, idx);
        strncpy(&(newStr[index]), val, valLen);
        index = index + valLen;
        strncpy(&(newStr[index]), &(string->str[idx]), string->len - idx);

        root->g_free(string->str);

        string->str           = newStr;
        string->len           = totalLen;
        string->allocared_len = MALLOC_BLOCK(totalLen);

        res = string;

    } else {

        uint16_t index = idx;
        gchar*   tmp   = root->g_malloc(string->allocared_len);
        memcpy(tmp, string->str, string->len);

        strncpy(string->str, tmp, idx);
        strncpy(&(string->str[index]), val, valLen);
        index = index + valLen;
        strncpy(&(string->str[index]), &(tmp[idx]), string->len - idx);
        string->len = totalLen;

        res = string;

        root->g_free(tmp);
    }

    (*err) = gOK;
    return res;
}

GString* g_string_append(GRoot* root, GString* string, const gchar* val, GErr* err) {
    return g_string_insert(root, string, string->len, val, err);
}

void g_string_append_vprintf(GRoot* root, GString* string, GErr* err, const gchar* format, va_list cpy, va_list args) {

    if (string == NULL) {
        return;
    }
    if (format == NULL) {
        return;
    }

    gsize len = 0;
    len       = vsnprintf(NULL, 0, format, cpy);

    if (len >= 0) {
        if (string->len + len > string->allocared_len) {
            gsize  total  = string->len + len;
            gchar* newstr = root->g_malloc(MALLOC_BLOCK(total));
            memcpy(newstr, string->str, string->len);
            vsnprintf(&(newstr[string->len]), len + 1, format, args);
            root->g_free(string->str);
            string->str           = newstr;
            string->len           = total;
            string->allocared_len = MALLOC_BLOCK(total);
        } else {
            vsnprintf(&(string->str[string->len]), len + 1, format, args);
            string->len += len;
        }
    }
}

void g_string_printf(GRoot* root, GString* string, GErr* err, const gchar* format, ...) {

    va_list args;
    va_list vlTestLen;
    va_start(vlTestLen, format);
    va_start(args, format);

    g_string_truncate(string, 0);

    g_string_append_vprintf(root, string, err, format, vlTestLen, args);

    va_end(vlTestLen);
    va_end(args);
}

void g_string_append_printf(GRoot* root, GString* string, GErr* err, const gchar* format, ...) {

    va_list args;
    va_list vlTestLen;
    va_start(vlTestLen, format);
    va_start(args, format);

    g_string_append_vprintf(root, string, err, format, vlTestLen, args);

    va_end(vlTestLen);
    va_end(args);
}

GString* g_string_truncate(GString* string, gsize len) {

    if (string == NULL) {
        return NULL;
    }

    string->len              = MIN(len, string->len);
    string->str[string->len] = 0;

    return string;
}

GString* g_string_overwrite(GRoot*       root,
                            GString*     string,
                            gsize        pos,
                            const gchar* val,
                            GErr*        err) {

    return g_string_overwrite_len(root, string, pos, val, strlen(val), err);
}

GString* g_string_overwrite_len(GRoot*       root,
                                GString*     string,
                                gsize        pos,
                                const gchar* val,
                                gsize        len,
                                GErr*        err) {

    if (len < 1) {
        *(err) = gLenErr;
        return string;
    }
    if (val == NULL) {
        *(err) = gObjNull;
        return string;
    }
    if (pos >= string->len) {
        *(err) = gIdxErr;
        return string;
    }

    gsize end = pos + len;

    if (end >= string->allocared_len) {

        gchar* newStr = root->g_malloc(MALLOC_BLOCK(end));
        memcpy(newStr, string->str, string->len);

        root->g_free(string->str);

        string->str           = newStr;
        string->allocared_len = MALLOC_BLOCK(end);
    }

    memcpy(&(string->str[pos]), val, len);

    if (end > string->len) {
        string->str[end] = '\0';
        string->len      = end;
    }

    (*err) = gOK;
    return string;
}

GString* g_string_erase(GRoot* root, GString* string, gsize pos, gsize len, GErr* err) {

    if (len < 1) {
        *(err) = gLenErr;
        return string;
    }
    if (pos >= string->len) {
        *(err) = gIdxErr;
        return string;
    }

    gsize end = pos + len;
    if (end <= string->len) {
        if (end < string->len) {
            memmove(&(string->str[pos]), &(string->str[end]), string->len - end);
        }
    }
    string->len -= len;
    string->str[string->len] = '\0';

    *(err) = gOK;
    return string;
}

void g_string_free(GRoot* root, GString* objstr, GErr* err) {

    if (objstr == NULL) {
        (*err) = gObjNull;
        return;
    }

    root->g_free(objstr->str);
    objstr->len           = 0;
    objstr->allocared_len = 0;
    root->g_free(objstr);
    (*err) = gOK;
}

/*</test>*/
