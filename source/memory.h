#pragma once

typedef struct {
	size_t size_permanent;
	size_t size_transient;
	size_t size_total;
	void *permanent;
	void *transient;
} GameMemory;

#define kilobytes(value) ((value) * 1024LL)
#define megabytes(value) (kilobytes(value) * 1024LL)
#define gigabytes(value) (megabytes(value) * 1024LL)

#ifdef _WIN32
#include "win32_memory.c"
#define memory_game_allocate win32_memory_game_allocate
#define memory_game_deallocate win32_memory_game_deallocate
#endif
