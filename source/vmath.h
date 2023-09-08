#pragma once

#include <math.h>
#include <string.h>
#include <stdarg.h>

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

typedef vec4 quat;

static inline float vec3_magnitude(vec3 vector)
{
	float result = sqrtf(square(vector[0]) + square(vector[1]) + square(vector[2]));
	return result;
}

static inline void vec3_add(vec3 result, vec3 left, vec3 right)
{
	result[0] = left[0] + right[0];
	result[1] = left[1] + right[1];
	result[2] = left[2] + right[2];
}

static inline void vec3_subtract(vec3 result, vec3 left, vec3 right)
{
	result[0] = left[0] - right[0];
	result[1] = left[1] - right[1];
	result[2] = left[2] - right[2];
}

static inline void vec3_multiply(vec3 result, float scalar, vec3 vector)
{
	result[0] = scalar * vector[0];
	result[1] = scalar * vector[1];
	result[2] = scalar * vector[2];
}

static inline void vec3_normalize(vec3 result)
{
	float magnitude = vec3_magnitude(result);
	result[0] /= magnitude;
	result[1] /= magnitude;
	result[2] /= magnitude;
}

static inline void vec3_cross(vec3 result, vec3 left, vec3 right)
{
	result[0] = left[1] * right[2] - left[2] * right[1];
	result[1] = left[2] * right[0] - left[0] * right[2];
	result[2] = left[0] * right[1] - left[1] * right[0];
}

static inline float vec3_dot(vec3 left, vec3 right)
{
	return left[0] * right[0] + left[1] * right[1] + left[2] * right[2];
}

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

static inline void quat_to_mat4(mat4 result, quat rotation)
{
	float r = rotation[0], i = rotation[1], j = rotation[2], k = rotation[3];
	float s = 1 / (square(r) + square(i) + square(j) + square(k));

	result[0][0] = 1 - 2 * s * (square(j) + square(k));
	result[0][1] = 2 * s * (i * j + k * r);
	result[0][2] = 2 * s * (i * k - j * r);
	result[0][3] = 0.0;

	result[1][0] = 2 * s * (i * j - k * r);
	result[1][1] = 1 - 2 * s * (square(i) + square(k));
	result[1][2] = 2 * s * (j * k + i * r);
	result[1][3] = 0.0;

	result[2][0] = 2 * s * (i * k + j * r);
	result[2][1] = 2 * s * (j * k - i * r);
	result[2][2] = 1 - 2 * s * (square(i) + square(j));
	result[2][3] = 0.0;

	result[3][0] = 0.0;
	result[3][1] = 0.0;
	result[3][2] = 0.0;
	result[3][3] = 1.0;
}

// NOTE: all angles are expressed in TURNS
static inline void mat4_rotate(mat4 result, float angle, vec3 axis)
{
	quat rotation;
	float sina_halves = sinf((TAU * angle) / 2.0);
	rotation[0] = cosf((TAU * angle) / 2.0);
	rotation[1] = sina_halves * axis[0];
	rotation[2] = sina_halves * axis[1];
	rotation[3] = sina_halves * axis[2];
	quat_to_mat4(result, rotation);
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
	float half_tan = tanf((TAU * fov) / 2.0);
	float top = half_tan * n;
	float right = top * aspect_ratio;

	result[0][0] = n / right;
	result[1][1] = n / top;
	result[2][2] = -(f + n) / (f - n);
	result[2][3] = -1.0;
	result[3][2] = (-2 * f * n) / (f - n);
	result[3][3] = 0.0;
}

static inline void mat4_lookat(mat4 result, vec3 from, vec3 to)
{
	vec3 forward = {0};
	vec3_subtract(forward, from, to);
	vec3_normalize(forward);

	vec3 right = {0};
	vec3 y_base = {0.0, 1.0, 0.0};
	vec3_cross(right, y_base, forward);
	vec3_normalize(right);

	vec3 up = {0};
	vec3_cross(up, forward, right);

	vec3 negative_from;
	vec3_subtract(negative_from, (vec3) {0}, from);

	mat4 rotation = IDENTITY4;
	rotation[0][0] = right[0];
	rotation[0][1] = up[0];
	rotation[0][2] = forward[0];
	rotation[1][0] = right[1];
	rotation[1][1] = up[1];
	rotation[1][2] = forward[1];
	rotation[2][0] = right[2];
	rotation[2][1] = up[2];
	rotation[2][2] = forward[2];

	mat4 translation = IDENTITY4;
	mat4_translate(translation, -from[0], -from[1], -from[2]);

	mat4_multiply(result, rotation, translation);
}
