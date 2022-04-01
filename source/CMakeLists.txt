cmake_minimum_required(VERSION 3.16.3)
project(VideoGame) 
set(CMAKE_BUILD_TYPE Debug)
add_compile_options(-g)

add_executable(Game main.c
		render/tiles.c
		render/sprite.c
		logic/player.c
		logic/shape.c
		logic/map.c
		logic/destructible.c
		libs/nxjson/nxjson.c
	)
target_link_libraries(Game
	-lGLEW -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -lSOIL -lm
		)

target_link_directories(Game 
	PUBLIC -I/usr/include/GLFW/glfw3.h
		)

