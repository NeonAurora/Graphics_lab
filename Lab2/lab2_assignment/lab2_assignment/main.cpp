//
//  main.cpp
//  3D Object Drawing with Axes and Camera Movement
//
//  Created by Nazirul Hasan on 4/9/23.
//  modified by Badiuzzaman on 3/11/24.
//  Modified by Assistant to add dynamic camera movement, rotation, and 3D Axis Drawing on 11/19/24.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "basic_camera.h"

#include <iostream>
#include <vector>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// draw object functions
void drawCube(Shader shaderProgram, unsigned int VAO, glm::mat4 parentTrans,
    float posX = 0.0f, float posY = 0.0f, float posZ = 0.0f,
    float rotX = 0.0f, float rotY = 0.0f, float rotZ = 0.0f,
    float scX = 1.0f, float scY = 1.0f, float scZ = 1.0f,
    glm::vec4 color = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f)); // Default color is green



// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// modelling transform
float rotateAngle_X = 0.0;
float rotateAngle_Y = 0.0;
float rotateAngle_Z = 0.0;
float translate_X = 0.0;
float translate_Y = 0.0;
float translate_Z = 0.0;
float scale_X = 1.0;
float scale_Y = 1.0;
float scale_Z = 1.0;

// camera
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

BasicCamera basic_camera(3.0f, 3.0f, 3.0f, 0.0f, 0.0f, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));

// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;
float fanRotateAngle_Y = 0.0f;

