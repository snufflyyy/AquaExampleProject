#pragma once

#include "aqua.h"
#include "model/model.h"

typedef struct AquaExampleProject {
	AquaProject project;

	AquaMaterial* material;
	AquaModel model;

	AquaCamera camera;
	f32 camera_yaw, camera_pitch;
	f32 mouse_sensitivity;
	f32 camera_move_speed;

	bool forward_pressed;
	bool backward_pressed;
	bool left_pressed;
	bool right_pressed;
} AquaExampleProject;

AquaExampleProject* aqua_example_project_create(void);
void aqua_example_project_destroy(AquaExampleProject* example_project);
