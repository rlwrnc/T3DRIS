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

	GameCode game = platform_load_game_code("game.dll", "game_temp.dll");
	game.initialize(&game_memory);
	while (!glfwWindowShouldClose(window)) {
		FILETIME new_write_time = platform_get_write_time("game.dll");
		if (CompareFileTime(&new_write_time, &game.write_time) != 0) {
			platform_unload_game_code(&game);
			game = platform_load_game_code("game.dll", "game_temp.dll");
			game.initialize(&game_memory);
		}

		glfwPollEvents();
		game.update_and_render(&game_memory);
		glfwSwapBuffers(window);
	}

	platform_unload_game_code(&game);
	platform_memory_game_deallocate(&game_memory);

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
