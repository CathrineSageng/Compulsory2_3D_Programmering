#include "Door.h"

Door::Door()
{
    VBO = 0;
    VAO = 0;

    GLfloat vertices[] = {
        //Door                          //Color black
         0.663f, 0.0f, -2.00f,        0.0f, 0.0f, 0.0f,
         -0.667f, 0.0f, -2.00f,       0.0f, 0.0f, 0.0f,
         -0.667f, 1.5f, -2.00f,       0.0f, 0.0f, 0.0f,
         -0.667f, 1.5f, -2.00f,       0.0f, 0.0f, 0.0f,
         0.663f, 1.5f, -2.00f,        0.0f, 0.0f, 0.0f,
         0.663f, 0.0f, -2.00f,        0.0f, 0.0f, 0.0f,
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

Door::~Door()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Door::DrawDoor()
{
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}