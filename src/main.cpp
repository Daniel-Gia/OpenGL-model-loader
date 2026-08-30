#include "Models/Model.h"
#include "Shader.h"

#include <GLFW/glfw3.h>
#include <cmath>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <stb_image.h>

const char* const ModelPath = "assets/models/dodge-car/scene.gltf";

unsigned int initialScrWidth = 1920;
unsigned int initialScrHeight = 1080;

int main() {
    // Initialize GLFW
    glfwInit();

    // define the version of OpenGL we are using
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a maximized window
    glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

    // Create the window (full screen)
    GLFWwindow* window = glfwCreateWindow(initialScrWidth, initialScrHeight, "OpenGL Model Loader", nullptr, nullptr);

    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << '\n';
        glfwTerminate();
        return -1;
    }

    // Enable V-Sync (synchronize frame rate with monitor refresh rate and prevent screen tearing)
    glfwSwapInterval(1);

    // Make the window the current OpenGL context
    glfwMakeContextCurrent(window);

    // Load GLAD so it configures OpenGL
    gladLoadGL();

    // Specify the viewport
    glViewport(0, 0, initialScrWidth, initialScrHeight);

    // Load shader programs
    Shader shaderProgram("Shaders/default.vert", "Shaders/default.frag");

    // Create camera object
    Camera camera(initialScrWidth, initialScrHeight, glm::vec3(0.0f, 0.0f, 2.0f));

    // Enable depth - for correct 3D rendering
    glEnable(GL_DEPTH_TEST);

    std::unique_ptr<Model> model = Model::load(ModelPath);

    // Model Transformations - For now hardcoded (TODO: Add GUI to change these values)
    // model->rotate(glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    // model->scale(100.0f);
    // model->setPosition(glm::vec3(10.0f, 0.0f, 0.0f));

    float deltaTime = 0.0f; // Time between current frame and last frame
    float lastFrame = 0.0f; // Time of last frame

    while (glfwWindowShouldClose(window) == 0) {
        // Set the color of the background
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Calculate delta time
        auto currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Handle Window Resize
        int width;
        int height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        camera.width = width;
        camera.height = height;

        camera.handleInputs(window, deltaTime);
        camera.updateMatrix(45.0f, 0.1f, 1000.0f);

        // glm::vec3 cameraPos = camera.Position;
        // std::cout << "Camera Position: (" << cameraPos.x << ", " << cameraPos.y << ", " << cameraPos.z << ")" << std::endl;

        model->draw(shaderProgram, camera);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up
    shaderProgram.deleteFromMemory();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