int main()
{
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CSE 4208: Computer Graphics Laboratory", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader program
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes for cube
    float cube_vertices[] = {
        // positions          // colors
        0.0f, 0.0f, 0.0f, 0.3f, 0.8f, 0.5f,
        0.5f, 0.0f, 0.0f, 0.5f, 0.4f, 0.3f,
        0.5f, 0.5f, 0.0f, 0.2f, 0.7f, 0.3f,
        0.0f, 0.5f, 0.0f, 0.6f, 0.2f, 0.8f,
        0.0f, 0.0f, 0.5f, 0.8f, 0.3f, 0.6f,
        0.5f, 0.0f, 0.5f, 0.4f, 0.4f, 0.8f,
        0.5f, 0.5f, 0.5f, 0.2f, 0.3f, 0.6f,
        0.0f, 0.5f, 0.5f, 0.7f, 0.5f, 0.4f
    };
    unsigned int cube_indices[] = {
        0, 3, 2,
        2, 1, 0,

        1, 2, 6,
        6, 5, 1,

        5, 6, 7,
        7, 4, 5,

        4, 7, 3,
        3, 0, 4,

        6, 2, 3,
        3, 7, 6,

        1, 5, 4,
        4, 0, 1
    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        processInput(window);

        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // pass projection matrix to shader
        glm::mat4 projection = glm::perspective(glm::radians(basic_camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        ourShader.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = basic_camera.createViewMatrix();
        ourShader.setMat4("view", view);

        // Draw Axes
        ourShader.use();
        glm::mat4 model = glm::mat4(1.0f);
        ourShader.setMat4("model", model);
        glDrawArrays(GL_LINES, 0, 6);
        glBindVertexArray(0);

        glm::mat4 parentTrans = glm::mat4(1.0f);

        // Apply a translation to move the entire table
        parentTrans = glm::translate(parentTrans, glm::vec3(translate_X, translate_Y, translate_Z));

        // Apply rotation around the X-axis
        parentTrans = glm::rotate(parentTrans, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));

        // Apply rotation around the Y-axis
        parentTrans = glm::rotate(parentTrans, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));

        // Apply rotation around the Z-axis
        parentTrans = glm::rotate(parentTrans, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));


        // Draw Cube
        drawCube(ourShader, VAO, parentTrans, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 2.0, 1.0, 1.0, glm::vec4(0.3f, 0.3f, 0.3f, 1.0f));

        // Drawing floor (light gray color)
        drawCube(ourShader, VAO, parentTrans, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 12.0, 0.05, 12.0, glm::vec4(0.8f, 0.8f, 0.8f, 1.0f));

        // Drawing walls (light beige color)
        drawCube(ourShader, VAO, parentTrans, -3.0, 1.5, 0.0f, 0.0f, 0.0f, 0.0f, 0.05, 6.0, 12.0, glm::vec4(0.9f, 0.85f, 0.75f, 1.0f)); // wall
        drawCube(ourShader, VAO, parentTrans, 3.0, 1.5, 0.0f, 0.0f, 0.0f, 0.0f, 0.05, 6.0, 12.0, glm::vec4(0.9f, 0.85f, 0.75f, 1.0f)); // wall
        drawCube(ourShader, VAO, parentTrans, 0.0f, 1.5, -3.0, 0.0f, 0.0f, 0.0f, 12.0, 6.0, 0.05, glm::vec4(0.9f, 0.85f, 0.75f, 1.0f)); // wall
        drawCube(ourShader, VAO, parentTrans, 0.0f, 1.5, 4.0, 0.0f, 0.0f, 0.0f, 12.0, 6.0, 0.05, glm::vec4(0.9f, 0.85f, 0.75f, 1.0f)); // wall

        // Drawing Table (wood-like color for the surface, metallic gray for the legs)
        drawCube(ourShader, VAO, parentTrans, 2.0f, 0.5f, 2.0f, 0.0f, 0.0f, 0.0f, 2.0f, 0.2f, 2.0f, glm::vec4(0.72f, 0.52f, 0.04f, 1.0f)); // wooden surface
        drawCube(ourShader, VAO, parentTrans, 1.6f, 0.25f, 1.6f, 0.0f, 0.0f, 0.0f, 0.2f, 1.0f, 0.2f, glm::vec4(0.6f, 0.6f, 0.6f, 1.0f)); // metallic leg
        drawCube(ourShader, VAO, parentTrans, 2.4f, 0.25f, 2.4f, 0.0f, 0.0f, 0.0f, 0.2f, 1.0f, 0.2f, glm::vec4(0.6f, 0.6f, 0.6f, 1.0f)); // metallic leg
        drawCube(ourShader, VAO, parentTrans, 2.4f, 0.25f, 1.6f, 0.0f, 0.0f, 0.0f, 0.2f, 1.0f, 0.2f, glm::vec4(0.6f, 0.6f, 0.6f, 1.0f)); // metallic leg
        drawCube(ourShader, VAO, parentTrans, 1.6f, 0.25f, 2.4f, 0.0f, 0.0f, 0.0f, 0.2f, 1.0f, 0.2f, glm::vec4(0.6f, 0.6f, 0.6f, 1.0f)); // metallic leg

        // Drawing Chair (dark wood-like color, darker legs, fabric backrest)
        drawCube(ourShader, VAO, parentTrans, 1.5f, 0.25f, 2.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.2f, 1.0f, glm::vec4(0.54f, 0.27f, 0.07f, 1.0f)); // wooden seat
        drawCube(ourShader, VAO, parentTrans, 1.3f, 0.1f, 1.8f, 0.0f, 0.0f, 0.0f, 0.2f, 0.5f, 0.2f, glm::vec4(0.4f, 0.26f, 0.13f, 1.0f)); // darker leg
        drawCube(ourShader, VAO, parentTrans, 1.65f, 0.1f, 2.15f, 0.0f, 0.0f, 0.0f, 0.2f, 0.5f, 0.2f, glm::vec4(0.4f, 0.26f, 0.13f, 1.0f)); // darker leg
        drawCube(ourShader, VAO, parentTrans, 1.65f, 0.1f, 1.8f, 0.0f, 0.0f, 0.0f, 0.2f, 0.5f, 0.2f, glm::vec4(0.4f, 0.26f, 0.13f, 1.0f)); // darker leg
        drawCube(ourShader, VAO, parentTrans, 1.3f, 0.1f, 2.15f, 0.0f, 0.0f, 0.0f, 0.2f, 0.5f, 0.2f, glm::vec4(0.4f, 0.26f, 0.13f, 1.0f)); // darker leg
        drawCube(ourShader, VAO, parentTrans, 1.30f, 0.4f, 2.0f, 0.0f, 0.0f, 0.0f, 0.1f, 0.8f, 1.0f, glm::vec4(0.9f, 0.75f, 0.55f, 1.0f)); // fabric backrest

        // Drawing DoubleSitSofa (dark green fabric with gray legs)
        drawCube(ourShader, VAO, parentTrans, -2.0f, 0.25f, 0.0f, 0.0f, 0.0f, 0.0f, 0.99f, 0.1f, 2.99f, glm::vec4(0.0f, 0.39f, 0.3f, 1.0f)); // surface
        drawCube(ourShader, VAO, parentTrans, -2.25f, 0.25f, 0.0f, 0.0f, 0.0f, 0.0f, 0.1f, 1.5f, 2.99f, glm::vec4(0.0f, 0.39f, 0.3f, 1.0f)); // backrest
        drawCube(ourShader, VAO, parentTrans, -2.0f, 0.19f, -0.7f, 0.0f, 0.0f, 0.0f, 1.0f, 0.75f, 0.2f, glm::vec4(0.5f, 0.5f, 0.5f, 1.0f)); // leg
        drawCube(ourShader, VAO, parentTrans, -2.0f, 0.19f, 0.7f, 0.0f, 0.0f, 0.0f, 1.0f, 0.75f, 0.2f, glm::vec4(0.5f, 0.5f, 0.5f, 1.0f)); // leg
        drawCube(ourShader, VAO, parentTrans, -1.8f, 0.125f, 0.0f, 0.0f, 0.0f, 0.0f, 0.1f, 0.5f, 3.0f, glm::vec4(0.5f, 0.5f, 0.5f, 1.0f)); // leg

        // Drawing SingleSitSofa (teal blue fabric with gray legs)
        drawCube(ourShader, VAO, parentTrans, 0.0f, 0.25f, 2.0f, 0.0f, 90.0f, 0.0f, 0.9f, 0.1f, 1.499f, glm::vec4(0.0f, 0.55f, 0.55f, 1.0f)); // surface
        drawCube(ourShader, VAO, parentTrans, 0.0f, 0.25f, 2.25f, 0.0f, 90.0f, 0.0f, 0.1f, 1.5f, 1.499f, glm::vec4(0.0f, 0.55f, 0.55f, 1.0f)); // backrest
        drawCube(ourShader, VAO, parentTrans, 0.4f, 0.19f, 2.0f, 0.0f, 90.0f, 0.0f, 1.0f, 0.75f, 0.2f, glm::vec4(0.5f, 0.5f, 0.5f, 1.0f)); // leg
        drawCube(ourShader, VAO, parentTrans, -0.4f, 0.19f, 2.0f, 0.0f, 90.0f, 0.0f, 1.0f, 0.75f, 0.2f, glm::vec4(0.5f, 0.5f, 0.5f, 1.0f)); // leg
        drawCube(ourShader, VAO, parentTrans, 0.0f, 0.125f, 1.8f, 0.0f, 90.0f, 0.0f, 0.1f, 0.5f, 1.5f, glm::vec4(0.5f, 0.5f, 0.5f, 1.0f)); // leg


        // Drawing TV
        drawCube(ourShader, VAO, parentTrans, 0.0f, 1.5, -2.99, 0.0f, 0.0f, 0.0f, 3.0, 1.0, 0.05, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)); // TV

        
        // Animate the fan blades
        // Fan rotation logic
        fanRotateAngle_Y += deltaTime * 100.0f;  // Increase the rotation angle to animate fan blades
        if (fanRotateAngle_Y > 360.0f)
            fanRotateAngle_Y -= 360.0f;

        // Create a transformation matrix for the fan
        glm::mat4 fanTransform = glm::translate(parentTrans, glm::vec3(0.0f, 2.5f, 0.0f)); // Move fan above the floor
        fanTransform = glm::rotate(fanTransform, glm::radians(fanRotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate the fan around Y-axis

        // Draw Central Rod
        drawCube(ourShader, VAO, fanTransform, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.1f, 1.0f, 0.1f, glm::vec4(0.5f, 0.5f, 0.5f, 1.0f)); // Rod

        // Draw the 3 blades, using fanTransform as the parent transform
        glm::mat4 bladeTransform;

        // Blade 1 - Color Red
        bladeTransform = glm::rotate(fanTransform, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));  // No rotation, Blade 1 starts at 0 degrees
        drawCube(ourShader, VAO, bladeTransform, 0.5f, 0.0f, 0.0f, 20.0f, 0.0f, 0.0f, 2.0f, 0.05f, 0.2f, glm::vec4(0.5f, 0.5f, 0.0f, 1.0f));

        // Blade 2 - Color Green
        bladeTransform = glm::rotate(fanTransform, glm::radians(120.0f), glm::vec3(0.0f, 1.0f, 0.0f));  // Rotate 120 degrees around Y-axis
        drawCube(ourShader, VAO, bladeTransform, 0.5f, 0.0f, 0.0f, 20.0f, 0.0f, 0.0f, 2.0f, 0.05f, 0.2f, glm::vec4(0.5f, 0.5f, 0.0f, 1.0f));

        // Blade 3 - Color Blue
        bladeTransform = glm::rotate(fanTransform, glm::radians(240.0f), glm::vec3(0.0f, 1.0f, 0.0f));  // Rotate 240 degrees around Y-axis
        drawCube(ourShader, VAO, bladeTransform, 0.5f, 0.0f, 0.0f, 20.0f, 0.0f, 0.0f, 2.0f, 0.05f, 0.2f, glm::vec4(0.5f, 0.5f, 0.0f, 1.0f));


        // Swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // De-allocate resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    // Terminate GLFW
    glfwTerminate();
    return 0;
}

