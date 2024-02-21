#version 330 core

layout (location = 0) in vec3 aPos; // The position variable has attribute position 0
layout (location = 1) in vec2 aTexCoord; // The texture coordinate variable has attribute position 1

out vec2 TexCoord; // Output texture coordinate to fragment shader

uniform mat4 model; // Model matrix
uniform mat4 view; // View matrix
uniform mat4 projection; // Projection matrix

void main()
{
    // Transform vertex positions from model space to clip space
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    // Pass through texture coordinates
    TexCoord = aTexCoord;
}
