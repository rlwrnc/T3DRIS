#include <windows.h>
#include "t3dris.h"

#if TED_RELEASE == 0
game_initialize_func *game_initialize = 0;
game_update_and_render_func *game_update_and_render = 0;

void *platform_load_game_code()
{
	CopyFile("game.dll", "game_temp.dll", false);
	HMODULE library = LoadLibraryA("game_temp.dll");
	assert(library);

	game_initialize = (game_initialize_func *) GetProcAddress(library, "game_initialize");
	game_update_and_render = (game_update_and_render_func *) GetProcAddress(library, "game_update_and_render");
	return library;
}

void platform_unload_game_code(void *code_handle)
{
	FreeLibrary(code_handle);
	code_handle = 0;
	game_initialize = 0;
	game_update_and_render = 0;
}
#endif

static GameMemory platform_memory_game_allocate(size_t size_permanent, size_t size_transient)
{
#if TED_RELEASE == 0
	LPVOID base_address = (LPVOID) terabytes(3);
#else
	LPVOID base_address = 0;
#endif
	GameMemory new_memory = {0};
	new_memory.size_permanent = size_permanent;
	new_memory.size_transient = size_transient;
	new_memory.size_total = size_permanent + size_transient;
	new_memory.permanent = VirtualAlloc(
			base_address,
			new_memory.size_total,
			MEM_RESERVE | MEM_COMMIT,
			PAGE_READWRITE);
	new_memory.transient = (u8 *) new_memory.permanent + new_memory.size_permanent;
	return new_memory;
}

static inline int platform_memory_game_deallocate(GameMemory *memory)
{
	memory->size_permanent = 0;
	memory->size_transient = 0;
	memory->transient = 0;
	int retval = VirtualFree(memory->permanent, memory->size_total, MEM_DECOMMIT | MEM_RELEASE);
	memory->size_total = 0;
	return retval;
}
