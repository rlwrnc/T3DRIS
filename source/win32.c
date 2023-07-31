#include <windows.h>
#include "t3dris.h"

#if TED_RELEASE == 0
typedef struct {
	bool is_valid;
	HMODULE handle;
	FILETIME write_time;
	game_initialize_func *initialize;
	game_update_and_render_func *update_and_render;
} GameCode;

inline FILETIME platform_get_write_time(const char *filename)
{
	FILETIME write_time = {0};
	WIN32_FIND_DATA find_data;
	HANDLE find_handle = FindFirstFileA(filename, &find_data);
	
	if (find_handle == INVALID_HANDLE_VALUE) {
		// TODO: log this
		return write_time;
	}

	write_time = find_data.ftLastWriteTime;
	FindClose(find_handle);
	return write_time;
}

GameCode platform_load_game_code(
		const char *source_library_name,
		const char *temp_library_name)
{
	GameCode game_code = {0};
	game_code.write_time = platform_get_write_time(source_library_name);
	CopyFile(source_library_name, temp_library_name, false);
	game_code.handle = LoadLibraryA(temp_library_name);
	assert(game_code.handle);

	game_code.initialize = (game_initialize_func *)
		GetProcAddress(game_code.handle, "game_initialize");
	game_code.update_and_render = (game_update_and_render_func *)
		GetProcAddress(game_code.handle, "game_update_and_render");

	return game_code;
}

void platform_unload_game_code(GameCode *game_code)
{
	FreeLibrary(game_code->handle);
	game_code->handle = 0;
	game_code->initialize = 0;
	game_code->update_and_render = 0;
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
