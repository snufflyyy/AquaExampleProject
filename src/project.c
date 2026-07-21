#include "project.h"
#include "cimgui.h"
#include "event/event.h"
#include "gfx/camera/camera.h"
#include "gfx/material/material.h"
#include "gfx/mesh/mesh.h"
#include "gfx/renderer/renderer.h"
#include "gfx/texture/texture.h"
#include "input/input.h"
#include "model/model.h"

#include <aqua.h>

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

bool initialize(AquaEngineContext* aqua_context) {
	AquaExampleProject* project_data = (AquaExampleProject*) aqua_context->project->project_data_ptr;

	aqua_window_set_title(aqua_context->window, project_data->project.name);

	AquaTextureHandle texture = aqua_renderer_create_texture(aqua_context->renderer, "../assets/container.jpg");

	project_data->material = aqua_material_create(aqua_context->renderer, texture, (AquaColor) { 0.0f, 0.5f, 1.0f, 1.0f });
	if (!project_data->material) {
		fprintf(stderr, "[ERROR] [Aqua Example Project] Failed to create material!\n");
		return false;
	}

	project_data->model = aqua_model_create(aqua_context->renderer, "../assets/CommercialRefrigerator.glb");

	project_data->camera = aqua_camera_create((vec3) { 0.0f, 0.0f, 1.0f }, glm_rad(70.0f), ((f32) aqua_context->window->properties.width / (f32) aqua_context->window->properties.height));

	project_data->camera_pitch = 0.0f;
	project_data->camera_yaw = -90.0f;

	project_data->mouse_sensitivity = 0.2f;

	project_data->camera_move_speed = 0.5f;

	return true;
}

void event(AquaEngineContext* aqua_context, AquaEvent* event) {
	AquaExampleProject* project_data = (AquaExampleProject*) aqua_context->project->project_data_ptr;

	switch (event->type) {
		case AQUA_EVENT_TYPE_KEY_DOWN: {
			switch (event->key.scancode) {
				case AQUA_SCANCODE_ESCAPE: {
					aqua_input_manager_set_mouse_captured(&aqua_context->input_manager, !aqua_context->input_manager.properties.mouse_captured);
				} break;

				case AQUA_SCANCODE_W: { project_data->forward_pressed = true; } break;
				case AQUA_SCANCODE_A: { project_data->left_pressed = true; } break;
				case AQUA_SCANCODE_S: { project_data->backward_pressed = true; } break;
				case AQUA_SCANCODE_D: { project_data->right_pressed = true; } break;
				default: break;
			}
		} break;
		case AQUA_EVENT_TYPE_KEY_UP: {
		    switch (event->key.scancode) {
				case AQUA_SCANCODE_W: { project_data->forward_pressed = false; } break;
				case AQUA_SCANCODE_A: { project_data->left_pressed = false; } break;
				case AQUA_SCANCODE_S: { project_data->backward_pressed = false; } break;
				case AQUA_SCANCODE_D: { project_data->right_pressed = false; } break;
				default: break;
			}
		} break;
	    case AQUA_EVENT_TYPE_MOUSE_MOTION: {
			if (aqua_context->input_manager.properties.mouse_captured) {
				project_data->camera_yaw += (event->mouse_motion.x_relative * project_data->mouse_sensitivity);
				project_data->camera_pitch -= (event->mouse_motion.y_relative * project_data->mouse_sensitivity);

				if (project_data->camera_pitch > 89.0f) { project_data->camera_pitch = 89.0f; }
				if (project_data->camera_pitch < -89.0f) { project_data->camera_pitch = -89.0f; }

				vec3 direction = {
	                cosf(glm_rad(project_data->camera_yaw)) * cosf(glm_rad(project_data->camera_pitch)),
	                sinf(glm_rad(project_data->camera_pitch)),
	                sinf(glm_rad(project_data->camera_yaw)) * cosf(glm_rad(project_data->camera_pitch)),
	            };
	            glm_normalize(direction);
	            glm_vec3_copy(direction, project_data->camera.front);
			}
		} break;
		case AQUA_EVENT_TYPE_WINDOW_RESIZED: {
			aqua_camera_update_aspect(&project_data->camera, (f32) event->window.new_width / (f32) event->window.new_height);
		} break;
		default: break;
	}
}

