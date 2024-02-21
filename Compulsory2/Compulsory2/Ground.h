#ifndef GROUND_H
#define GROUND_H

#include <glad/glad.h>
#include <glm/glm.hpp>

class Ground
{
public:
	Ground();
	~Ground();

	void DrawGround();

private:
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;
};
#endif
