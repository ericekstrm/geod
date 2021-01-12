cc = g++ -std=c++17 -Wall -Wextra -Wpedantic -g -D GLFW_INCLUDE_NONE -D GL_GLEXT_PROTOTYPES
include_directory_both = -I src/state -I src/gui -I src/tree -I src/model -I src/shader -I src/camera -I src/light -I src/util -I src/framebuffer
include_directory_win = -I ./ -I lib -I lib/glad -I lib/glfw -I lib/glfw/include 
include_directory_linux = -I ./ -I lib

libs_windows = -lopengl32 -L lib/glfw/lib-mingw-w64 -lglfw3 -lgdi32
libs_linux = -lGL -L/usr/local/lib -lglfw3 -lrt -lm -ldl -lpthread

camera_o    = obj/Camera.o obj/Flying_Camera.o obj/Third_Person_Camera.o
framebuffer = obj/Framebuffer.o obj/Main_Image.o
gui_o       = obj/Font.o obj/Text.o obj/Word.o obj/Button.o obj/Image.o
light_o     =  obj/Light.o obj/Shadowmap.o
model_o     = obj/model_util.o obj/Model.o obj/Skybox.o obj/Terrain.o obj/Terrain_Container.o
shader_o    = obj/Shader.o obj/Model_Shader.o obj/Model_Instance_Shader.o obj/Postprocess_Shader.o
state_o     = obj/Game.o obj/Game_State.o obj/Menu_State.o
tree_o      = obj/Tree.o obj/Tree_Shadow.o
util_o      = obj/Matrix.o obj/Vector.o obj/stb_image.o obj/settings.o
glad_o      = obj/glad.o

#ifeq ($(os), Windows_NT)
	libs = $(libs_windows)
	include_directory = $(include_directory_both) $(include_directory_win)
	ofiles = $(camera_o) $(framebuffer) $(gui_o) $(light_o) $(model_o) $(shader_o) $(state_o) $(tree_o) $(util_o) $(glad_o)
	clean_command = del obj\*.o; del tsbk.exe

#else
	#libs = $(libs_linux)
	#include_directory = $(include_directory_both) $(include_directory_linux)
	#ofiles = $(camera_o) $(framebuffer) $(gui_o) $(light_o) $(model_o) $(shader_o) $(state_o) $(tree_o) $(util_o)
	#clean_command = rm obj/*.o; rm tsbk

#endif

main: main.cc $(ofiles)
	$(cc) -o tsbk main.cc $(ofiles) $(include_directory) $(libs)

# Camera

obj/Camera.o: src/camera/Camera.cc src/camera/Camera.h
	$(cc) -c -o $@ $< $(include_directory) $(libs)

obj/Flying_Camera.o: src/camera/Flying_Camera.cc src/camera/Flying_Camera.h
	$(cc) -c -o $@ $< $(include_directory) $(libs)

obj/Third_Person_Camera.o: src/camera/Third_Person_Camera.cc src/camera/Third_Person_Camera.h
	$(cc) -c -o $@ $< $(include_directory) $(libs)

# Framebuffer

obj/Framebuffer.o: src/framebuffer/Framebuffer.cc src/framebuffer/Framebuffer.h
	$(cc) -c -o $@ $< $(include_directory) $(libs)

obj/Main_Image.o: src/framebuffer/Main_Image.cc src/framebuffer/Main_Image.h
	$(cc) -c -o $@ $< $(include_directory) $(libs)

# Gui

obj/Font.o: src/gui/Font.cc src/gui/Font.h
	$(cc) -c -o $@ $< $(include_directory) $(libs)

obj/Text.o: src/gui/Text.cc src/gui/Text.h
	$(cc) -c -o $@ $< $(include_directory) $(libs)

obj/Word.o: src/gui/Word.cc src/gui/Word.h
	$(cc) -c -o $@ $< $(include_directory) $(libs)

obj/Button.o: src/gui/Button.cc src/gui/Button.h
	$(cc) -c -o $@ $< $(include_directory) $(libs)

