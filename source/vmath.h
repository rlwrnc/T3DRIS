#pragma once

#include <math.h>
#include <string.h>
#include <stdarg.h>

// TODO:
// - add projection matrix transformations
// - replace gimble rotations with quaternion rotations

/*
NOTE: vmath only provides vector/matrix math plus some useful transforms
it does not combine these transformations; that is for the renderer to do.

example model matrix calculation:
mat4 model = IDENTITY4, translation, rotation, scale;
*/

#define TAU 6.28318531f
#define IDENTITY4 {[0][0] = 1.0, [1][1] = 1.0, [2][2] = 1.0, [3][3] = 1.0}
#define square(n) n * n

typedef float vec3[3];
typedef float vec4[4];

typedef vec4 mat4[4];

typedef float quat[4];

static inline void mat4_identity(mat4 m)
{
	memset(m, 0.0, 16 * sizeof (float));
	m[0][0] = 1.0;
	m[1][1] = 1.0;
	m[2][2] = 1.0;
	m[3][3] = 1.0;
}

static inline void mat4_add(mat4 result, mat4 left, mat4 right)
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			result[i][j] = left[i][j] + right[i][j];
}

static inline void mat4_multiply(mat4 result, mat4 left, mat4 right)
{
	mat4 product = {0};
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			for (int k = 0; k < 4; k++)
				product[i][j] += left[k][j] * right[i][k];
	memcpy(result, product, sizeof product);
}

static inline void mat4_translate(mat4 result, float x, float y, float z)
{
	mat4_identity(result);
	result[3][0] = x;
	result[3][1] = y;
	result[3][2] = z;
}

// NOTE: all angles are expressed in TURNS
static inline void mat4_rotate_x(mat4 result, float x)
{
	float sinx = sinf(TAU * x), cosx = cosf(TAU * x);
	mat4_identity(result);
	result[0][0] = 1.0;
	result[1][1] = cosx;
	result[2][2] = cosx;
	result[3][3] = 1.0;
	result[1][2] = sinx;
	result[2][1] = -sinx;
}

static inline void mat4_rotate_y(mat4 result, float y)
{
	float siny = sinf(TAU * y), cosy = cosf(TAU * y);
	mat4_identity(result);
	result[0][0] = cosy;
	result[1][1] = 1.0;
	result[2][2] = cosy;
	result[3][3] = 1.0;
	result[0][2] = -siny;
	result[2][0] = siny;
}

static inline void mat4_rotate_z(mat4 result, float z)
{
	float sinz = sinf(TAU * z), cosz = cosf(TAU * z);
	mat4_identity(result);
	result[0][0] = cosz;
	result[1][1] = cosz;
	result[2][2] = 1.0;
	result[3][3] = 1.0;
	result[0][1] = sinz;
	result[1][0] = -sinz;
}

static inline void mat4_rotate(mat4 result, float x, float y, float z)
{
	mat4 rx, ry, rz, rzy, rzyx;
	mat4_rotate_x(rx, x);
	mat4_rotate_y(ry, y);
	mat4_rotate_z(rz, z);
	mat4_multiply(rzy, rz, ry);
	mat4_multiply(rzyx, rzy, rx);
	memcpy(result, rzyx, sizeof rzyx);
}

static inline void mat4_scale(mat4 result, float x, float y, float z)
{
	mat4_identity(result);
	result[0][0] = x;
	result[1][1] = y;
	result[2][2] = z;
	result[3][3] = 1;
}

static inline void mat4_perspective(mat4 result, float fov, float aspect_ratio,
		float n, float f)
{
	float half_tan = tanf(1.0 / (TAU * fov));
	result[0][0] = 1.0 / (aspect_ratio * half_tan);
	result[1][1] = 1.0 / (half_tan);
	result[2][2] = (f + n) / (f - n);
	result[2][3] = -1.0;
	result[3][2] = (2 * f * n) / (f - n);
}

static inline void mat4_rotation(mat4 result, quat rotation)
{
	memset(result, 0.0, 16 * sizeof (float));

	result[0][0] = 1 - 2 * (square(rotation[2]) + square(rotation[3]));
	result[0][1] = 2 * (rotation[1] * rotation[2] + rotation[3] * rotation[0]);
	result[0][2] = 2 * (rotation[1] * rotation[3] + rotation[2] * rotation[0]);

	result[1][0] = 2 * (rotation[1] * rotation[2] - rotation[3] * rotation[0]);
	result[1][1] = 1 - 2 * (square(rotation[1]) + square(rotation[3]));
	result[1][2] = 2 * (rotation[2] * rotation[3] + rotation[1] * rotation[0]);

	result[2][0] = 2 * (rotation[1] * rotation[3] + rotation[2] * rotation[0]);
	result[2][1] = 2 * (rotation[2] * rotation[3] + rotation[1] * rotation[0]);
	result[2][2] = 1 - 2 * (square(rotation[1]) - square(rotation[2]));

	result[3][3] = 1.0;
}
