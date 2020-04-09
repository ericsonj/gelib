/*
 * test_GString.c
 *
 *  Created on: Mar 11, 2020
 *      Author: Ericson Joseph
 */
#include "unity.h"
#include <string.h>

#include "getypes.h"
#include "GEString.h"

GRoot gr;
GErr  err;

void setUp(void) {
    gr.g_free   = free;
    gr.g_malloc = malloc;
}

void tearDown(void) {
}

void test_g_string_sized_new() {

    GEString* str = ge_string_sized_new(&gr, 128, &err);
    TEST_ASSERT(err == gOK);
    TEST_ASSERT(str != NULL);
    TEST_ASSERT(str->len == 0);
    TEST_ASSERT(str->allocared_len == MALLOC_BLOCK(128))
    ge_string_free(&gr, str, &err);
    TEST_ASSERT(err == gOK)
}

void test_g_string_new() {

    const gechar* kstr    = "Testing new string";
    gesize        kstrlen = strlen(kstr);

    GEString* str = ge_string_new(&gr, kstr, &err);
    TEST_ASSERT(err == gOK);
    TEST_ASSERT(str != NULL);
    TEST_ASSERT_EQUAL_STRING(kstr, str->str);
    TEST_ASSERT_EQUAL_INT(kstrlen, str->len);
    ge_string_free(&gr, str, &err);
    TEST_ASSERT(err == gOK)
}

void test_g_string_overwrite() {

	const gechar* strtest = "Hello testing";
	gesize pos = 6;
	const gechar* strow = "T";
	const gechar* expected = "Hello Testing";

    GEString* str = ge_string_new(&gr, strtest, &err);
    TEST_ASSERT(err == gOK);
    TEST_ASSERT(str != NULL);
    str = ge_string_overwrite(&gr, str, pos, strow, &err);
    TEST_ASSERT(err == gOK);
    TEST_ASSERT_EQUAL_STRING(expected, str->str);
    TEST_ASSERT(err == gOK);
    TEST_ASSERT_EQUAL_INT(strlen(expected), str->len);
    ge_string_free(&gr, str, &err);
    TEST_ASSERT(err == gOK);

}

void test_g_string_erase(){
	const gechar* strtest = "Hello testing";
	gesize pos = 5;
	gesize len = 8;
	const gechar* expected = "Hello";

    GEString* str = ge_string_new(&gr, strtest, &err);
    TEST_ASSERT(err == gOK);
    TEST_ASSERT(str != NULL);
    str = ge_string_erase(&gr, str, pos, len, &err);
    TEST_ASSERT(err == gOK);
    TEST_ASSERT_EQUAL_STRING(expected, str->str);
    TEST_ASSERT(err == gOK);
    TEST_ASSERT_EQUAL_INT(strlen(expected), str->len);
    ge_string_free(&gr, str, &err);
    TEST_ASSERT(err == gOK);
}

void test_g_string_append() {
    TEST_IGNORE_MESSAGE("implement me!");
}

//int main(void){
//
//	UNITY_BEGIN();
//
//	RUN_TEST(test_g_string_sized_new);
//	RUN_TEST(test_g_string_new);
//
//	UNITY_END();
//
//}
