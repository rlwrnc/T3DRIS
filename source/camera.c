#pragma once

#include "t3dris.h"

#define CAMERA_INITIAL_THETA 0.785398163397
#define CAMERA_MAX_THETA 7.06858347058
#define CAMERA_MIN_THETA -5.49778714378
#define CAMERA_ROTATION_STEP 1.57079632679

typedef enum CameraOrientation CameraOrientation;
typedef struct Camera Camera;

enum CameraOrientation {
	CAM_NW, CAM_SW, CAM_SE, CAM_NE
};

struct Camera {
	float theta, phi, radius;	// spherical coords
	float target_theta;
	float rotation_speed;
	float rotation_step;
	vec3 position;			// cartesian coords
	vec3 target;
	CameraOrientation orientation;
	i8 rotation_direction;		// >0 for clockwise, <0 for counter-clockwise
};

void camera_rotate(Camera *camera, float delta_time)
{
	if (camera->rotation_direction > 0) { // CLOCKWISE
		camera->theta += camera->rotation_speed * delta_time;

		if (camera->theta > camera->target_theta) {
			camera->theta = camera->target_theta;
			camera->rotation_direction = 0;
			fprintf(stderr, "%f\n", camera->target_theta);
		}

		if (camera->theta > CAMERA_MAX_THETA) {
			camera->theta = CAMERA_INITIAL_THETA;
			camera->target_theta = CAMERA_INITIAL_THETA;
			camera->rotation_direction = 0;
			fprintf(stderr, "%f\n", camera->target_theta);
		}
	}

	if (camera->rotation_direction < 0) { // COUNTER-CLOCKWISE
		camera->theta -= camera->rotation_speed * delta_time;
		
		if (camera->theta < camera->target_theta) {
			camera->theta = camera->target_theta;
			camera->rotation_direction = 0;
			fprintf(stderr, "%f\n", camera->target_theta);
		}

		if (camera->theta < CAMERA_MIN_THETA) {
			camera->theta = CAMERA_INITIAL_THETA;
			camera->target_theta = CAMERA_INITIAL_THETA;
			camera->rotation_direction = 0;
			fprintf(stderr, "%f\n", camera->target_theta);
		}
	}

	camera->position[0] = camera->radius * sinf(camera->phi) * sinf(camera->theta) + 4.5;
	camera->position[1] = camera->radius * cosf(camera->phi) + 10.0;
	camera->position[2] = camera->radius * sinf(camera->phi) * cosf(camera->theta) + 4.5;
}
