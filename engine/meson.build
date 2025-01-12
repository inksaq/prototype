subdir('includes')
subdir('src')


dependencies = [
    fmt_dep,
    assimp_dep,
    threads_dep, 
    gl_dep,
    glm_dep,
    glfw_dep,
    gleq_dep,
    glew_dep,
    imgui_dep,
    stb_dep,
    entt_dep,
    nlohmann_json,
    clay_dep
]

lib_inc = include_directories('.','src','includes')
dep_inc = include_directories('../')

sources = engine_includes + engine_sources

engine_lib = library(
    'core-framework', 
    dependencies :dependencies,
    include_directories: lib_inc,
    sources : sources
)

engine_dep = declare_dependency(
    link_with: engine_lib, 
    include_directories: dep_inc,
    dependencies: dependencies
)
