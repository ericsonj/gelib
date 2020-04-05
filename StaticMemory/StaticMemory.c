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
#include <stdint.h>
#include <string.h>
#include <StaticMemory.h>

static uint32_t mem32Size = NUM_BLOCK_32 * (BLOCK_SIZE_32 + sizeof (struct mem_block_s));
static uint32_t mem64Size = NUM_BLOCK_64 * (BLOCK_SIZE_64 + sizeof (struct mem_block_s));
static uint32_t mem128Size = NUM_BLOCK_128 * (BLOCK_SIZE_128 + sizeof (struct mem_block_s));
static uint32_t mem256Size = NUM_BLOCK_256 * (BLOCK_SIZE_256 + sizeof (struct mem_block_s));
static uint32_t mem512Size = NUM_BLOCK_512 * (BLOCK_SIZE_512 + sizeof (struct mem_block_s));
#if __USE_BLOCK_1024__
static uint32_t mem1024Size = NUM_BLOCK_1024 * (BLOCK_SIZE_1024 + sizeof (struct mem_block_s));
#endif

static void MEM_initStaticMem(BYTE mem[], uint32_t memSize, uint32_t blockSize);
static bool MEM_blockIsFree(void *ptr);
static void MEM_setBlockUse(void *ptr);
static void *MEM_mallocBySize(BYTE mem[], uint32_t memSize, uint32_t blockSize);

void MEM_init() {
    MEM_initStaticMem(mem_32, mem32Size, BLOCK_SIZE_32);
    MEM_initStaticMem(mem_64, mem64Size, BLOCK_SIZE_64);
    MEM_initStaticMem(mem_128, mem128Size, BLOCK_SIZE_128);
    MEM_initStaticMem(mem_256, mem256Size, BLOCK_SIZE_256);
    MEM_initStaticMem(mem_512, mem512Size, BLOCK_SIZE_512);
#if __USE_BLOCK_1024__
    MEM_initStaticMem(mem_1024, mem1024Size, BLOCK_SIZE_1024);
#endif
}


int8_t MEM_pool_init(MemDinamicPool *pool){

	if (pool == NULL){
		return -1;
	}
	if (pool->pool32.addr != NULL) {
		MEM_initStaticMem(pool->pool32.addr, pool->pool32.size, BLOCK_SIZE_32);
	}
	if (pool->pool64.addr != NULL) {
		MEM_initStaticMem(pool->pool64.addr, pool->pool64.size, BLOCK_SIZE_64);
	}
	if (pool->pool128.addr != NULL) {
		MEM_initStaticMem(pool->pool128.addr, pool->pool128.size,
				BLOCK_SIZE_128);
	}
	if (pool->pool256.addr != NULL) {
		MEM_initStaticMem(pool->pool256.addr, pool->pool256.size,
				BLOCK_SIZE_256);
	}
	if (pool->pool512.addr != NULL) {
		MEM_initStaticMem(pool->pool512.addr, pool->pool512.size,
				BLOCK_SIZE_512);
	}

	return 0;
}

void *MEM_malloc(uint32_t size) {
    void *rest = NULL;
    if (size <= BLOCK_SIZE_32) {
        rest = MEM_mallocBySize(mem_32, mem32Size, BLOCK_SIZE_32);
    } else if (size <= BLOCK_SIZE_64) {
        rest = MEM_mallocBySize(mem_64, mem64Size, BLOCK_SIZE_64);
    } else if (size <= BLOCK_SIZE_128) {
        rest = MEM_mallocBySize(mem_128, mem128Size, BLOCK_SIZE_128);
    } else if (size <= BLOCK_SIZE_256) {
        rest = MEM_mallocBySize(mem_256, mem256Size, BLOCK_SIZE_256);
    } else if (size <= BLOCK_SIZE_512) {
        rest = MEM_mallocBySize(mem_512, mem512Size, BLOCK_SIZE_512);
    }
#if __USE_BLOCK_1024__
    else if (size <= BLOCK_SIZE_1024) {
        rest = MEM_mallocBySize(mem_1024, mem1024Size, BLOCK_SIZE_1024);
    }
#endif        
    else {
    }
    return rest;
}

