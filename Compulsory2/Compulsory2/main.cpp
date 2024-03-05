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
#include "House.h"
#include "Door.h"
#include "HouseObject.h"

using namespace std;

const GLuint WIDTH = 1200, HEIGHT = 1000;
const GLfloat cameraSpeed = 0.001f;
//Position of the cube
glm::vec3 cubePosition = glm::vec3(0.0f, 0.5f, 0.0f);
const float groundSize = 10.0f; 

//The points for the trophies
vector<glm::vec3> points = 
{
    glm::vec3(1, 0.1,  2), glm::vec3(3, 0.1, 1), glm::vec3(2, 0.1, 4), glm::vec3(5, 0.1, 3),
    glm::vec3(4, 0.1, 5), glm::vec3(6, 0.1, 5), glm::vec3(6, 0.1, 7), glm::vec3(8, 0.1, 4)
};

// Define bounding sphere for character
glm::vec3 characterCenter = cubePosition;
float characterRadius = 0.5f;

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
float npcSpeed = 0.1f; 
float npcTime = 0.0f; // Timer for NPC movement
bool npcForward = true; 

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

bool drawDoor = true;

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

   //Open the door to the house 
    if (key == GLFW_KEY_O && action == GLFW_PRESS) {
        drawDoor = !drawDoor;
    }

    // Handle the movement to the NPC
    if (key == GLFW_KEY_M && action == GLFW_PRESS) {
        npcForward = !npcForward;
    }
}

// Check collision between character and house
bool checkHouseCollision(glm::vec3 characterPosition) {
    // Define the bounding box of the house
    glm::vec3 houseMinBounds = glm::vec3(-5.5f, 0.0f, 6.5f);
    glm::vec3 houseMaxBounds = glm::vec3(-3.0f, 3.0f, 5.0f);

    // Expand the bounding box
    float expansionX = 1.3f; 
    float expansionZ = 3.1f; 
    houseMinBounds -= glm::vec3(expansionX, 0.0f, expansionZ);
    houseMaxBounds += glm::vec3(expansionX, 0.0f, expansionZ);

    // Check if the character's position is within the expanded house boundaries
    if (characterPosition.x >= houseMinBounds.x && characterPosition.x <= houseMaxBounds.x &&
        characterPosition.y >= houseMinBounds.y && characterPosition.y <= houseMaxBounds.y &&
        characterPosition.z >= houseMinBounds.z && characterPosition.z <= houseMaxBounds.z) {
        // Character's position is inside the expanded house boundaries
        return true;
    }
    return false;
}

// Update character position with collision detection
void updateCharacterPosition(glm::vec3& characterPosition, glm::vec3 newPosition)
{
    // Check collision with the house
    if (!checkHouseCollision(newPosition)) {
        // If there is no collision, update character position
        characterPosition = newPosition;
    }
}

int main() 
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Game", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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

    //Creates the graph to the NPC
    Graph graph;
    graph.generateGraph();

    //NPC
    NPC NPC;

    //House
    House house; 

    Door door;

    HouseObject sphere(0.5f, 30, 30);

    const float trophyRadius = 0.5f; 

    // Define bounding spheres for trophies
    std::vector<std::pair<glm::vec3, float>> trophyBoundingSpheres;
    for (const auto& trophy : points)
    {
        trophyBoundingSpheres.push_back({ trophy, trophyRadius }); 
    }

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
   

    // Projection matrix
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);

    // Game loop
    while (!glfwWindowShouldClose(window)) 
    {
        glfwPollEvents();

        bool insideHouse = false; // Flag to indicate whether the character is inside the house

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

        // Update character position with collision detection
        updateCharacterPosition(cubePosition, newPosition);

        // Update model matrix for character
        glm::mat4 modelCharacter;
        modelCharacter = glm::translate(glm::mat4(1.0f), cubePosition);

        // Update model matrix for ground, so the ground is not moving
        glm::mat4 modelGround = glm::mat4(1.0f);

        // Translate the model matrix of the house to the desired position
        glm::mat4 modelHouse = glm::translate(glm::mat4(1.0f), glm::vec3(-4.0f, 0.0f, 6.0f));

        // Translate the model matrix of the house to the desired position
        glm::mat4 modelSphere = glm::translate(glm::mat4(1.0f), glm::vec3(-4.0f, 0.5f, 6.0f));

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

        // Pass transformation matrices to shader for house
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelHouse));
        house.DrawHouse();

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelHouse));

        if (drawDoor) {
            door.DrawDoor();
        }

        // Pass transformation matrices to shader for ground
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelSphere));
        sphere.DrawHouseObject();

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

        // Check if character enters the house
        if (checkHouseCollision(cubePosition))
        {
            insideHouse = true;
            // Adjust camera position for inside the house
            cameraPos = glm::vec3(-4.0f, 1.0f, 4.0f); // Adjust this position as needed
        }
        else
        {
            // Character is outside the house by default
            insideHouse = false;
        }

        // Swap the screen buffers
        glfwSwapBuffers(window);

    }

    // Clean up
    glfwTerminate();

    return 0;
}