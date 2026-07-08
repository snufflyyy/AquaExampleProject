#pragma once

#include "aqua.h"

typedef struct AquaExampleProject {
	AquaProject project;

	AquaMaterial* material;
	AquaMeshHandle mesh;

	AquaCamera camera;
} AquaExampleProject;

AquaExampleProject* aqua_example_project_create(void);
void aqua_example_project_destroy(AquaExampleProject* example_project);
