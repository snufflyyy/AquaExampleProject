#pragma once

#include "aqua.h"

typedef struct AquaExampleProject {
	AquaProject project;

	AquaMaterial* material;
	AquaMeshHandle mesh;

	bool mouse_captured;

	AquaCamera camera;
	f32 camera_yaw, camera_pitch;
	f32 mouse_sensitivity;
} AquaExampleProject;

AquaExampleProject* aqua_example_project_create(void);
void aqua_example_project_destroy(AquaExampleProject* example_project);
