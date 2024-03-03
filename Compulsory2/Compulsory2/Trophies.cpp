#include "Trophies.h"

Trophies::Trophies()
{
    VBO = 0;
    VAO = 0;
    CBO = 0;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &CBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, CBO);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
}

Trophies::~Trophies()
{
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &CBO);
    glDeleteVertexArrays(1, &VAO);
}

void Trophies::SetPoints(const vector<glm::vec3>& points)
{
    this->points = points;
    // Initialize all trophies as active
    activeTrophies = std::vector<bool>(points.size(), true);

    farger.resize(points.size(), glm::vec3(1.0f, 0.8f, 0.9f));
    //Colors the points 
    glBindBuffer(GL_ARRAY_BUFFER, CBO);
    glBufferData(GL_ARRAY_BUFFER, farger.size() * sizeof(glm::vec3), &farger[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //Render the 8 points
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(glm::vec3), &points[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Trophies::DrawTrophies()
{
    glBindVertexArray(VAO);
    for (size_t i = 0; i < points.size(); ++i) {
        if (activeTrophies[i]) {
            // Draw the trophy at points[i]
            // Implement your drawing logic here
            glPointSize(40.0f);
            glDrawArrays(GL_POINTS, i, 1);
        }
    }
    glBindVertexArray(0);
}

void Trophies::RemoveTrophy(const glm::vec3& position)
{
    // Find the index of the trophy with the specified position
    auto it = std::find(points.begin(), points.end(), position);
    if (it != points.end()) {
        // Calculate the index of the trophy
        size_t index = std::distance(points.begin(), it);
        // Deactivate the trophy
        activeTrophies[index] = false;
    }
}