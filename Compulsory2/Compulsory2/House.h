#ifndef HOUSE_H
#define HOUSE_H

#include <glad/glad.h>
#include <glm/glm.hpp>


class House
{
public:
    House();
    ~House();
    void DrawHouse();

private:
    GLuint VAO;
    GLuint VBO;

};

#endif
