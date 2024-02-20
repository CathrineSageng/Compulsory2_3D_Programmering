#include "Character.h"

#include <iostream>

Character::Character()
{
    // Define vertices of the cube
    GLfloat vertices[] = {
        // Positions
         -0.25f, -0.5f, -0.25f,        1.0f, 0.0f, 0.0f,
    0.25f, -0.5f, -0.25f,         1.0f, 0.0f, 0.0f,
    0.25f, 0.5f, -0.25f,          1.0f, 0.0f, 0.0f,
    0.25f, 0.5f, -0.25f,          1.0f, 0.0f, 0.0f,
    -0.25f, 0.5f, -0.25f,         1.0f, 0.0f, 0.0f,
    -0.25f, -0.5f, -0.25f,        1.0f, 0.0f, 0.0f,

    -0.25f, -0.5f, 0.25f,         1.0f, 0.0f, 0.0f,
    0.25f, -0.5f, 0.25f,          1.0f, 0.0f, 0.0f,
    0.25f, 0.5f, 0.25f,           1.0f, 0.0f, 0.0f,
    0.25f, 0.5f, 0.25f,           1.0f, 0.0f, 0.0f,
    -0.25f, 0.5f, 0.25f,          1.0f, 0.0f, 0.0f,
    -0.25f, -0.5f, 0.25f,         1.0f, 0.0f, 0.0f,

    -0.25f, 0.5f, 0.25f,          1.0f, 0.0f, 0.0f,
    -0.25f, 0.5f, -0.25f,         1.0f, 0.0f, 0.0f,
    -0.25f, -0.5f, -0.25f,        1.0f, 0.0f, 0.0f,
    -0.25f, -0.5f, -0.25f,        1.0f, 0.0f, 0.0f,
    -0.25f, -0.5f, 0.25f,         1.0f, 0.0f, 0.0f,
    -0.25f, 0.5f, 0.25f,          1.0f, 0.0f, 0.0f,

    0.25f, 0.5f, 0.25f,           1.0f, 0.0f, 0.0f,
    0.25f, 0.5f, -0.25f,          1.0f, 0.0f, 0.0f,
    0.25f, -0.5f, -0.25f,         1.0f, 0.0f, 0.0f,
    0.25f, -0.5f, -0.25f,         1.0f, 0.0f, 0.0f,
    0.25f, -0.5f, 0.25f,          1.0f, 0.0f, 0.0f,
    0.25f, 0.5f, 0.25f,           1.0f, 0.0f, 0.0f,

    -0.25f, -0.5f, -0.25f,        1.0f, 0.0f, 0.0f,
    0.25f, -0.5f, -0.25f,         1.0f, 0.0f, 0.0f,
    0.25f, -0.5f, 0.25f,          1.0f, 0.0f, 0.0f,
    0.25f, -0.5f, 0.25f,          1.0f, 0.0f, 0.0f,
    -0.25f, -0.5f, 0.25f,         1.0f, 0.0f, 0.0f,
    -0.25f, -0.5f, -0.25f,        1.0f, 0.0f, 0.0f,

    -0.25f, 0.5f, -0.25f,         1.0f, 0.0f, 0.0f,
    0.25f, 0.5f, -0.25f,          1.0f, 0.0f, 0.0f,
    0.25f, 0.5f, 0.25f,           1.0f, 0.0f, 0.0f,
    0.25f, 0.5f, 0.25f,           1.0f, 0.0f, 0.0f,
    -0.25f, 0.5f, 0.25f,          1.0f, 0.0f, 0.0f,
    -0.25f, 0.5f, -0.25f,         1.0f, 0.0f, 0.0f,
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

Character::~Character()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
   /* glDeleteBuffers(1, &EBO);*/
}

void Character::DrawCharacter()
{
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}
