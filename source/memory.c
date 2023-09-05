#pragma once

#include <assert.h>
#include "memory.h"

// NOTE: only transient memory can have sub-arenas
inline void arena_initialize(MemoryArena *arena, GameMemory *memory, size_t size)
{
	arena->size = size;
	arena->base = arena_allocate(&memory->transient, size);
	arena->top = arena->base;
	arena->previous_top = 0;
}

inline u8 *arena_allocate(MemoryArena *arena, size_t amount)
{
	assert(amount < arena->size - (arena->top - arena->base));
	arena->previous_top = arena->top;
	arena->top += amount;
	return arena->previous_top;
}

inline u8 *arena_peek(MemoryArena *arena)
{
	assert(arena->previous_top != 0);
	return arena->previous_top;
}

inline void arena_reset(MemoryArena *arena)
{
	arena->top = arena->base;
	arena->previous_top = 0;
}
