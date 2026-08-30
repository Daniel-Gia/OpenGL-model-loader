#pragma once

#include "Shader.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

class Camera {
  public:
    glm::vec3 Position;
    glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::mat4 cameraMatrix = glm::mat4(1.0f);

    // Prevents the camera from jumping around when first clicking left click
    bool firstClick = true;

    // width and height of the window
    int width;
    int height;

    float normalSpeed = 5.0f;
    float fastSpeed = 10.0f;
    float speed;
    float sensitivity = 100.0f;

    Camera(int width, int height, glm::vec3 position);

    // Updates the camera matrix to the Vertex Shader
    void updateMatrix(float FOVdeg, float nearPlane, float farPlane);
    // Exports the camera matrix to the Vertex Shader
    void exportMatrixToShader(Shader& shader, const char* uniform);
    // Exports the camera position to the Fragment Shader (used for specular lighting)
    void exportPositionToShader(Shader& shader, const char* uniform) const;

    // Handles inputs
    void handleInputs(GLFWwindow* window, float deltaTime);
};
