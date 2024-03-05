#include "House.h"

House::House()
{
    VBO = 0;
    VAO = 0;

    GLfloat vertices[] = {
        // Positions                //Colors
        //Front Top
        -2.00f, 1.5f, -2.00f,        1.0f, 0.0f, 0.0f,
        2.00f, 1.5f, -2.00f,         1.0f, 0.0f, 0.0f,
        2.00f, 3.0f, -2.00f,         1.0f, 0.0f, 0.0f,
        2.00f, 3.0f, -2.00f,         1.0f, 0.0f, 0.0f,
        -2.00f, 3.0f, -2.00f,        1.0f, 0.0f, 0.0f,
        -2.00f, 1.5f, -2.00f,        1.0f, 0.0f, 0.0f,

        //Front down right
        -2.00f, 0.0f, -2.00f,        1.0f, 0.0f, 0.0f,
        -0.667f, 0.0f, -2.00f,       1.0f, 0.0f, 0.0f,
        -0.677f, 1.5f, -2.00f,       1.0f, 0.0f, 0.0f,
        -0.667f, 1.5f, -2.00f,       1.0f, 0.0f, 0.0f,
        -2.00f, 1.5f, -2.00f,        1.0f, 0.0f, 0.0f,
        -2.00f, 0.0f, -2.00f,        1.0f, 0.0f, 0.0f,

        //Door front
        0.663f, 0.0f, -2.00f,        0.64f, 0.16f, 0.16f,
        -0.667f, 0.0f, -2.00f,       0.64f, 0.16f, 0.16f,
        -0.667f, 1.5f, -2.00f,       0.64f, 0.16f, 0.16f,
        -0.667f, 1.5f, -2.00f,       0.64f, 0.16f, 0.16f,
        0.663f, 1.5f, -2.00f,        0.64f, 0.16f, 0.16f,
        0.663f, 0.0f, -2.00f,        0.64f, 0.16f, 0.16f,

        //Front down left
        0.663f, 0.0f, -2.00f,        1.0f, 0.0f, 0.0f,
        2.00f, 0.0f, -2.00f,         1.0f, 0.0f, 0.0f,
        2.00f, 1.5f, -2.00f,         1.0f, 0.0f, 0.0f,
        2.00f, 1.5f, -2.00f,         1.0f, 0.0f, 0.0f,
        0.663f, 1.5f, -2.00f,        1.0f, 0.0f, 0.0f,
        0.663f, 0.0f, -2.00f,        1.0f, 0.0f, 0.0f,

        //Back
        -2.00f, 0.0f, 2.0f,         1.0f, 0.0f, 0.0f,
        2.00f, 0.0f, 2.00f,         1.0f, 0.0f, 0.0f,
        2.00f, 3.0f, 2.00f,         1.0f, 0.0f, 0.0f,
        2.00f, 3.0f, 2.00f,         1.0f, 0.0f, 0.0f,
        -2.00f, 3.0f, 2.00f,        1.0f, 0.0f, 0.0f,
        -2.00f, 0.0f, 2.00f,        1.0f, 0.0f, 0.0f,

        //Left side
        -2.00f, 3.0f, 2.00f,         1.0f, 0.0f, 0.0f,
        -2.00f, 3.0f, -2.00f,        1.0f, 0.0f, 0.0f,
        -2.00f, 0.0f, -2.00f,        1.0f, 0.0f, 0.0f,
        -2.00f, 0.0f, -2.00f,        1.0f, 0.0f, 0.0f,
        -2.00f, 0.0f, 2.00f,         1.0f, 0.0f, 0.0f,
        -2.00f, 3.0f, 2.00f,         1.0f, 0.0f, 0.0f,

        //Right side
        2.00f, 3.0f, 2.00f,           1.0f, 0.0f, 0.0f,
        2.00f, 3.0f, -2.00f,          1.0f, 0.0f, 0.0f,
        2.00f, 0.0f, -2.00f,          1.0f, 0.0f, 0.0f,
        2.00f, 0.0f, -2.00f,          1.0f, 0.0f, 0.0f,
        2.00f, 0.0f, 2.00f,           1.0f, 0.0f, 0.0f,
        2.00f, 3.0f, 2.00f,           1.0f, 0.0f, 0.0f,

        //Bottom side
        -2.00f, 0.0f, -2.00f,        0.647059f, 0.164706f, 0.164706f,
        2.00f, 0.0f, -2.00f,         0.647059f, 0.164706f, 0.164706f,
        2.00f, 0.0f, 2.00f,          0.647059f, 0.164706f, 0.164706f,
        2.00f, 0.0f, 2.00f,          0.647059f, 0.164706f, 0.164706f,
        -2.00f, 0.0f, 2.00f,         0.647059f, 0.164706f, 0.164706f,
        -2.00f, 0.0f, -2.00f,        0.647059f, 0.164706f, 0.164706f,

        //Top side
        -2.00f, 3.0f, -2.00f,         0.0f, 0.0f, 0.0f,
        2.00f, 3.0f, -2.00f,          0.0f, 0.0f, 0.0f,
        2.00f, 3.0f, 2.00f,           0.0f, 0.0f, 0.0f,
        2.00f, 3.0f, 2.00f,           0.0f, 0.0f, 0.0f,
        -2.00f, 3.0f, 2.00f,          0.0f, 0.0f, 0.0f,
        -2.00f, 3.0f, -2.00f,         0.0f, 0.0f, 0.0f,
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

House::~House()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void House::DrawHouse()
{
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 54);
    glBindVertexArray(0);
    /*glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);*/
}
