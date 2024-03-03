#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<vector>

#include"ShaderClass.h"
#include"Character.h"
#include "Ground.h"
#include "Trophies.h"
#include "NPC.h"
#include "GraphNPC.h"

using namespace std;

const GLuint WIDTH = 1200, HEIGHT = 1000;
const GLfloat cameraSpeed = 0.001f;
glm::vec3 cubePosition = glm::vec3(0.0f, 0.5f, 0.0f); // Initial position of the cube
const float groundSize = 10.0f; // Half of the ground's size in each direction

//The points for the trophies
vector<glm::vec3> points = 
{
    glm::vec3(1, 0.1,  2), glm::vec3(3, 0.1, 1), glm::vec3(2, 0.1, 4), glm::vec3(5, 0.1, 3),
    glm::vec3(4, 0.1, 5), glm::vec3(6, 0.1, 5), glm::vec3(6, 0.1, 7), glm::vec3(8, 0.1, 4)
};

// Define bounding sphere for character
glm::vec3 characterCenter = cubePosition;
float characterRadius = 0.5f; // Adjust as needed

// Calculate distance between two points
float distance(glm::vec3 p1, glm::vec3 p2) {
    return glm::length(p1 - p2);
}

// Check collision between character and trophy
bool checkCollision(glm::vec3 trophyCenter, float trophyRadius)
{
    float dist = distance(characterCenter, trophyCenter);
    return dist <= (characterRadius + trophyRadius);
}

vector<glm::vec3> punkter = { glm::vec3(-1, 0, -1), glm::vec3(-2, 0, -8), glm::vec3(-6, 0, -3), glm::vec3(-8, 0, -1) };

// Global variables to control NPC movement
float npcSpeed = 0.1f; // Adjust speed as needed
float npcTime = 0.0f; // Timer for NPC movement
bool npcForward = true; // Flag to indicate the direction of NPC movement

void updateNPCPosition(const Shader& shaderProgram, GLfloat deltaTime, Graph& graph, float& npcTime, bool& npcForward) {
    // Update NPC time and direction
    if (npcForward) {
        npcTime += npcSpeed * deltaTime;
        if (npcTime >= 1.0f) {
            npcTime = 1.0f;
            npcForward = false; // Reverse direction when reaching the end
        }
    }
    else {
        npcTime -= npcSpeed * deltaTime;
        if (npcTime <= 0.0f) {
            npcTime = 0.0f;
            npcForward = true; // Reverse direction when reaching the start
        }
    }

    // Calculate NPC position along the graph based on time
    int numPoints = graph.getNumPoints();
    float numPointsFloat = static_cast<float>(numPoints);
    float indexFloat = npcTime * numPointsFloat;
    int currentIndex = static_cast<int>(indexFloat);

    // Ensure currentIndex stays within valid range
    currentIndex = glm::clamp(currentIndex, 0, numPoints - 1);

    glm::vec3 npcPosition = graph.getGraphPoint(currentIndex);

    // Update model matrix for NPC
    glm::mat4 modelNPC = glm::translate(glm::mat4(1.0f), npcPosition);

    // Pass model matrix to shader
    GLint modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelNPC));
}

// Camera settings
float angle = glm::radians(20.0f); // Convert 20 degrees to radians
float distanceFromScene = 5.0f; // Distance from the scene
float yOffset = distanceFromScene * sin(angle); // Calculate the y offset
float zOffset = -distanceFromScene * cos(angle); // Calculate the z offset

glm::vec3 cameraPos = glm::vec3(0.0f, yOffset, zOffset); // Set the new camera position
glm::vec3 cameraFront = glm::normalize(glm::vec3(0.0f, -yOffset, -zOffset)); // Adjust camera front to look at the scene
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
//glm::vec3 cameraFrontt = glm::vec3(0.0f, 0.0f, -1.0f);
GLfloat yaw = -90.0f;
GLfloat pitch = 0.0f;
GLfloat lastX = WIDTH / 2.0f;
GLfloat lastY = HEIGHT / 2.0f;
bool firstMouse = true;
bool keys[1024];


 //Mouse movement callback function
//void mouse_callback(GLFWwindow* window, double xpos, double ypos) 
//{
//    if (firstMouse)
//    {
//        lastX = xpos;
//        lastY = ypos;
//        firstMouse = false;
//    }
//
//    GLfloat xoffset = xpos - lastX;
//    GLfloat yoffset = lastY - ypos;
//    lastX = xpos;
//    lastY = ypos;
//
//    GLfloat sensitivity = 0.05f;
//    xoffset *= sensitivity;
//    yoffset *= sensitivity;
//
//    yaw += xoffset;
//    pitch += yoffset;
//
//    if (pitch > 89.0f) pitch = 89.0f;
//    if (pitch < -89.0f) pitch = -89.0f;
//
//    glm::vec3 front;
//    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
//    front.y = sin(glm::radians(pitch));
//    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
//    cameraFrontt = glm::normalize(front);
//
//    // Print out the mouse position for debugging
//    std::cout << "Mouse Position: (" << xpos << ", " << ypos << ")" << std::endl;
//}

