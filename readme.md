Headshot - Prototype

A testing ground for C++ development, using meson as the build system, opengl, glew, glfw and imgui.

There has been a plan to include vulkan, But for now we will focus on OpenGL and refactor the engine and abstract it for vulkan support at a later date.

External dists:
    - Engine
    - Editor
    - Game

Internal dists:
    - Model/Texture Editor standalone
    - Level Editor Layer with dev engine
    - Multiplayer + Command & Control Center
    - Data Analysis(TBD (idea only))


Build Prototype project

- 'meson setup --cross-file cross/windows.txt --wipe build'
- 'meson compile -C build'
- 'meson install -C build'

Alternatively, Run the Script file per OS