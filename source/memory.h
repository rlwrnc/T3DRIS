#pragma once

#include "types.h"

typedef struct GameMemory GameMemory;
typedef struct MemoryArena MemoryArena;

inline void arena_initialize(MemoryArena *arena, GameMemory *memory, size_t size);
inline u8 *arena_allocate(MemoryArena *arena, size_t amount);
inline void arena_reset(MemoryArena *arena);

struct MemoryArena {
        size_t size;
        u8 *base;
        u8 *top;
        u8 *previous_top;
};

struct GameMemory {
        size_t size_total;
        MemoryArena permanent;
        MemoryArena transient;
};
