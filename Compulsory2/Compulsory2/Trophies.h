#ifndef TROPHIES_H
#define TROPHIES_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
using namespace std;

class Trophies
{
public:
    Trophies();
    ~Trophies();
    void SetPoints(const vector<glm::vec3>& newPoints);
    void DrawTrophies();

private:
    GLuint VAO;
    GLuint VBO;
    GLuint CBO;
    vector<glm::vec3> points;
    vector<glm::vec3> farger;
};

#endif
