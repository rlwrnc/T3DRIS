#include "t3dris.c"
#include "memory.h"

#include <stdbool.h>
#include "glfw/glfw3.h"

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main()
{
	glfwInit();
	GLFWwindow *window = glfwCreateWindow(1920, 1080, "t3dris", 0, 0);
	glfwMakeContextCurrent(window);
	gladLoadGL(glfwGetProcAddress);
	glfwSetKeyCallback(window, key_callback);

	GameMemory game_memory = memory_game_allocate(kilobytes(64), megabytes(512));

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		game_update_and_render(&game_memory);
		glfwSwapBuffers(window);
	}

	memory_game_deallocate(&game_memory);

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
