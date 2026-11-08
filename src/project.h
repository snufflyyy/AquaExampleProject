#pragma once

#include "aqua.h"

#include "fly-camera/fly-camera.h"

typedef struct AquaExampleProject {
	AquaProject project;

	AquaFlyCamera fly_camera;

	AquaStaticModel model;
	vec3 model_position;
	vec3 model_rotation; // euler XYZ, degrees
	vec3 model_scale;
} AquaExampleProject;

AquaExampleProject* aqua_example_project_create(void);
void aqua_example_project_destroy(AquaExampleProject* example_project);
