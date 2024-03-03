#ifndef NPC_H
#define NPC_H

#include <glad/glad.h>
#include <glm/glm.hpp>


class NPC
{
public:
    NPC();
    ~NPC();
    void DrawNPC();

private:
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
};



#endif
