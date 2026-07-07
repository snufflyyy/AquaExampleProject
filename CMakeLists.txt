cmake_minimum_required(VERSION 3.16)
project(AquaExampleProject)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(AquaEngine)

include_directories(src)

add_executable(
	${PROJECT_NAME}
	src/main.c

	src/project.c
)

target_link_libraries(${PROJECT_NAME} PRIVATE AquaEngine)

target_compile_options(${PROJECT_NAME} PRIVATE
	-Wall -Wextra -Wpedantic
	-Wconversion -Wshadow -Wstrict-prototypes -Wformat=2 -Wnull-dereference
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
	$<$<NOT:$<CONFIG:Debug>>:NDEBUG>
)
