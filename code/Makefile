cc = g++ -std=c++17 -Wall -Wextra -Wpedantic -g -D GLFW_INCLUDE_NONE -D GL_GLEXT_PROTOTYPES
include_directory = -I ./ -I lib -I src/state -I src/gui -I src/tree -I src/model -I src/shader -I src/camera -I src/light -I src/util -I src/framebuffer -I src/scene -I src/renderer
libs = -lGL -L/usr/local/lib -lglfw3 -lrt -lm -ldl -lpthread

camera_o    = obj/Camera.o obj/Flying_Camera.o obj/Third_Person_Camera.o
framebuffer = obj/Framebuffer.o obj/Main_Image.o
gui_o       = obj/Font.o obj/Text.o obj/Word.o obj/Button.o obj/Image.o obj/Slider.o
light_o     =  obj/Light.o obj/Shadowmap.o
model_o     = obj/model_util.o obj/Model.o obj/Skybox.o obj/Terrain.o
renderer_o  = obj/Renderer.o
scene_o     = obj/Scene.o obj/Road.o obj/Bezier.o obj/Lane.o obj/Water.o
shader_o    = obj/Shader.o
state_o     = obj/Game.o obj/Game_State.o obj/Menu_State.o obj/Settings_Menu_State.o obj/Main_Menu_State.o
util_o      = obj/Matrix.o obj/Vector.o obj/stb_image.o obj/settings.o obj/Settings.o obj/Heightmap.o
glad_o      = obj/glad.o
ofiles = $(camera_o) $(framebuffer) $(gui_o) $(light_o) $(model_o) $(renderer_o) $(scene_o) $(shader_o) $(state_o) $(tree_o) $(util_o)

main: main.cc $(ofiles)
	$(cc) -o geod main.cc $(ofiles) $(include_directory) $(libs)

run: main.cc $(ofiles)
	$(cc) -o geod main.cc $(ofiles) $(include_directory) $(libs)
	sudo ./geod

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

obj/Shadowmap.o: src/framebuffer/Shadowmap.cc src/framebuffer/Shadowmap.h
	$(cc) -c -o $@ $< $(include_directory) $(libs)

# Gui

obj/Button.o: src/gui/Button.cc src/gui/Button.h
	$(cc) -c -o $@ $< $(include_directory) $(libs)

obj/Font.o: src/gui/Font.cc src/gui/Font.h
	$(cc) -c -o $@ $< $(include_directory) $(libs)

obj/Image.o: src/gui/Image.cc src/gui/Image.h
	$(cc) -c -o $@ $< $(include_directory) $(libs)

obj/Slider.o: src/gui/Slider.cc src/gui/Slider.h
	$(cc) -c -o $@ $< $(include_directory) $(libs)

obj/Text.o: src/gui/Text.cc src/gui/Text.h
	$(cc) -c -o $@ $< $(include_directory) $(libs)

obj/Word.o: src/gui/Word.cc src/gui/Word.h
	$(cc) -c -o $@ $< $(include_directory) $(libs)

# Light

obj/Light.o: src/light/Light.cc src/light/Light.h
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

# Renderer

obj/Renderer.o: src/renderer/Renderer.cc src/renderer/Renderer.h
	$(cc) -c -o $@ $< $(include_directory) $(libs)

# Scene

obj/Bezier.o: src/scene/Bezier.cc src/scene/Bezier.h
	$(cc) -c -o $@ $< $(include_directory) $(libs)

obj/Lane.o: src/scene/Lane.cc src/scene/Lane.h
	$(cc) -c -o $@ $< $(include_directory) $(libs)

obj/Scene.o: src/scene/Scene.cc src/scene/Scene.h
	$(cc) -c -o $@ $< $(include_directory) $(libs)

obj/Road.o: src/scene/Road.cc src/scene/Road.h
	$(cc) -c -o $@ $< $(include_directory) $(libs)

obj/Water.o: src/scene/Water.cc src/scene/Water.h
	$(cc) -c -o $@ $< $(include_directory) $(libs)

# Shader

obj/Shader.o: src/shader/Shader.cc src/shader/Shader.h
	$(cc) -c -o $@ $< $(include_directory) $(libs)

# State

obj/Game.o: src/state/Game.cc src/state/Game.h src/util/settings.h
	$(cc) -c -o $@ $< $(include_directory) $(libs)

obj/Game_State.o: src/state/Game_State.cc src/state/Game_State.h
	$(cc) -c -o $@ $< $(include_directory) $(libs)

obj/Menu_State.o: src/state/Menu_State.cc src/state/Menu_State.h
	$(cc) -c -o $@ $< $(include_directory) $(libs)

obj/Settings_Menu_State.o: src/state/Settings_Menu_State.cc src/state/Settings_Menu_State.h
	$(cc) -c -o $@ $< $(include_directory) $(libs)

obj/Main_Menu_State.o: src/state/Main_Menu_State.cc src/state/Main_Menu_State.h
	$(cc) -c -o $@ $< $(include_directory) $(libs)
	
# Util

obj/Heightmap.o: src/util/Heightmap.cc src/util/Heightmap.h
	$(cc) -c -o $@ $< $(include_directory) $(libs)

obj/Matrix.o: src/util/Matrix.cc src/util/Matrix.h
	$(cc) -c -o $@ $< $(include_directory) $(libs)

obj/Vector.o: src/util/Vector.cc src/util/Vector.h
	$(cc) -c -o $@ $< $(include_directory) $(libs)

obj/settings.o: src/util/settings.cc src/util/settings.h
	$(cc) -c -o $@ $< $(include_directory) $(libs)

obj/Settings.o: src/util/Settings.cc src/util/Settings.h
	$(cc) -c -o $@ $< $(include_directory) $(libs)

# Libs

obj/glad.o: lib/glad/glad.c lib/glad/glad.h
	$(cc) -c -o $@ $< $(include_directory) $(libs)

obj/stb_image.o: lib/stb_image.cc lib/stb_image.h
	$(cc) -c -o $@ $< $(include_directory) $(libs)

# Clean

clean :
	rm obj/*.o; rm geod