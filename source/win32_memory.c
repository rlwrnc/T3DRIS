#include <windows.h>
#include "types.h"

GameMemory win32_memory_game_allocate(size_t size_permanent, size_t size_transient)
{
	GameMemory new_memory = {0};
	new_memory.size_permanent = size_permanent;
	new_memory.size_transient = size_transient;
	new_memory.size_total = size_permanent + size_transient;
	new_memory.permanent = VirtualAlloc(
			0,
			new_memory.size_total,
			MEM_RESERVE | MEM_COMMIT,
			PAGE_READWRITE);
	new_memory.transient = (u8 *) new_memory.permanent + new_memory.size_permanent;
	return new_memory;
}

int win32_memory_game_deallocate(GameMemory *memory)
{
	memory->size_permanent = 0;
	memory->size_transient = 0;
	memory->transient = 0;
	int retval = VirtualFree(memory->permanent, memory->size_total, MEM_DECOMMIT | MEM_RELEASE);
	memory->size_total = 0;
	return retval;
}
