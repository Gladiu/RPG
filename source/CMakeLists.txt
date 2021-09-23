cmake_minimum_required(VERSION 3.16.3)
project(VideoGame) 
set(CMAKE_BUILD_TYPE Debug)
add_compile_options(-g)

add_executable(Game main.c
		render/sprites.c
	)
target_link_libraries(Game
	-lGLEW -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -lSOIL
		)
target_link_directories(Game 
	PUBLIC -I/usr/include/GLFW/glfw3.h
		)

