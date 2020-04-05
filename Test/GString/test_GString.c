/*
 * test_GString.c
 *
 *  Created on: Mar 11, 2020
 *      Author: Ericson Joseph
 */
#include "GString.h"
#include "gtypes.h"
#include "unity.h"
#include <string.h>

GRoot gr;
GErr  err;

void setUp(void) {
    gr.g_free   = free;
    gr.g_malloc = malloc;
}

void tearDown(void) {
}

void test_g_string_sized_new() {

    GString* str = g_string_sized_new(&gr, 128, &err);
    TEST_ASSERT(err == gOK);
    TEST_ASSERT(str != NULL);
    TEST_ASSERT(str->len == 0);
    TEST_ASSERT(str->allocared_len == MALLOC_BLOCK(128))
    g_string_free(&gr, str, &err);
    TEST_ASSERT(err == gOK)
}

void test_g_string_new() {

    const gchar* kstr    = "Testing new string";
    gsize        kstrlen = strlen(kstr);

    GString* str = g_string_new(&gr, kstr, &err);
    TEST_ASSERT(err == gOK);
    TEST_ASSERT(str != NULL);
    TEST_ASSERT_EQUAL_STRING(kstr, str->str);
    TEST_ASSERT_EQUAL_INT(kstrlen, str->len);
    g_string_free(&gr, str, &err);
    TEST_ASSERT(err == gOK)
}

void test_g_string_overwrite() {

	const gchar* strtest = "Hello testing";
	gsize pos = 6;
	const gchar* strow = "T";
	const gchar* expected = "Hello Testing";

    GString* str = g_string_new(&gr, strtest, &err);
    TEST_ASSERT(err == gOK);
    TEST_ASSERT(str != NULL);
    str = g_string_overwrite(&gr, str, pos, strow, &err);
    TEST_ASSERT(err == gOK);
    TEST_ASSERT_EQUAL_STRING(expected, str->str);
    TEST_ASSERT(err == gOK);
    TEST_ASSERT_EQUAL_INT(strlen(expected), str->len);
    g_string_free(&gr, str, &err);
    TEST_ASSERT(err == gOK);

}

void test_g_string_erase(){
	const gchar* strtest = "Hello testing";
	gsize pos = 5;
	gsize len = 8;
	const gchar* expected = "Hello";

    GString* str = g_string_new(&gr, strtest, &err);
    TEST_ASSERT(err == gOK);
    TEST_ASSERT(str != NULL);
    str = g_string_erase(&gr, str, pos, len, &err);
    TEST_ASSERT(err == gOK);
    TEST_ASSERT_EQUAL_STRING(expected, str->str);
    TEST_ASSERT(err == gOK);
    TEST_ASSERT_EQUAL_INT(strlen(expected), str->len);
    g_string_free(&gr, str, &err);
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