obj/Image.o: src/gui/Image.cc src/gui/Image.h
	$(cc) -c -o $@ $< $(include_directory) $(libs)

# Light

obj/Light.o: src/light/Light.cc src/light/Light.h
	$(cc) -c -o $@ $< $(include_directory) $(libs)

obj/Shadowmap.o: src/light/Shadowmap.cc src/light/Shadowmap.h
	$(cc) -c -o $@ $< $(include_directory) $(libs)

# Model

obj/model_util.o: src/model/model_util.cc src/model/model_util.h
	$(cc) -c -o $@ $< $(include_directory) $(libs)

obj/Model.o: src/model/Model.cc src/model/Model.h
	$(cc) -c -o $@ $< $(include_directory) $(libs)

obj/Skybox.o: src/model/Skybox.cc src/model/Skybox.h
	$(cc) -c -o $@ $< $(include_directory) $(libs)

obj/Terrain.o: src/model/Terrain.cc src/model/Terrain.h
	$(cc) -c -o $@ $< $(include_directory) $(libs)

obj/Terrain_Container.o: src/model/Terrain_Container.cc src/model/Terrain_Container.h
	$(cc) -c -o $@ $< $(include_directory) $(libs)

# Shader

obj/Shader.o: src/shader/Shader.cc src/shader/Shader.h
	$(cc) -c -o $@ $< $(include_directory) $(libs)

obj/Model_Shader.o: src/shader/Model_Shader.cc src/shader/Model_Shader.h
	$(cc) -c -o $@ $< $(include_directory) $(libs)

obj/Model_Instance_Shader.o: src/shader/Model_Instance_Shader.cc src/shader/Model_Instance_Shader.h
	$(cc) -c -o $@ $< $(include_directory) $(libs)

obj/Postprocess_Shader.o: src/shader/Postprocess_Shader.cc src/shader/Postprocess_Shader.h
	$(cc) -c -o $@ $< $(include_directory) $(libs)

# State

obj/Game.o: src/state/Game.cc src/state/Game.h src/util/settings.h
	$(cc) -c -o $@ $< $(include_directory) $(libs)

obj/Game_State.o: src/state/Game_State.cc src/state/Game_State.h
	$(cc) -c -o $@ $< $(include_directory) $(libs)

obj/Menu_State.o: src/state/Menu_State.cc src/state/Menu_State.h
	$(cc) -c -o $@ $< $(include_directory) $(libs)

# Tree

obj/Tree.o: src/tree/Tree.cc src/tree/Tree.h src/tree/tree_params.h
	$(cc) -c -o $@ $< $(include_directory) $(libs)

obj/Tree_Shadow.o: src/tree/Tree_Shadow.cc src/tree/Tree_Shadow.h src/tree/tree_params.h
	$(cc) -c -o $@ $< $(include_directory) $(libs)

# Util

obj/Matrix.o: src/util/Matrix.cc src/util/Matrix.h
	$(cc) -c -o $@ $< $(include_directory) $(libs)

obj/Vector.o: src/util/Vector.cc src/util/Vector.h
	$(cc) -c -o $@ $< $(include_directory) $(libs)

obj/settings.o: src/util/settings.cc src/util/settings.h
	$(cc) -c -o $@ $< $(include_directory) $(libs)

# Libs

obj/glad.o: lib/glad/glad.c lib/glad/glad.h
	$(cc) -c -o $@ $< $(include_directory) $(libs)

obj/stb_image.o: lib/stb_image.cc lib/stb_image.h
	$(cc) -c -o $@ $< $(include_directory) $(libs)

# Testing

test: obj/test.o obj/test_main.o $(ofiles)
	g++ -std=c++17 -Wall -o tsbk_test obj/test.o obj/test_main.o $(ofiles) $(include_directory) $(libs)

obj/test.o: src/testing/test.cc
	g++ -std=c++17 -Wall -c -o $@ -I src/testing $< $(include_directory) $(libs)

obj/test_main.o: src/testing/test_main.cc
	g++ -std=c++17 -Wall -c -o $@ -I src/testing $<

# Clean

clean :
	$(clean_command)