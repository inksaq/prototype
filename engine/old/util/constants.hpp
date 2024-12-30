//
// Created by Administrator on 5/19/2024.
//

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>
#include <glad/gl.h>
#include <glm/glm.hpp>



// Log levels
#define INFO 0
#define WARN 1
#define ERROR 2
#define FATAL 3
#define NO_LOG 4

//Colors
#define BLACK 0
#define DARK_RED 4
#define DARK_GREEN 2
#define DARK_YELLOW 6
#define DARK_BLUE 1
#define DARK_MAGENTA 5
#define DARK_CYAN 3
#define DARK_WHITE 7
#define BRIGHT_BLACK 8
#define BRIGHT_RED 12
#define BRIGHT_GREEN 10
#define BRIGHT_YELLOW 14
#define BRIGHT_BLUE 9
#define BRIGHT_MAGENTA 13
#define BRIGHT_CYAN 11
#define WHITE 15

struct Vertex {
    glm::vec3 position = glm::vec3(0);
    float pad0 = 0;
    glm::vec3 normal = glm::vec3(0);
    float pad1 = 0;
    glm::vec2 uv = glm::vec2(0);
    float pad2 = 0;
    float pad3 = 0;
    glm::vec3 tangent = glm::vec3(0);
    float pad4 = 0;
    glm::vec3 bitangent = glm::vec3(0);
    float pad5 = 0;
    glm::vec4 weight = glm::vec4(0);
    glm::ivec4 boneID = glm::ivec4(0);

    bool operator==(const Vertex& other) const {
        return position == other.position && normal == other.normal && uv == other.uv;
    }
};

struct UIRenderInfo {
    std::string textureName;
    int screenX = 0;
    int screenY = 0;
    glm::vec3 color = glm::vec3(1,1,1);
    bool centered = false;
    GLuint target = GL_TEXTURE_2D;
    void* parent = nullptr;
};



#endif //CONSTANTS_H
