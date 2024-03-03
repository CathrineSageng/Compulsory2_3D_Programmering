#ifndef GRAPHNPC_H
#define GRAPHNPC_H

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>

class Graph {
public:
    Graph();
    ~Graph();

    void generateGraph(); // Generate the graph
    void drawGraph();     // Draw the graph
    void renderScene();   // Render the scene
    int getNumPoints();   // Get the number of points in the graph
    glm::vec3 getGraphPoint(int index); // Get a point from the graph

private:
    std::vector<glm::vec3> calculateGraphCoordinates();

    GLuint VAO, VBO; // VAO and VBO for the graph

    std::vector<glm::vec3> graphCoords; // Vector to store graph coordinates
};


#endif // GRAPH_H
