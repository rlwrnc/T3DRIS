#ifdef _WIN32
#include "win32.c"
#endif

#include "t3dris.h"

// TODO:
// - hot reloading
// - basic renderer

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow *window = glfwCreateWindow(1920, 1080, "t3dris", 0, 0);
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);

	GameMemory game_memory = platform_memory_game_allocate(
			kilobytes(64),
			megabytes(512));
	game_memory.glfwGetProcAddress = glfwGetProcAddress;
	void *game_code_handle = platform_load_game_code();

	game_initialize(&game_memory);
	while (!glfwWindowShouldClose(window)) {
		platform_unload_game_code(game_code_handle);
		game_code_handle = platform_load_game_code();
		glfwPollEvents();
		game_update_and_render(&game_memory);
		glfwSwapBuffers(window);
	}

	platform_memory_game_deallocate(&game_memory);

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