// Process input
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) translate_Y += 0.01;
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) translate_Y -= 0.01;
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) translate_X += 0.01;
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) translate_X -= 0.01;
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) translate_Z += 0.01;
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) translate_Z -= 0.01;

    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) rotateAngle_X += 1;
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) rotateAngle_Y += 1;
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) rotateAngle_Z += 1;

    // Camera movement
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        basic_camera.ProcessKeyboard('W', deltaTime);  // Forward
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        basic_camera.ProcessKeyboard('S', deltaTime);  // Backward
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        basic_camera.ProcessKeyboard('A', deltaTime);  // Left
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        basic_camera.ProcessKeyboard('D', deltaTime);  // Right
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        basic_camera.ProcessKeyboard('E', deltaTime);  // Up
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        basic_camera.ProcessKeyboard('R', deltaTime);  // Down

    // Camera rotation
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        basic_camera.ProcessRotation('P', deltaTime);  // Pitch up
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        basic_camera.ProcessRotation('N', deltaTime);  // Pitch down
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        basic_camera.ProcessRotation('Y', deltaTime);  // Yaw left
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        basic_camera.ProcessRotation('H', deltaTime);  // Yaw right
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        basic_camera.ProcessRotation('L', deltaTime);  // Roll counter-clockwise
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
        basic_camera.ProcessRotation('R', deltaTime);  // Roll clockwise
}

// Framebuffer size callback
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// Scroll callback
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    basic_camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

// Draw Cube Function
void drawCube(Shader shaderProgram, unsigned int VAO, glm::mat4 parentTrans,
    float posX, float posY, float posZ,
    float rotX, float rotY, float rotZ,
    float scX, float scY, float scZ,
    glm::vec4 color)
{
    shaderProgram.use();

    // Apply transformations: translation, rotation, scaling
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, model, modelCentered;
    translateMatrix = glm::translate(parentTrans, glm::vec3(posX, posY, posZ));
    rotateXMatrix = glm::rotate(translateMatrix, glm::radians(rotX), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(rotateXMatrix, glm::radians(rotY), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(rotateYMatrix, glm::radians(rotZ), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(rotateZMatrix, glm::vec3(scX, scY, scZ));
    modelCentered = glm::translate(model, glm::vec3(-0.25f, -0.25f, -0.25f));

    // Set the model transformation in the shader
    shaderProgram.setMat4("model", modelCentered);

    // Use the custom color passed to the function
    shaderProgram.setVec4("color", color);

    // Draw the cube
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}