void *MEM_pool_malloc(MemDinamicPool *pool, uint32_t size){
	if (pool == NULL) {
		return NULL;
	}
    void *rest = NULL;
    if (size <= BLOCK_SIZE_32) {
        rest = MEM_mallocBySize(pool->pool32.addr, pool->pool32.size, BLOCK_SIZE_32);
    } else if (size <= BLOCK_SIZE_64) {
        rest = MEM_mallocBySize(pool->pool64.addr, pool->pool64.size, BLOCK_SIZE_64);
    } else if (size <= BLOCK_SIZE_128) {
        rest = MEM_mallocBySize(pool->pool128.addr, pool->pool128.size, BLOCK_SIZE_128);
    } else if (size <= BLOCK_SIZE_256) {
        rest = MEM_mallocBySize(pool->pool256.addr, pool->pool256.size, BLOCK_SIZE_256);
    } else if (size <= BLOCK_SIZE_512) {
        rest = MEM_mallocBySize(pool->pool512.addr, pool->pool512.size, BLOCK_SIZE_512);
    }
#if __USE_BLOCK_1024__
    else if (size <= BLOCK_SIZE_1024) {
        rest = MEM_mallocBySize(mem_1024, mem1024Size, BLOCK_SIZE_1024);
    }
#endif
    else {
    }
    return rest;
}



void MEM_free(void *prt) {
    if (prt == NULL) {
        return;
    }
    BYTE *prt_dst = (BYTE*) prt;
    prt_dst = (prt_dst - sizeof (struct mem_block_s));
    struct mem_block_s block;
    memcpy(&block, prt_dst, sizeof (struct mem_block_s));
    if ((block.dst_size & MASK_MEM_VERSION_MSK) == MASK_MEM_VERSION) {
        block.dst_size |= MASK_MEM_FREE;
        memcpy(prt_dst, &block, sizeof (struct mem_block_s));
    }
}


void MEM_pool_free(MemDinamicPool *pool) {
	free(pool->pool32.addr);
	pool->pool32.size = 0;
	free(pool->pool64.addr);
	pool->pool64.size = 0;
	free(pool->pool128.addr);
	pool->pool128.size = 0;
	free(pool->pool256.addr);
	pool->pool256.size = 0;
	free(pool->pool512.addr);
	pool->pool512.size = 0;
}


void MEM_initStaticMem(BYTE mem[], uint32_t memSize, uint32_t blockSize) {
    uint32_t idx = 0;
    struct mem_block_s block;
    block.dst_size = 0;
    block.dst_size = blockSize & MASK_MEM_SIZE;
    block.dst_size |= MASK_MEM_FREE;
    block.dst_size |= MASK_MEM_VERSION;
    while ((idx + sizeof (struct mem_block_s) +blockSize) <= memSize) {
        memcpy(&mem[idx], &block, sizeof (struct mem_block_s));
        idx = idx + (sizeof (struct mem_block_s)) + blockSize;
    }
}

bool MEM_blockIsFree(void *ptr) {
    BYTE *dsc_size = (BYTE*) ptr;
    struct mem_block_s block;
    memcpy(&block, dsc_size, sizeof (struct mem_block_s));
    return block.dst_size & MASK_MEM_FREE;
}

void MEM_setBlockUse(void *ptr) {
    BYTE *dsc_size = (BYTE*) ptr;
    struct mem_block_s block;
    memcpy(&block, dsc_size, sizeof (struct mem_block_s));
    block.dst_size &= (~MASK_MEM_FREE);
    memcpy(ptr, &block, sizeof (struct mem_block_s));
}

void * MEM_mallocBySize(BYTE mem[], uint32_t memSize, uint32_t blockSize) {
    uint32_t idx = 0;
    while ((idx + sizeof (struct mem_block_s) +blockSize) <= memSize) {
        if (MEM_blockIsFree(&mem[idx])) {
            MEM_setBlockUse(&mem[idx]);
            bzero(&mem[idx + sizeof (struct mem_block_s)], blockSize);
            return &mem[idx + sizeof (struct mem_block_s)];
        }
        idx = idx + (sizeof (struct mem_block_s)) + blockSize;
    }
    return NULL;
}
