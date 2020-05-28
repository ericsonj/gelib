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
#include "estring.h"
#include "main.h"

static MemDinamicPool mempool;

void* App_staticMalloc(size_t size){
	return MEM_pool_malloc(&mempool, size);
}


void App_staticFree(void* ptr){
	MEM_free(ptr);
}

int main() {
	mempool.pool32.addr = malloc(
			10 * (BLOCK_SIZE_32 + sizeof(struct mem_block_s)));
	mempool.pool32.size = 10 * (BLOCK_SIZE_32 + sizeof(struct mem_block_s));

	mempool.pool64.addr = malloc(
			10 * (BLOCK_SIZE_64 + sizeof(struct mem_block_s)));
	mempool.pool64.size = 10 * (BLOCK_SIZE_64 + sizeof(struct mem_block_s));

//	mempool.pool128.addr = malloc(
//			1 * (BLOCK_SIZE_128 + sizeof(struct mem_block_s)));
//	mempool.pool128.size = 1 * (BLOCK_SIZE_128 + sizeof(struct mem_block_s));
//
//	mempool.pool256.addr = malloc(
//			1 * (BLOCK_SIZE_256 + sizeof(struct mem_block_s)));
//	mempool.pool256.size = 1 * (BLOCK_SIZE_256 + sizeof(struct mem_block_s));
//
//	mempool.pool512.addr = malloc(
//			1 * (BLOCK_SIZE_512 + sizeof(struct mem_block_s)));
//	mempool.pool512.size = 1 * (BLOCK_SIZE_512 + sizeof(struct mem_block_s));

	mempool.pool1024.addr = NULL;
	mempool.pool1024.size = 0;

	MEM_pool_init(&mempool);
	MEM_init();

	char *msg = MEM_pool_malloc(&mempool, 20);
	MEM_free(msg);



	EString* test = e_str_new("Buffer de prueba para que lueg");
	e_str_printf(test, "Test: %s", "HOLAA");
	e_str_append_printf(test, "+++HOLASSS");
	e_str_assign(test, "HOLA");
	e_str_insert_c(test, 1, 'c');

	MEM_pool_free(&mempool);
	return 0;
}
