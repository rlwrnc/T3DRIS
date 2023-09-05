#pragma once

/*
    NOTE: The preprocessor flags are defined as follows:

    TED_RELEASE: define to turn off debug nonsense
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

/* #include "glfw/glfw3.h" */
#include "vmath.h"
#include "opengl.h"
#include "memory.c"

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
typedef struct GameState GameState;
typedef struct OpenGLState OpenGLState;

#if TED_RELEASE == 0
typedef void game_initialize_func(GameMemory *memory, OpenGLFunctions *gl_funcs);
typedef void game_update_and_render_func(GameMemory *memory);
#else
void game_initialize(GameMemory *);
void game_update_and_render(GameMemory *);
#endif

struct GameState {
	GLuint vbo, vao, ebo, program;
	GLint model_location, view_location, projection_location;
	mat4 model, view, projection;
	GLint header;
	GLuint *cube_elements;
	float game_board[10][10][20];
};

static OpenGLFunctions gl;
