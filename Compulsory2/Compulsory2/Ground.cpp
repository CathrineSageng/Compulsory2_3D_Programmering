#include "Ground.h"

Ground::Ground()
{
    VBO = 0;
    VAO = 0;
    EBO = 0;

    // Vertices of the floor
    GLfloat groundVertices[] = {
        // Positions           // Colors
        -10.0f, -0.5f, -10.0f,    0.5f, 0.5f, 0.5f,
        10.0f, -0.5f, -10.0f,     0.5f, 0.5f, 0.5f,
        10.0f, -0.5f, 10.0f,      0.5f, 0.5f, 0.5f,
        10.0f, -0.5f, 10.0f,      0.5f, 0.5f, 0.5f,
        -10.0f, -0.5f, 10.0f,     0.5f, 0.5f, 0.5f,
        -10.0f, -0.5f, -10.0f,    0.5f, 0.5f, 0.5f
    };

    GLuint groundIndices[] = {
    0, 1, 2,
    2, 3, 0
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(groundVertices), groundVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(groundIndices), groundIndices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

Ground::~Ground()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Ground::DrawGround()
{
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
