#include "includes/render/vao.h"

RectVAO::RectVAO() {
   GLfloat vertices[] = {
    // Positions         // uv
    -1.0f,  1.0f, 0.0f,   0.0f, 1.0f, // left top
    -1.0f, -1.0f, 0.0f,   0.0f, 0.0f, // left bottom
     1.0f, -1.0f, 0.0f,   1.0f, 0.0f, // right bottom

    -1.0f,  1.0f, 0.0f,   0.0f, 1.0f, // left top
     1.0f, -1.0f, 0.0f,   1.0f, 0.0f, // right bottom
     1.0f,  1.0f, 0.0f,   1.0f, 1.0f  // right top
}; 
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
   glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1); 
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void RectVAO::Delete() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

}
