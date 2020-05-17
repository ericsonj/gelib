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
#include "GEStrTokenizer.h"

GRoot gr;
GErr  err;

void setUp(void) {
    gr.ge_free   = free;
    gr.ge_malloc = malloc;
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

	GEString* str = ge_string_sized_new(&gr, 32, &err);
    TEST_ASSERT(err == gOK);
    TEST_ASSERT(str != NULL);
    ge_string_append_printf(&gr, str, &err, "Add string ");
    TEST_ASSERT(err == gOK);
    ge_string_append_printf(&gr, str, &err, "Add number %d ", 10);
    TEST_ASSERT(err == gOK);
    ge_string_append_printf(&gr, str, &err, "Add string %s ", "Hola");
    TEST_ASSERT(err == gOK);
    ge_string_append_printf(&gr, str, &err, "Add float %4,2f ", 3.14);
    TEST_ASSERT(err == gOK);
    ge_string_free(&gr, str, &err);
    TEST_ASSERT(err == gOK);

}

void test_ge_strtk_new() {

	GEString *str = ge_string_new(&gr, "Hello;Would;GEStrTk!!!!;;;;;;;", &err);
	GEStrTk *token = ge_strtk_new(&gr, str, ";", &err);
	TEST_ASSERT(err == gOK);
	TEST_ASSERT(token != NULL);
	ge_strtk_free(&gr, token, &err);
	TEST_ASSERT(err == gOK);
	ge_string_free(&gr, str, &err);
	TEST_ASSERT(err == gOK);

}


void test_ge_strtk_nextToken() {

	GEString *str = ge_string_new(&gr, "Hello\r\nWould\r\nGEStrTk!!!!\r\n", &err);
	GEStrTk *token = ge_strtk_new(&gr, str, "\r\n", &err);
	TEST_ASSERT(err == gOK);
	TEST_ASSERT(token != NULL);

	GEStringRef ref = ge_strtk_nextToken(str, token, &err);
	if (!GEString_isEmpty(ref)) {
		TEST_ASSERT_EQUAL_MEMORY("Hello", ref.str, ref.len);
	}
	ref = ge_strtk_nextToken(str, token, &err);
	if (!GEString_isEmpty(ref)) {
		TEST_ASSERT_EQUAL_MEMORY("Would", ref.str, ref.len);
	}
	ref = ge_strtk_nextToken(str, token, &err);
	if (!GEString_isEmpty(ref)) {
		TEST_ASSERT_EQUAL_MEMORY("GEStrTk!!!!", ref.str, ref.len);
	}

	ge_strtk_free(&gr, token, &err);
	TEST_ASSERT(err == gOK);
	ge_string_free(&gr, str, &err);
	TEST_ASSERT(err == gOK);

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
