#pragma once

#include <aqua.h>

#define AQUA_EXAMPLE_PROJECT_MODEL_INSTANCE_COUNT 2

typedef struct AquaExampleModelInstance {
	AquaModelInstance instance;
	vec3 position;
	vec3 rotation; // Euler XYZ, degrees
	vec3 scale;
} AquaExampleModelInstance;

typedef struct AquaExampleProject {
	AquaProject project;

	AquaFlyCamera* fly_camera;

	AquaModelHandle model;
	AquaExampleModelInstance model_instances[AQUA_EXAMPLE_PROJECT_MODEL_INSTANCE_COUNT];
	int selected_model_instance;
} AquaExampleProject;

AquaExampleProject* aqua_example_project_create(void);
void aqua_example_project_destroy(AquaExampleProject* example_project);
