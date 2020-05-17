/*
 * Copyright (c) 2020, Ericson Joseph
 * 
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 *     * Redistributions of source code must retain the above copyright notice,
 *       this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright notice,
 *       this list of conditions and the following disclaimer in the documentation
 *       and/or other materials provided with the distribution.
 *     * Neither the name of pyMakeTool nor the names of its contributors
 *       may be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <StaticMemory.h>
#include "regex.h"


#include "GEString.h"
#include "GEStrTokenizer.h"

static MemDinamicPool mempool;

int main() {
	mempool.pool32.addr = malloc(
			1 * (BLOCK_SIZE_32 + sizeof(struct mem_block_s)));
	mempool.pool32.size = 1 * (BLOCK_SIZE_32 + sizeof(struct mem_block_s));

	mempool.pool64.addr = malloc(
			1 * (BLOCK_SIZE_64 + sizeof(struct mem_block_s)));
	mempool.pool64.size = 1 * (BLOCK_SIZE_64 + sizeof(struct mem_block_s));

	mempool.pool128.addr = malloc(
			1 * (BLOCK_SIZE_128 + sizeof(struct mem_block_s)));
	mempool.pool128.size = 1 * (BLOCK_SIZE_128 + sizeof(struct mem_block_s));

	mempool.pool256.addr = malloc(
			1 * (BLOCK_SIZE_256 + sizeof(struct mem_block_s)));
	mempool.pool256.size = 1 * (BLOCK_SIZE_256 + sizeof(struct mem_block_s));

	mempool.pool512.addr = malloc(
			1 * (BLOCK_SIZE_512 + sizeof(struct mem_block_s)));
	mempool.pool512.size = 1 * (BLOCK_SIZE_512 + sizeof(struct mem_block_s));

	mempool.pool1024.addr = NULL;
	mempool.pool1024.size = 0;

	MEM_pool_init(&mempool);
	MEM_init();

	char *msg = MEM_pool_malloc(&mempool, 20);

	MEM_free(msg);

	MEM_pool_free(&mempool);

	GErr err;
	GRoot root;
	root.ge_free = MEM_free;
	root.ge_malloc = MEM_malloc;

	int j;
	for (j = 0; j < 10000; ++j) {

		GEString *str = ge_string_new(&root, "Hello World GString!!!", &err);
		int i;
		for (i = 0; i < 10; ++i) {
			str = ge_string_append(&root, str, " Add more ", &err);
		}
		str = ge_string_append(&root, str, "\r\n", &err);

		ge_string_append_printf(&root, str, &err, "%s", "HOLAAAAAA");

		ge_string_free(&root, str, &err);

	}

	GEString* str = ge_string_new(&root, "Hello;Would;GEStrTk!!!!;;;;;;;", &err);

	GEStrTk* token = ge_strtk_new(&root, str, ";", &err);

	GEStringRef ref;

	while ((ref = ge_strtk_nextToken(str, token, &err)).str != NULL) {
		printf("%.*s\n", ref.len, ref.str);
	}

	ge_string_free(&root, str, &err);
	ge_strtk_free(&root, token, &err);

	return 0;
}
