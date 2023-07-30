#pragma once

/*
    NOTE: The preprocessor flags are defined as follows:

    TED_RELEASE:
        0 - Internal debug build
	1 - Release build
*/


#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#define GLAD_GL_IMPLEMENTATION
#include "glad/gl.h"
#include "glfw/glfw3.h"

#define kilobytes(value) ((value) * 1024LL)
#define megabytes(value) (kilobytes(value) * 1024LL)
#define gigabytes(value) (megabytes(value) * 1024LL)
#define terabytes(value) (gigabytes(value) * 1024LL)

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef struct GameMemory GameMemory;

#if TED_RELEASE == 0
typedef void game_initialize_func(GameMemory *memory);
typedef void game_update_and_render_func(GameMemory *memory);
typedef GLFWglproc glfwGetProcAddress_func(const char *procname);
#else
void game_initialize(GameMemory *);
void game_update_and_render(GameMemory *);
#endif

struct GameMemory {
	size_t size_permanent;
	size_t size_transient;
	size_t size_total;
	void *permanent;
	void *transient;
	glfwGetProcAddress_func *glfwGetProcAddress;
};
