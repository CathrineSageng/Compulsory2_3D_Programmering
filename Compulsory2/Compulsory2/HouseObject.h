#ifndef HOUSEOBJECT_H
#define HOUSEOBJECT_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include<iostream>
#include <vector>


class HouseObject
{
public:
    HouseObject(float radius, int sectors, int stacks);
    void DrawHouseObject();

private:
    GLuint VAO, VBO, EBO;
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;
};

#endif
