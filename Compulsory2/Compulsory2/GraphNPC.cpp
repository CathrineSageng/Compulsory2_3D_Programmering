#include "GraphNPC.h"

Graph::Graph()
{
    VBO = 0;
    VAO = 0;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

}

Graph::~Graph()
{
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}

std::vector<glm::vec3> Graph::calculateGraphCoordinates() {
    // Control points for the graph
    std::vector<glm::vec3> controlPoints = {
        glm::vec3(-1, 0, -1), glm::vec3(-2, 0, -8),
        glm::vec3(-6, 0, -3), glm::vec3(-8, 0, -1)
    };

    // Vector to store the calculated graph coordinates
    std::vector<glm::vec3> coords;

    // Perform cubic Hermite interpolation between control points
    for (size_t i = 0; i < controlPoints.size() - 1; ++i) {
        glm::vec3 p0 = controlPoints[i];
        glm::vec3 p1 = controlPoints[i + 1];

        glm::vec3 m0 = i == 0 ? glm::normalize(p1 - p0) : 0.5f * (p1 - controlPoints[i - 1]);
        glm::vec3 m1 = i == controlPoints.size() - 2 ? glm::normalize(p1 - p0) : 0.5f * (controlPoints[i + 2] - p0);

        for (float t = 0.0f; t <= 1.0f; t += 0.01f) {
            float h00 = 2.0f * t * t * t - 3.0f * t * t + 1.0f;
            float h10 = t * t * t - 2.0f * t * t + t;
            float h01 = -2.0f * t * t * t + 3.0f * t * t;
            float h11 = t * t * t - t * t;

            glm::vec3 point = h00 * p0 + h10 * m0 + h01 * p1 + h11 * m1;
            coords.push_back(point);
        }
    }

    return coords;
}

void Graph::generateGraph() {

    graphCoords = calculateGraphCoordinates();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, graphCoords.size() * sizeof(glm::vec3), graphCoords.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Graph::drawGraph() {
    // Draw the graph
    glBindVertexArray(VAO);

    // Set line thickness
    glLineWidth(3.0f);

    glDrawArrays(GL_LINE_STRIP, 0, graphCoords.size()); 
    glBindVertexArray(0);
}

void Graph::renderScene()
{
    drawGraph();
}

int Graph::getNumPoints()
{
    return graphCoords.size();
}

glm::vec3 Graph::getGraphPoint(int index)
{
    if (index >= 0 && index < graphCoords.size()) {
        return graphCoords[index];
    }
    // Return a default value if index is out of range
    return glm::vec3(0.0f);
}

