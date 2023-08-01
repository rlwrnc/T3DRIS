#include <windows.h>
#include "t3dris.h"

#if TED_RELEASE == 0

game_initialize_func *game_initialize = 0;
game_update_and_render_func *game_update_and_render = 0;

void *platform_hotload_load_game_code(const char *compile_path, const char *load_path)
{
	CopyFile(compile_path, load_path, false);
	HMODULE code_handle = LoadLibraryA(load_path);
	assert(code_handle);

	game_initialize = (game_initialize_func *)
		GetProcAddress(code_handle, "game_initialize");
	assert(game_initialize);

	game_update_and_render = (game_update_and_render_func *)
		GetProcAddress(code_handle, "game_update_and_render");
	assert(game_update_and_render);

	return code_handle;
}

bool platform_hotload_game_has_been_compiled(const char *compile_path, const char *load_path)
{
	FILETIME compile_filetime = {0}, load_filetime = {0};
	WIN32_FILE_ATTRIBUTE_DATA file_data = {0};

	assert(GetFileAttributesExA(compile_path, GetFileExInfoStandard, &file_data));
	compile_filetime = file_data.ftLastWriteTime;
	assert(GetFileAttributesExA(load_path, GetFileExInfoStandard, &file_data));
	load_filetime = file_data.ftLastWriteTime;

	int time_difference = CompareFileTime(&compile_filetime, &load_filetime);
	return (time_difference != 0);
}

void platform_hotload_unload_game_code(void *code_handle)
{
	assert(FreeLibrary(code_handle));
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
