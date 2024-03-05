#ifndef DOOR_H
#define DOOR_H

#include <glad/glad.h>
#include <glm/glm.hpp>


class Door
{
public:
    Door();
    ~Door();
    void DrawDoor();

private:
    GLuint VAO;
    GLuint VBO;

};

#endif
