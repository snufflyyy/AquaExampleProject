#include <stdio.h>

#include <aqua.h>
#include <stdlib.h>

#include "project.h"

int main(void) {
	AquaExampleProject* example_project = aqua_example_project_create();
	if (!example_project) {
		fprintf(stderr, "[ERROR] [Aqua Example Project] Failed to create aqua example project!\n");
		return EXIT_FAILURE;
	}

	AquaEngineContext* aqua_context = aqua_engine_context_create(&example_project->project, AQUA_ENGINE_CONTEXT_DEFAULT_INITIALIZE_PROPERTIES);
	if (!aqua_context) {
		fprintf(stderr, "[ERROR] [Aqua Example Project] Failed to create aqua engine context!\n");
		aqua_example_project_destroy(example_project);
		return EXIT_FAILURE;
	}

	aqua_engine_run(aqua_context);

	aqua_engine_context_destroy(aqua_context);
	aqua_example_project_destroy(example_project);

	return EXIT_SUCCESS;
}
