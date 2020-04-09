/*
 * GString.c
 *
 *  Created on: Mar 3, 2020
 *      Author: Ericon Joseph
 */

/*<test>*/

#include "GEString.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

static void g_string_append_vprintf(GRoot* root, GEString* string, GErr* err, const gechar* format, va_list cpy, va_list args);

GEString* ge_string_new(GRoot* root, const gechar* init, GErr* err) {

    GEString* gstr = (GEString*)root->ge_malloc((gesize)sizeof(GEString));
    if (gstr == NULL) {
        (*err) = gMemErr;
        return NULL;
    }
    gstr->len           = strlen(init);
    gstr->allocared_len = MALLOC_BLOCK(gstr->len);
    gstr->str           = (gechar*)root->ge_malloc(gstr->allocared_len);
    memset(gstr->str, 0, gstr->allocared_len);
    if (gstr->str == NULL) {
        root->ge_free(gstr);
        (*err) = gMemErr;
        return NULL;
    }

    memcpy(gstr->str, init, gstr->len);
    (*err) = gOK;
    return gstr;
}

GEString* ge_string_sized_new(GRoot* root, gesize dfl_size, GErr* err) {

    GEString* newStr = (GEString*)root->ge_malloc((gesize)sizeof(GEString));

    if (newStr == NULL) {
        (*err) = gMemErr;
        return NULL;
    }

    newStr->len           = 0;
    newStr->allocared_len = MALLOC_BLOCK(dfl_size);
    newStr->str           = (gechar*)root->ge_malloc(newStr->allocared_len);

    memset(newStr->str, 0, newStr->allocared_len);

    return newStr;
}

GEString* ge_string_insert(GRoot* root, GEString* string, gesize idx, const gechar* val, GErr* err) {

    if (idx > string->len || idx < 0) {
        (*err) = gIdxErr;
        return NULL;
    }

    GEString* res      = NULL;
    uint16_t valLen   = strlen(val);
    uint16_t totalLen = string->len + valLen;

    if (totalLen > string->allocared_len) {

        /* g_string_sized_new*/
        gechar* newStr = root->ge_malloc(MALLOC_BLOCK(totalLen));
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

        root->ge_free(string->str);

        string->str           = newStr;
        string->len           = totalLen;
        string->allocared_len = MALLOC_BLOCK(totalLen);

        res = string;

    } else {

        uint16_t index = idx;
        gechar*   tmp   = root->ge_malloc(string->allocared_len);
        memcpy(tmp, string->str, string->len);

        strncpy(string->str, tmp, idx);
        strncpy(&(string->str[index]), val, valLen);
        index = index + valLen;
        strncpy(&(string->str[index]), &(tmp[idx]), string->len - idx);
        string->len = totalLen;

        res = string;

        root->ge_free(tmp);
    }

    (*err) = gOK;
    return res;
}

GEString* ge_string_append(GRoot* root, GEString* string, const gechar* val, GErr* err) {
    return ge_string_insert(root, string, string->len, val, err);
}

void g_string_append_vprintf(GRoot* root, GEString* string, GErr* err, const gechar* format, va_list cpy, va_list args) {

    if (string == NULL) {
        return;
    }
    if (format == NULL) {
        return;
    }

    gesize len = 0;
    len       = vsnprintf(NULL, 0, format, cpy);

    if (len >= 0) {
        if (string->len + len > string->allocared_len) {
            gesize  total  = string->len + len;
            gechar* newstr = root->ge_malloc(MALLOC_BLOCK(total));
            memcpy(newstr, string->str, string->len);
            vsnprintf(&(newstr[string->len]), len + 1, format, args);
            root->ge_free(string->str);
            string->str           = newstr;
            string->len           = total;
            string->allocared_len = MALLOC_BLOCK(total);
        } else {
            vsnprintf(&(string->str[string->len]), len + 1, format, args);
            string->len += len;
        }
    }
}

void ge_string_printf(GRoot* root, GEString* string, GErr* err, const gechar* format, ...) {

    va_list args;
    va_list vlTestLen;
    va_start(vlTestLen, format);
    va_start(args, format);

    ge_string_truncate(string, 0);

    g_string_append_vprintf(root, string, err, format, vlTestLen, args);

    va_end(vlTestLen);
    va_end(args);
}

void ge_string_append_printf(GRoot* root, GEString* string, GErr* err, const gechar* format, ...) {

    va_list args;
    va_list vlTestLen;
    va_start(vlTestLen, format);
    va_start(args, format);

    g_string_append_vprintf(root, string, err, format, vlTestLen, args);

    va_end(vlTestLen);
    va_end(args);
}

GEString* ge_string_truncate(GEString* string, gesize len) {

    if (string == NULL) {
        return NULL;
    }

    string->len              = GE_MIN(len, string->len);
    string->str[string->len] = 0;

    return string;
}

GEString* ge_string_overwrite(GRoot*       root,
                            GEString*     string,
                            gesize        pos,
                            const gechar* val,
                            GErr*        err) {

    return ge_string_overwrite_len(root, string, pos, val, strlen(val), err);
}

GEString* ge_string_overwrite_len(GRoot*       root,
                                GEString*     string,
                                gesize        pos,
                                const gechar* val,
                                gesize        len,
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

    gesize end = pos + len;

    if (end >= string->allocared_len) {

        gechar* newStr = root->ge_malloc(MALLOC_BLOCK(end));
        memcpy(newStr, string->str, string->len);

        root->ge_free(string->str);

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

GEString* ge_string_erase(GRoot* root, GEString* string, gesize pos, gesize len, GErr* err) {

    if (len < 1) {
        *(err) = gLenErr;
        return string;
    }
    if (pos >= string->len) {
        *(err) = gIdxErr;
        return string;
    }

    gesize end = pos + len;
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

void ge_string_free(GRoot* root, GEString* objstr, GErr* err) {

    if (objstr == NULL) {
        (*err) = gObjNull;
        return;
    }

    root->ge_free(objstr->str);
    objstr->len           = 0;
    objstr->allocared_len = 0;
    root->ge_free(objstr);
    (*err) = gOK;
}

/*</test>*/
