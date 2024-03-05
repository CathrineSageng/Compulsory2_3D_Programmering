#ifndef GRAPHNPC_H
#define GRAPHNPC_H

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>

class Graph {
public:
    Graph();
    ~Graph();

    void generateGraph(); 
    void drawGraph();     
    void renderScene(); 
    // Get the number of points in the graph
    int getNumPoints();   
    // Get a point from the graph
    glm::vec3 getGraphPoint(int index); 

private:
    std::vector<glm::vec3> calculateGraphCoordinates();

    GLuint VAO, VBO; 

    // Vector to store graph coordinates
    std::vector<glm::vec3> graphCoords; 
};


#endif // GRAPH_H