// Keyboard input callback function
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) 
{
    GLfloat cameraSpeed = 0.05f;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key >= 0 && key < 1024) 
    {
        if (action == GLFW_PRESS)
            keys[key] = true;
        else if (action == GLFW_RELEASE)
            keys[key] = false;
    }

    // Calculate the movement direction in the horizontal plane
    glm::vec3 movementDirection = glm::normalize(glm::vec3(cameraFront.x, 0.0f, cameraFront.z));

    // Handle keyboard input for character movement
    if (action == GLFW_PRESS)
    {
        if (key == GLFW_KEY_W)
            cubePosition += cameraSpeed * movementDirection;
        else if (key == GLFW_KEY_S)
            cubePosition -= cameraSpeed * movementDirection;
        else if (key == GLFW_KEY_A)
            cubePosition -= glm::normalize(glm::cross(movementDirection, cameraUp)) * cameraSpeed;
        else if (key == GLFW_KEY_D)
            cubePosition += glm::normalize(glm::cross(movementDirection, cameraUp)) * cameraSpeed;
    }

    // Handle 'm' key for toggling movement direction of modelNPC
    if (key == GLFW_KEY_M && action == GLFW_PRESS) {
        npcForward = !npcForward; // Toggle the movement direction
    }
}

int main() 
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL Cube with Camera", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetKeyCallback(window, key_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
    {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }

    glViewport(0, 0, WIDTH, HEIGHT);

    //Generates Shader object using shaders defualt.vert and default.frag
    Shader shaderProgram("default.vert", "default.frag");

    //Creates a character object
    Character character1;

    //Creates a ground object
    Ground ground1;

    //Creates trophies objects
    Trophies trophies1;
    trophies1.SetPoints(points);

    Graph graph;
    graph.generateGraph();

    NPC NPC;

    const float trophyRadius = 0.5f; // Adjust the value as needed

    // Define bounding spheres for trophies (assuming the trophy positions are stored in the `points` vector)
    std::vector<std::pair<glm::vec3, float>> trophyBoundingSpheres;
    for (const auto& trophy : points)
    {
        trophyBoundingSpheres.push_back({ trophy, trophyRadius }); // Assuming trophyRadius is defined elsewhere
    }

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
   

    // Projection matrix
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);

    // Game loop
    while (!glfwWindowShouldClose(window)) 
    {
        glfwPollEvents();

        GLfloat deltaTime = glfwGetTime();
        glfwSetTime(0.0f);

        // Handle keyboard input for camera movement
        if (keys[GLFW_KEY_UP])
            cameraPos += cameraSpeed * cameraFront;
        if (keys[GLFW_KEY_DOWN])
            cameraPos -= cameraSpeed * cameraFront;
        if (keys[GLFW_KEY_LEFT])
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        if (keys[GLFW_KEY_RIGHT])
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;


        // Calculate the movement direction in the horizontal plane
        glm::vec3 movementDirection = glm::normalize(glm::vec3(cameraFront.x, 0.0f, cameraFront.z));

        glm::vec3 newPosition = cubePosition;

        // Handle keyboard input for character movement
        if (keys[GLFW_KEY_W])
            newPosition += cameraSpeed * movementDirection;
        if (keys[GLFW_KEY_A])
            newPosition -= glm::normalize(glm::cross(movementDirection, cameraUp)) * cameraSpeed;
        if (keys[GLFW_KEY_S])
            newPosition -= cameraSpeed * movementDirection;
        if (keys[GLFW_KEY_D])
            newPosition += glm::normalize(glm::cross(movementDirection, cameraUp)) * cameraSpeed;

        // Check boundaries
        if (newPosition.x < -groundSize) newPosition.x = -groundSize;
        if (newPosition.x > groundSize) newPosition.x = groundSize;
        if (newPosition.z < -groundSize) newPosition.z = -groundSize;
        if (newPosition.z > groundSize) newPosition.z = groundSize;

        // Update character position
        cubePosition = newPosition;

        // Update model matrix for character
        glm::mat4 modelCharacter;
        modelCharacter = glm::translate(glm::mat4(1.0f), cubePosition);

        // Update model matrix for ground, so the ground is not moving
        glm::mat4 modelGround = glm::mat4(1.0f);

        // Clear the color and depth buffers
        glClearColor(0.196078f, 0.196078f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use shader program
        shaderProgram.Activate();

        // View matrix
        glm::mat4 view;
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        // Pass transformation matrices to shader
        GLint modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
        GLint viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
        GLint projLoc = glGetUniformLocation(shaderProgram.ID, "projection");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        glm::mat4 model = glm::translate(glm::mat4(1.0f), cubePosition);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCharacter));

        character1.DrawCharacter();

        // Pass transformation matrices to shader for ground
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelGround));

        ground1.DrawGround();

        trophies1.DrawTrophies();

        graph.renderScene();
        updateNPCPosition(shaderProgram, deltaTime, graph, npcTime, npcForward);

        NPC.DrawNPC();

        characterCenter = cubePosition;

        // Collision detection between character and trophies
        for (const auto& trophySphere : trophyBoundingSpheres) {
            glm::vec3 trophyCenter = trophySphere.first;
            float trophyRadius = trophySphere.second;
            if (checkCollision(trophyCenter, trophyRadius)) {
                trophies1.RemoveTrophy(trophyCenter);
            }
        }

        // Swap the screen buffers
        glfwSwapBuffers(window);

    }

    // Clean up
    glfwTerminate();

    return 0;
}