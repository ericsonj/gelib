/*
 * Copyright 2019 Ericson Joseph.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* 
 * File:   static_memory.h
 * Author: Ericson Joseph <ericsonjoseph@gmail.com>
 *
 * Created on March 31, 2019, 11:33 AM
 */

#ifndef STATIC_MEMORY_H
#define STATIC_MEMORY_H

#ifdef __cplusplus
extern "C" {
#endif


#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define __USE_BLOCK_1024__ 0

#if __USE_BLOCK_1024__
#define BLOCK_SIZE_1024 1024 /* 1024 BYTES */
#define NUM_BLOCK_1024  10
#endif

#define BLOCK_SIZE_512 512   /* 256 BYTES */
#define NUM_BLOCK_512 20     /* 512*20 = 10240 BYTES 10KB */

#define BLOCK_SIZE_256 256   /* 256 BYTES */
#define NUM_BLOCK_256  20    /* 256*20 = 5120 BYTES  5KB */

#define BLOCK_SIZE_128 128   /* 128 BYTES */
#define NUM_BLOCK_128  20    /* 128*20 = 2560 BYTES  2KB */

#define BLOCK_SIZE_64 64     /* 63 BYTES */
#define NUM_BLOCK_64  40     /* 64*40 = 2560 BYTES   2KB  */

#define BLOCK_SIZE_32 32     /* 32 BYTES */
#define NUM_BLOCK_32  40     /* 32*40 = 1280 BYTES   1KB */


#define MASK_MEM_VERSION_MSK 0xC000
#define MASK_MEM_VERSION 0x8000
#define MASK_MEM_SIZE 0x07FF
#define MASK_MEM_FREE 0x2000

    typedef uint8_t BYTE;

    struct mem_block_s {
        uint16_t dst_size;
    };

    BYTE mem_32[NUM_BLOCK_32   * (BLOCK_SIZE_32  + sizeof (struct mem_block_s))];
    BYTE mem_64[NUM_BLOCK_64   * (BLOCK_SIZE_64  + sizeof (struct mem_block_s))];
    BYTE mem_128[NUM_BLOCK_128 * (BLOCK_SIZE_128 + sizeof (struct mem_block_s))];
    BYTE mem_256[NUM_BLOCK_256 * (BLOCK_SIZE_256 + sizeof (struct mem_block_s))];
    BYTE mem_512[NUM_BLOCK_512 * (BLOCK_SIZE_512 + sizeof (struct mem_block_s))];
#if __USE_BLOCK_1024__
    BYTE mem_1024[NUM_BLOCK_1024 * (BLOCK_SIZE_1024 + sizeof (struct mem_block_s))];
#endif


#define MALLOC_32B_POOL(X) 	    ({.addr = (malloc( X * (BLOCK_SIZE_32  + sizeof (struct mem_block_s)))), .size = X * (BLOCK_SIZE_32  + sizeof (struct mem_block_s))})
#define MALLOC_64B_POOL(X) 	    ({.addr = (malloc( X * (BLOCK_SIZE_64  + sizeof (struct mem_block_s)))), .size = X * (BLOCK_SIZE_64  + sizeof (struct mem_block_s))})
#define MALLOC_128B_POOL(X) 	({.addr = (malloc( X * (BLOCK_SIZE_128 + sizeof (struct mem_block_s)))), .size = X * (BLOCK_SIZE_128 + sizeof (struct mem_block_s))})
#define MALLOC_256B_POOL(X) 	({.addr = (malloc( X * (BLOCK_SIZE_256 + sizeof (struct mem_block_s)))), .size = X * (BLOCK_SIZE_256 + sizeof (struct mem_block_s))})
#define MALLOC_512B_POOL(X) 	({.addr = (malloc( X * (BLOCK_SIZE_512 + sizeof (struct mem_block_s)))), .size = X * (BLOCK_SIZE_512 + sizeof (struct mem_block_s))})

    typedef struct _MemPool MemPool;

    struct _MemPool {
    	void *addr;
    	uint32_t size;
    };

    typedef struct _MemDinamicPool MemDinamicPool;

    struct _MemDinamicPool {
    	MemPool pool32;
    	MemPool pool64;
    	MemPool pool128;
    	MemPool pool256;
    	MemPool pool512;
    	MemPool pool1024;
    };


    void MEM_init();

    int8_t MEM_pool_init(MemDinamicPool *pool);

    void *MEM_malloc(uint32_t size);

    void *MEM_pool_malloc(MemDinamicPool *pool, uint32_t size);

    void MEM_free(void *prt);

    void MEM_pool_free(MemDinamicPool *pool);

#ifdef __cplusplus
}
#endif

#endif /* STATIC_MEMORY_H */