void tick(AquaEngineContext* aqua_context) {
	AquaExampleProject* project_data = (AquaExampleProject*) aqua_context->project->project_data_ptr;

}

void update(AquaEngineContext* aqua_context) {
	AquaExampleProject* project_data = (AquaExampleProject*) aqua_context->project->project_data_ptr;

   	f32 move_speed_dt = project_data->camera_move_speed * (f32) aqua_context->renderer->delta_time;
    if (project_data->forward_pressed) {
            vec3 forward;
            glm_vec3_scale(project_data->camera.front, move_speed_dt, forward);
            glm_vec3_add(project_data->camera.position, forward, project_data->camera.position);
    }
    if (project_data->backward_pressed) {
            vec3 backward;
            glm_vec3_scale(project_data->camera.front, move_speed_dt, backward);
            glm_vec3_sub(project_data->camera.position, backward, project_data->camera.position);
    }
    if (project_data->left_pressed) {
    vec3 left;
            glm_vec3_crossn(project_data->camera.front, project_data->camera.up, left);
            glm_vec3_scale(left, move_speed_dt, left);
            glm_vec3_sub(project_data->camera.position, left, project_data->camera.position);
    }
    if (project_data->right_pressed) {
    vec3 right;
            glm_vec3_crossn(project_data->camera.front, project_data->camera.up, right);
            glm_vec3_scale(right, move_speed_dt, right);
            glm_vec3_add(project_data->camera.position, right, project_data->camera.position);
    }

	aqua_camera_update(&project_data->camera);
}

void render(AquaEngineContext* aqua_context) {
	AquaExampleProject* project_data = (AquaExampleProject*) aqua_context->project->project_data_ptr;

	aqua_renderer_clear();

	aqua_renderer_bind_camera(aqua_context->renderer, &project_data->camera);

	for (usize i = 0; i < project_data->model.submeshes_count; i++) {
        AquaSubMesh* submesh = &project_data->model.submeshes[i];

        aqua_renderer_bind_material(aqua_context->renderer, project_data->material);
        aqua_renderer_draw_submesh(aqua_context->renderer, submesh->mesh, submesh->indices_start, submesh->indices_count);
	}
}

void imgui_properties_window(AquaEngineContext* aqua_context) {
	AquaExampleProject* project_data = (AquaExampleProject*) aqua_context->project->project_data_ptr;

	igBegin("Aqua Example Project Properties", &project_data->project.show_properties_window, ImGuiWindowFlags_None);
	igCheckbox("Material Properties", &project_data->material->properties.show_properties_window);
	igCheckbox("Camera Properties", &project_data->camera.properties.show_properties_window);
	igEnd();
}

void imgui_update(AquaEngineContext* aqua_context) {
	AquaExampleProject* project_data = (AquaExampleProject*) aqua_context->project->project_data_ptr;

	aqua_material_imgui_update(project_data->material);
	aqua_camera_imgui_update(&project_data->camera);

	if (project_data->project.show_properties_window) {
		imgui_properties_window(aqua_context);
	}
}

void destroy(AquaEngineContext* aqua_context) {
	AquaExampleProject* project_data = (AquaExampleProject*) aqua_context->project->project_data_ptr;

	aqua_material_destroy(project_data->material);
}

AquaExampleProject* aqua_example_project_create(void) {
	AquaExampleProject* project_data = (AquaExampleProject*) malloc(sizeof(AquaExampleProject));
	if (!project_data) {
		fprintf(stderr, "[ERROR] [Aqua] [Project] Failed to allocate memory for project data!\n");
		return NULL;
	}

	project_data->project = (AquaProject) {
		.name = "Aqua Example Project",
		.creator = "Braden",

		.show_properties_window = false,

		.project_data_ptr = (void*) project_data,

		.initialize = initialize,
		.event = event,
		.tick = tick,
		.update = update,
		.render = render,
		.imgui_update = imgui_update,
		.destroy = destroy,
	};

	return project_data;
}

void aqua_example_project_destroy(AquaExampleProject* example_project) {
	assert(example_project != NULL);
	free(example_project);
}
